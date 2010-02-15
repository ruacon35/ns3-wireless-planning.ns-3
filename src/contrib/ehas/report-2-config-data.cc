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

#include "ns3/log.h"
#include "util.h"

#include "report-2-config-data.h"
#include "net-data-struct.h"

NS_LOG_COMPONENT_DEFINE ("report-2-config-data");

namespace ns3
{

 Report2ConfigData::Report2ConfigData () { }

 Report2ConfigData::~Report2ConfigData () { }

 TypeId
 Report2ConfigData::GetTypeId (void)
 {
  static TypeId tid = TypeId ("ns3::Report2ConfigData")
    .SetParent<Object> ()
  ;
  return tid;
 }

 NetworkConfig::NetworkData
 Report2ConfigData::NetData2NetworkData (NetDataStruct::NetData net)
 {
  NetworkConfig::NetworkData network;

  NetworkConfig::VectorNodeData vectorNodeData;

  vector<string> nodeNames = net.nodesInfo.names;

  for (uint16_t i = 0; i < nodeNames.size (); i++) // loop throw nodes
   {
    NetworkConfig::NodeData node;
    node.name = nodeNames.at (i);
    NS_LOG_INFO ("Node: " << i << " " << node.name);
    for (uint16_t j = 0; j < net.vSubnetData.size (); j++) // loop throw subnets
     {
      NS_LOG_INFO ("  subnet: " << j);
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
  return network;
 }

 void
 Report2ConfigData::AddDevice2Node (NetworkConfig::NodeData &node, NetDataStruct::SubnetData subnet, uint16_t index, uint16_t chId)
 {
  NS_LOG_INFO ("index: " << index << " " << subnet.roles.at (index));
  NetworkConfig::MacType macType = Role2MacType (subnet.roles.at (index));
  NetworkConfig::DeviceData deviceData = m_config.SetDeviceData (chId, macType);
  node.vectorDeviceData.push_back (deviceData);
 }

 NetworkConfig::MacType
 Report2ConfigData::Role2MacType (string role)
 {
  NetworkConfig::MacType macType = NetworkConfig::QSTA;
  vector<string> roles;

  roles.push_back ("Node"); //0
  roles.push_back ("Terminal"); //1
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
