/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 */

#include <sstream>

#include "ns3/log.h"
#include "print.h"

#include "rm-report-reader.h"

NS_LOG_COMPONENT_DEFINE ("rm-report-reader");

using namespace std;

namespace ns3
{

 RmReportReader::RmReportReader ()
 {
  // Some general lines
  m_errorMsg = "Unrecognized report structure.";
  m_dashLine = "---------------------------------------------------------------------------";
  m_blankLine = "";
  m_doubleSpace = "  ";
  m_space = " ";
 }

 RmReportReader::~RmReportReader () { }

 TypeId
 RmReportReader::GetTypeId (void)
 {
  static TypeId tid = TypeId ("ns3::RmReportReader")
          .SetParent<Object > ()
          ;
  return tid;
 }

 uint16_t
 RmReportReader::Section2vector (ifstream &file, string begining, string header, vector<string> &section)
 {
  string line;

  /* Structure:
   * m_dashLine
   * sectionBeginig
   * m_dashLine
   * sectionHeader
   * m_blankLine
   * data
   *
   * e.g.:
   * -----------
   * section
   * -----------
   * header
   *
   * data
   */

  while (line.compare (begining) != 0)
   {
    getline (file, line); //advance in the file
   }
  getline (file, line);
  if (line.compare (m_dashLine) == 0)
   {
    if (header.compare (m_blankLine) != 0)
     {//Has header
      getline (file, line);
      if (line.compare (header) == 0)
       {
        getline (file, line);
        if (line.compare (m_blankLine) == 0)
         {
          getline (file, line);
          while (line.compare (m_dashLine) != 0 && file.eof () != 1)// end of section
           {
            section.push_back (line);
            NS_LOG_DEBUG ("sec: " << line);
            getline (file, line);
           }
         }
        else // m_blankLine
         {
          NS_LOG_DEBUG (m_errorMsg << " 2 ");
          return -2;
         }
       }
     }
    else // no header
     {
      getline (file, line);
      while (line.compare (m_dashLine) != 0 && file.eof () != 1)// end of section
       {
        section.push_back (line);
        NS_LOG_DEBUG ("sec no header: " << line);
        getline (file, line);
       }
     }// no header
   }
  else // m_dashLine
   {
    NS_LOG_DEBUG (m_errorMsg << " 1 ");
    return -1;
   }

  return 0;
 }

 uint16_t
 RmReportReader::GetNodeNames (vector<string> sectionInfo, vector<string> &sectionNames)
 {
  // Separation character combination used: DOUBLE SPACE
  // max size 20 chars
  // http://www.bgsu.edu/departments/compsci/docs/string.html
  string elementInfo;
  string name;
  string::size_type name_max_length = 20;

  for (uint16_t i = 0; i < sectionInfo.size (); i++)
   {
    elementInfo = sectionInfo.at (i);
    name = RmReportReader::GetName (elementInfo, name_max_length);
    sectionNames.push_back (name);
   }
  return 0;
 }

 string
 RmReportReader::GetName (string line, string::size_type max_length)
 {
  string name;

  NS_LOG_DEBUG ("Line: #" << line << "#");
  string::size_type found = line.find (m_doubleSpace); //double space
  NS_LOG_DEBUG ("double space found: " << found);
  if (found > max_length)
   {
    found = max_length;
    NS_LOG_DEBUG ("  Found reduced: " << found);
   }
  name = line.substr (0, found);
  NS_LOG_DEBUG ("Name limited: #" << name << "#");

  //Cleaning spaces from the name, at the begining and at the end.
  found = line.find (m_space);
  if (found == 0)
   {
    NS_LOG_DEBUG ("Begining: space found: " << found);
    name = line.substr (1, name.size () - 1);
    NS_LOG_DEBUG ("Name without first space: #" << name << "#");
   }
  /*
    //must verify if the last char is an space: e.g. #Laykatuyoc Troncal #
    // Does it affect on something?
    string lastChar = line.at (line.size () - 1);
    NS_LOG_DEBUG ("End: space found: " << found << " last char #" << lastChar << "#");
    if (m_space.compare (lastChar))
     {
      NS_LOG_DEBUG ("End: space found: " << found);
      name = line.substr (0, line.size () -1);
   * NS_LOG_DEBUG ("Name without last space: #" << name << "#");
   */
  NS_LOG_DEBUG ("Name: #" << name << "#"); // delimiters to distiguish spaces

  return name;
 }

 uint16_t
 RmReportReader::GetNets (vector<string> netsInfo, vector< vector<string> > &netInfoMatrix)
 {
  // blank line (at the begining)
  // Number. Name
  // 7 lines
  // blank line
  // "Net members:               #  01 02 ... 0n Role:         System:             Antenna:"
  // Member names (n)                (matrix)    Terminal/Node  (system)            high? m
  // blank line
  // "                           Quality = 50 - number of resend"
  // blank line (at the end)

  string eon = "                           Quality = 50 - number of resend"; // End of Net
  uint16_t j = 0; // for debbuging
  for (uint16_t i = 0; i < netsInfo.size (); i++)
   {
    vector<string> netInfo;
    i++; // Blank Line at the begining
    while (eon.compare (netsInfo.at (i)) != 0)
     {
      NS_LOG_DEBUG ("net info: " << i << "," << j << " " << netsInfo.at (i));
      netInfo.push_back (netsInfo.at (i++));
     }
    NS_LOG_DEBUG ("net info (out of while): " << i << "," << j << " " << netsInfo.at (i) << endl);
    netInfo.push_back (netsInfo.at (i)); // we delete last line = blank line
    i++; // Blank line at the end

    netInfoMatrix.push_back (netInfo);
    j++;
   }
  return 0;
 }

 uint16_t
 RmReportReader::GetSubnetData (vector<vector<string> > netInfoMatrix, vector<NetDataStruct::SubnetData> &vSubnetData)
 {
  /* Generalization:
   * n. Net name (beeing n the number of net)
   * Cluster topology
   * 2400.0 MHz to 2483.0 MHz
   * Vertical polarization
   * Mode of variability is Broadcast, at
   * 50% of time, 50% of locations, 50% of situations
   * Refractivity= 301 N-units, conductivity= 0.005 S/m, permittivity= 15
   * Continental temperate climate
   * blank line
   * Net members:
   *  j lines (beeing j the number of nodes of the net)
   * blank line
   * string eon = "                           Quality = 50 - number of resend"; // En of Net
   *
   *
   */
  vector<string> netInfo;
  string line;
  string lineOne;
  string netName;
  string::size_type found;
  string::size_type name_max_length = 27;

  for (uint16_t i = 0; i < netInfoMatrix.size (); i++)
   {
    NetDataStruct::SubnetData subnetData;
    netInfo = netInfoMatrix.at (i);
    vector<string> members;
    NS_LOG_DEBUG ("init");
    Print::VectorString (members);

    uint16_t j = 0;
    line = netInfo.at (j); //Net name
    ostringstream oss;
    oss << i + 1 << ".";
    lineOne = oss.str ();
    NS_LOG_DEBUG ("~~~~Net number: " << i + 1);
    NS_LOG_DEBUG (lineOne);

    found = line.find (lineOne);

    NS_LOG_DEBUG ("found point: " << found);
    netName = line.substr (found + lineOne.size (), line.size ());
    subnetData.name = RmReportReader::GetName (netName, name_max_length);
    NS_LOG_DEBUG ("Net name: #" << subnetData.name << "#" << " i: " << i << " j: " << j); // delimiters to distiguish spaces
    //must verify if the last char is an space: e.g. #Laykatuyoc Troncal #


    j += 9; // go to Net members line

    string lineMembers = "Net members:";
    line = netInfo.at (j++); //Net Members:
    found = line.find (lineMembers);
    if (found == 0)// Line 9 = Net members line => known format
     {
      while (netInfo.at (j).compare (m_blankLine) != 0)
       {
        line = netInfo.at (j++);
        NS_LOG_DEBUG ("line members: " << line << " i:" << i << " j++:" << j);
        members.push_back (line);
       }
     }
    else // Net Members
     {
      NS_LOG_DEBUG (m_errorMsg << " m1 ");
      return -1;
     }

    //members processing
    GetMembersData (members, subnetData);

    vSubnetData.push_back (subnetData);
   }
  return 0;
 }

 uint16_t
 RmReportReader::GetMembersData (vector<string> members, NetDataStruct::SubnetData &subnetData)
 {
  /* e.g.
 Net members:               #  01 02 03 Role:         System:             Antenna:
 LAYKATUYOC                 01    50 00 Node          Laykatuyoc Troncal P10.0m
 ACCHA BRIDGE               02 50       Terminal      Accha Repetidor Clie5.0m
 HUASCAR                    03 00       Terminal      Huascar Troncal PCMC11.0m
   */
  string member;
  string node;
  string matrix;
  string role;
  string system;

  //empirical
  string::size_type name_max_length = 27;
  string::size_type matrix_max_length = 3 * (members.size () + 1);
  string::size_type role_max_length = 14;
  string::size_type system_max_length = 20;

  uint16_t i_b; // begining index
  uint16_t i_e; // end index
  //uint16_t eoName = 26; // End of Name

  // Get substrings
  for (uint16_t i = 0; i < members.size (); i++)
   {
    NS_LOG_DEBUG ("  member: " << i);
    member = members.at (i);
    i_e = -1; // init

    i_b = i_e + 1;
    i_e += name_max_length;
    node = member.substr (i_b, i_e);

    i_b = i_e + 1;
    i_e += matrix_max_length;
    matrix = member.substr (i_b, i_e);

    i_b = i_e + 1;
    i_e += role_max_length;
    role = member.substr (i_b, i_e);

    i_b = i_e + 1;
    i_e += system_max_length;
    system = member.substr (i_b, i_e);

    subnetData.nodes.push_back (GetName (node, name_max_length));
    subnetData.roles.push_back (GetName (role, role_max_length));
    //Matrix not processed
    subnetData.systems.push_back (GetName (system, system_max_length));
   }
  return 0;
 }

 NetDataStruct::NetData
 RmReportReader::ReadRmReport (ifstream &file)
 {
  string line;
  vector<NetDataStruct::SubnetData> vSubnetData;
  NetDataStruct::NetData netData;
  NS_LOG_DEBUG ("vSubnetData empty?: " << vSubnetData.empty ());

  if (file.is_open ())
   {
    uint16_t retval;

    /*
     * ----------------
     * Reading Sections
     * ----------------
     */


    /*
     * Reading Node names
     */

    string generalBegining = "General information";
    string generalHeader = "";
    vector<string> generalInfo;
    retval = Section2vector (file, generalBegining, generalHeader, generalInfo);

    netData.generalInfo = generalInfo;

    NS_LOG_DEBUG ("end of general info " << retval);


    /*
     * Reading Node names
     */
    string nodesBegining = "Active units information";
    string nodesHeader = "Name                Location                                     Elevation";
    vector<string> nodesInfo;

    retval = Section2vector (file, nodesBegining, nodesHeader, nodesInfo);

    NS_LOG_DEBUG ("end of nodes " << retval);

    /*
     * Systems
     */
    string systemsBegining = "Systems";
    string systemsHeader = "Name                Pwr Tx    Loss  Loss (+)  Rx thr.   Ant. G. Ant. Type";
    vector<string> systemsInfo;

    retval = Section2vector (file, systemsBegining, systemsHeader, systemsInfo);

    NS_LOG_DEBUG ("end of systems " << retval);

    /*
     * Links / Nets
     */
    string netsBegining = "Active nets information";
    string netsHeader = "";
    vector<string> netsInfo;

    retval = Section2vector (file, netsBegining, netsHeader, netsInfo);

    NS_LOG_DEBUG ("end of nets " << retval);


    while (!file.eof ())
     {
      getline (file, line);
     }
    NS_LOG_DEBUG ("End of file reached");

    file.close ();

    /*
     * -------------
     * Reading names
     * -------------
     */

    /*
     * Getting node names
     */
    vector<string> nodeNames;
    retval = GetNodeNames (nodesInfo, nodeNames);

    netData.nodesInfo.names = nodeNames;

    /*
     * Getting system names
     */
    vector<string> systemNames;
    retval = GetNodeNames (systemsInfo, systemNames);

    netData.systemsInfo.names = systemNames;

    /*
     * Separating nets
     */
    vector< vector<string> > netInfoMatrix;
    retval = GetNets (netsInfo, netInfoMatrix);

    /*
     * Getting Net parameters
     */

    retval = GetSubnetData (netInfoMatrix, vSubnetData);

    netData.vSubnetData = vSubnetData;

   }// .is_open
  else
   {
    NS_LOG_DEBUG ("Unable to open file");
    netData.nodesInfo.names.at (0) = "empty";
   }

  return netData;
 }

} // namespace ns3