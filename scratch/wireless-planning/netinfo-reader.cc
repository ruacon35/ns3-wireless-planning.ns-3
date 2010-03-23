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
    stringstream ss (s);
    string item;
    while (getline (ss, item, delim))
    {
      elems.push_back (item);
    }
    return elems;
  }

  vector<string>
  NetinfoReader::GetSection (vector<string> &lines, string start, string end)
  {
    vector<string> output_lines;
    string line;
    bool inSection = false;

    for (uint32_t i = 0; i < lines.size (); i++)
    {
      line = lines[i];
      if (!inSection && line == start)
      {
        inSection = true;
      }
      else if (inSection && line.find (end) == 0)
      {
        break;
      } else if (inSection && line != "")
      {
        output_lines.push_back (line);
      }
    }
    return output_lines;
  }

  vector<string>
  NetinfoReader::GetSubSectionNames (vector<string> &lines, string header)
  {
    vector<string> names;
    for (uint32_t i = 0; i < lines.size (); i++)
    {
      string line = lines[i];
      if (line.find (header) == 0)
      {
        string current_net = string (line, header.size (), line.size () - header.size ());
        names.push_back (current_net);
      }
    }
    return (names);
  }

  NetDataStruct::NetData
  NetinfoReader::Read (string filepath)
  {
    ifstream file (filepath.c_str());
    NS_ASSERT_MSG(file.is_open(), "Netinfo file not found: " << filepath);
    NetDataStruct::NetData netData;
    vector<string> netinfo_lines, lines;
    string line;    

    while (getline (file, line))
      netinfo_lines.push_back (line);

    // General information
    netData.generalInfo = GetSection (netinfo_lines, string ("= General information"), string ("= "));

    // Nodes
    lines = GetSection (netinfo_lines, string ("= Nodes"), string ("= "));
    for (uint32_t i = 0; i < lines.size (); i++)
    {
      vector<string> fields = split (lines[i], '\t');
      netData.nodesInfo.names.push_back (fields[0]);
      NetDataStruct::Position position;
      vector<string> positions = split (fields[3], ',');
      std::istringstream x (positions[0]);
      x >> position.x;
      std::istringstream y (positions[1]);
      y >> position.y;
      std::istringstream z (fields[1]);
      z >> position.z;
      NS_LOG_DEBUG ("Position (x y z) " << position.x << " " << position.y << " " << position.z);
      netData.nodesInfo.positions.push_back (position);
    }

    // Nets
    string net_header ("== ");
    lines = GetSection (netinfo_lines, string ("= Nets"), string ("= "));
    vector<string> net_names = GetSubSectionNames (lines, net_header);

    //  netData.vSubnetData.clear (); SIZE = 614133570 !?!
    for (uint32_t i = 0; i < net_names.size (); i++)
    {
      NetDataStruct::SubnetData subnetData;
      double distance;
      double max_distance = 0;

      string net_name = net_names[i];
      subnetData.name = net_name;
      vector<string> net_lines = GetSection (lines, net_header + net_name, net_header);

      vector<string> node_info = split (net_lines[0], ' ');
      subnetData.standard = node_info[1];
      for (uint32_t j = 2; j < net_lines.size (); j++)
      {
        string net_line = net_lines[j];
        vector<string> fields = split (net_line, '\t');
        subnetData.nodes.push_back (fields[0]);
        subnetData.roles.push_back (fields[1]);
        std::istringstream sdistance (fields[2]);
        sdistance >> distance;
        subnetData.distances.push_back (distance);
        if (distance > max_distance)
          max_distance = distance;
        // If system has Name + Mode must remove the name.
        subnetData.systems.push_back (fields[3]); // system = mode or modulation
      }
      subnetData.max_distance = max_distance;
      netData.vSubnetData.push_back (subnetData);
    }
    return netData;
  }

} // namespace ns3
