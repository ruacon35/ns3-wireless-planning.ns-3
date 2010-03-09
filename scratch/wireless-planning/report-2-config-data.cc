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

#include <stdlib.h>

#include "ns3/log.h"
#include "util.h"

#include "report-2-config-data.h"
#include "net-data-struct.h"

NS_LOG_COMPONENT_DEFINE ("report-2-config-data");

namespace ns3 {

  Report2ConfigData::Report2ConfigData ()
  {
  }

  Report2ConfigData::~Report2ConfigData ()
  {
  }

  TypeId
  Report2ConfigData::GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::Report2ConfigData")
            .SetParent<Object > ()
            ;
    return tid;
  }

  NetworkConfig::NetworkData
  Report2ConfigData::NetData2NetworkData (NetDataStruct::NetData net)
  {
    NetworkConfig::NetworkData network;

    NetworkConfig::VectorNodeData vectorNodeData;

    NetDataStruct::NodesInfo nodesInfo = net.nodesInfo;
    vector<string> nodeNames = nodesInfo.names;

    for (uint16_t i = 0; i < nodeNames.size (); i++) // loop throw NODES
    {
      NetworkConfig::NodeData node;
      node.name = nodeNames.at (i);

      NetDataStruct::Position pos = nodesInfo.positions.at (i);
      Vector position (pos.x, pos.y, pos.z);
      node.position = position;

      NS_LOG_INFO ("Node: " << i << " " << node.name);
      for (uint16_t j = 0; j < net.vSubnetData.size (); j++) // inner loop throw SUBNETS
      {
        NetDataStruct::SubnetData subnet = net.vSubnetData.at (j);
        // Search if the node has a device in each subnet
        int16_t index = Util::FindStrInVectorStr (node.name, subnet.nodes);
        if (index >= 0)// found
        {
          AddDevice2Node (node, subnet, index, j + 1);
        }
      }
      vectorNodeData.push_back (node); // add node to the tail of the vector.
    }
    network.vectorNodeData = vectorNodeData;

    NetworkConfig config;
    NetworkConfig::VectorChannelData vectorChannelData;

    for (uint16_t i = 0; i < net.vSubnetData.size (); i++)
    {
      //Wifi
      string mode = net.vSubnetData.at (i).systems.at (0); //All the nodes of a wifi ch have the same mode.
      config.SetWifiChannelData (i, mode, vectorChannelData.vWifiChData);
      //Wimax
    }
    network.vectorChannelData.vWifiChData = vectorChannelData.vWifiChData;

    return network;
  }

  void
  Report2ConfigData::AddDevice2Node (NetworkConfig::NodeData &node, NetDataStruct::SubnetData subnet, uint16_t index, uint16_t chId)
  {
    string commStandard = subnet.standard;
    enum NetworkConfig::CommunicationStandard standard;
    standard = ReadStandard (commStandard);

    double distance = subnet.distances.at (index);
    NetworkConfig::DeviceData deviceData;

    switch (standard)
    {
      case NetworkConfig::WIFI :
      {
        NS_LOG_DEBUG ("  adding Wi-Fi device in ch " << chId);
        NetworkConfig::MacType macType = Role2MacType (subnet.roles.at (index));
        deviceData = m_config.SetWifiDeviceData (chId, macType, distance);
        break;
      }
      case NetworkConfig::WIMAX :
      {
        NS_LOG_DEBUG ("  adding WiMAX device in ch " << chId);
        break;
      }
      default:
        NS_ASSERT_MSG (0,
                "  No correct communication standard selected");
    }


    node.vectorDeviceData.push_back (deviceData);
  }

  NetworkConfig::CommunicationStandard
  Report2ConfigData::ReadStandard (string communicationStandard)
  {
    enum NetworkConfig::CommunicationStandard retval;
    if (communicationStandard.compare ("wifi") == 0)
    {
      retval = NetworkConfig::WIFI;
    } else if (communicationStandard.compare ("wimax") == 0)
    {
      retval = NetworkConfig::WIMAX;
    }
    return retval;
  }

  NetworkConfig::MacType
  Report2ConfigData::Role2MacType (string role)
  {
    NetworkConfig::MacType macType = NetworkConfig::QSTA;
    vector<string> roles;

    roles.push_back ("AP"); //0
    roles.push_back ("STA"); //1
    roles.push_back ("Master"); //2
    roles.push_back ("Slave"); //3

    // Correspondance between Roles and NetworkConfig::MacType 3 = QAP, 4 = QSTA
    uint16_t table [] = {3, 4, 3, 4};

    int match;
    for (uint16_t i = 0; i < roles.size (); i++)
    {
      match = roles.at (i).compare (role);
      if (match == 0)
      {
        macType = NetworkConfig::MacType (table [i]);
        break;
      }
    }
    NS_LOG_INFO ("role: " << role << " MAC type: " << macType);
    return macType;
  }

} // namespace ns3
