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

#include "netinfo-reader.h"

NS_LOG_COMPONENT_DEFINE ("netinfo-reader");

using namespace std;

namespace ns3 {

vector<string> 
NetinfoReader::split (const string &s, char delim) 
{
  vector<string> elems;
  stringstream ss(s);
  string item;
  while(getline(ss, item, delim)) 
    {
      elems.push_back(item);
    }
  return elems;
}

vector<string>
NetinfoReader::GetSection (vector<string> &lines, string start, string end)
{
  vector<string> output_lines;
  string line;
  bool inSection = false;

  for (uint32_t i = 0; i < lines.size(); i++) 
   {
     line = lines[i];
     if (!inSection && line == start) { inSection = true; }                
     else if (inSection && line.find (end) == 0) { break; }
     else if (inSection && line != "") { output_lines.push_back (line); }
   }
  return output_lines;
}

vector<string>
NetinfoReader::GetSubSectionNames (vector<string> &lines, string header)
{
  vector<string> names;
  for (uint32_t i = 0; i < lines.size(); i++)
    {
      string line = lines[i];
      if (line.find (header) == 0)
        {
          string current_net = string (line, header.size(), line.size() - header.size());
          names.push_back (current_net);
        }
    } 
  return (names);
}

NetDataStruct::NetData
NetinfoReader::Read (ifstream &file)
{ 
  NetDataStruct::NetData netData;
  vector<string> netinfo_lines, lines;
  string line;
    
  while (getline(file, line))
     netinfo_lines.push_back (line);
     
  // General information
  netData.generalInfo = GetSection (netinfo_lines, string("= General information"), string("= "));

  // Nodes
  lines = GetSection (netinfo_lines, string("= Nodes"), string("= "));
  for (uint32_t i = 0; i < lines.size(); i++)
    {
      netData.nodesInfo.names.push_back(lines[i]);
    }

  // Nets
  string net_header ("== ");
  lines = GetSection (netinfo_lines, string("= Nets"), string("= "));
  vector<string> net_names = GetSubSectionNames (lines, net_header);

  for (uint32_t i = 0; i < net_names.size(); i++)
    {
      NetDataStruct::SubnetData subnetData;
      string net_name = net_names[i];
      vector<string> net_lines = GetSection (lines, net_header+net_name, net_header);
      subnetData.name = net_name;
      vector<string> node_info = split(net_lines[0], ' ');
      subnetData.mode = node_info[1];      
      //subnetData.distance = node_info[2];
      for (uint32_t j = 2; j < net_lines.size(); j++) 
        {
        string net_line = net_lines[j];
        vector<string> fields = split(net_line, '\t');
        subnetData.nodes.push_back (fields[0]);
        subnetData.roles.push_back (fields[1]);
        }
      netData.vSubnetData.push_back (subnetData); 
    }    
  return netData;
}

} // namespace ns3
