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

    /*
     * Channels
     */
    NetworkConfig config;
    NetworkConfig::VectorChannelData vectorChannelData;
    vector<uint16_t> chIds;
    uint16_t wifiChId = 0, wimaxChId = 0;
    
    for (uint16_t i = 0; i < net.vSubnetData.size (); i++)
    {
      NetDataStruct::SubnetData subNetData = net.vSubnetData.at (i);
      string system = subNetData.systems.at (0); // all nodes of the same net have the same wifi mode

      enum NetworkConfig::CommunicationStandard standard = ReadStandard (subNetData.standard);
      switch (standard)
      {
        case NetworkConfig::WIFI :
        {
          string mode = system;
          chIds.push_back(wifiChId);
          config.SetWifiChannelData (wifiChId, mode, vectorChannelData.vWifiChData);
          wifiChId++;
          break;
        }
        case NetworkConfig::WIMAX :
        {
          chIds.push_back(wimaxChId);
          NetworkConfig::WimaxChannelData channel;
          channel.id = wimaxChId;
          vectorChannelData.vWimaxChData.push_back (channel);
          wimaxChId++;
          break;
        }
        default:
          NS_ASSERT_MSG (0, "No correct communication standard selected");
      }
    }
    network.vectorChannelData = vectorChannelData;

    /*
     * Nodes
     */
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
        NS_LOG_INFO ("  subnet: " << j);
        NetDataStruct::SubnetData subnet = net.vSubnetData.at (j);
        // Search if the node has a device in each subnet
        int16_t index = Util::FindStrInVectorStr (node.name, subnet.nodes);
        if (index >= 0)// found
        {
          uint8_t chId = chIds.at (j);
          AddDevice2Node (node, subnet, index, chId+1);
        }
      }
      vectorNodeData.push_back (node); // add node to the tail of the vector.
    }
    network.vectorNodeData = vectorNodeData;


    return network;
  }

  void
  Report2ConfigData::AddDevice2Node (NetworkConfig::NodeData &node, NetDataStruct::SubnetData subnet, uint16_t index, uint16_t chId)
  {
    NetworkConfig config;
    enum NetworkConfig::CommunicationStandard standard = ReadStandard (subnet.standard);
    string role = subnet.roles.at (index);

    double distance = subnet.distances.at (index);
    double max_distance = subnet.max_distance;
    NetworkConfig::DeviceData deviceData;

    switch (standard)
    {
      case NetworkConfig::WIFI :
      {
        NS_LOG_DEBUG ("  adding Wi-Fi device in ch " << chId);
        NetworkConfig::MacType macType = Role2MacType (role);
        deviceData = config.SetWifiDeviceData (chId, macType, distance, max_distance);
        break;
      }
      case NetworkConfig::WIMAX :
      {
        NS_LOG_DEBUG ("  adding WiMAX device in ch " << chId);
        WimaxHelper::NetDeviceType deviceType = Role2WimaxDeviceType (role);
        string modulation = subnet.systems.at (index);
        WimaxPhy::ModulationType mod = ReadModulation (modulation);

        deviceData = config.SetWimaxDeviceData (chId, distance, max_distance, 
                deviceType, WimaxHelper::SCHED_TYPE_RTPS, mod);
        break;
      }
      default:
        NS_ASSERT_MSG (0, "The communication standard selected is not correct.");
    }
    node.vectorDeviceData.push_back (deviceData);
  }

  NetworkConfig::CommunicationStandard
  Report2ConfigData::ReadStandard (string communicationStandard)
  {
    enum NetworkConfig::CommunicationStandard retval = NetworkConfig::WIFI;
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

  WimaxHelper::NetDeviceType
  Report2ConfigData::Role2WimaxDeviceType (string role)
  {
    WimaxHelper::NetDeviceType deviceType = WimaxHelper::DEVICE_TYPE_SUBSCRIBER_STATION;
    vector<string> roles;

    roles.push_back ("SS"); //0
    roles.push_back ("BS"); //1

    int match;
    for (uint16_t i = 0; i < roles.size (); i++)
    {
      match = roles.at (i).compare (role);
      if (match == 0)
      {
        deviceType = WimaxHelper::NetDeviceType (i);
        break;
      }
    }
    NS_LOG_INFO ("role: " << role << " wimax device type: " << deviceType);
    return deviceType;
  }

  WimaxPhy::ModulationType
  Report2ConfigData::ReadModulation (string modulation)
  {
    WimaxPhy::ModulationType mod;
    vector<string> mods;
    // MODULATION_TYPE_BPSK_12, MODULATION_TYPE_QPSK_12, MODULATION_TYPE_QPSK_34, MODULATION_TYPE_QAM16_12,
    // MODULATION_TYPE_QAM16_34, MODULATION_TYPE_QAM64_23, MODULATION_TYPE_QAM64_34

    mods.push_back ("BPSK 1/2"); // 0
    mods.push_back ("QPSK 1/2"); // 1
    mods.push_back ("QPSK 3/4"); // 2
    mods.push_back ("16QAM 1/2"); // 3
    mods.push_back ("16QAM 3/4"); // 4
    mods.push_back ("64QAM 2/3"); // 5
    mods.push_back ("64QAM 3/4"); // 6

    int match;
    for (uint16_t i = 0; i < mods.size (); i++)
    {
      match = mods.at (i).compare (modulation);
      if (match == 0)
      {
        mod = WimaxPhy::ModulationType (i);
        break;
      } else if (modulation.compare ("All") == 0)
      {
        mod = WimaxPhy::ModulationType (6); // 6?
      }
    }
    NS_LOG_INFO ("modulation: " << modulation << " modulation enum: " << mod);
    return mod;
  }

} // namespace ns3
