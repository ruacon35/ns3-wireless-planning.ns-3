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
#include "ns3/object.h"
#include "ns3/ipv4.h"
#include "ns3/node.h"
#include "ns3/names.h"
#include "ns3/mobility-model.h"

#include "print.h"
#include "net-data-struct.h"

#include <iostream>
#include <string>
#include <fstream>

NS_LOG_COMPONENT_DEFINE ("print");

namespace ns3
{

 Print::Print () { }

 Print::~Print () { }

 TypeId
 Print::GetTypeId (void)
 {
  static TypeId tid = TypeId ("ns3::Print")
          .SetParent<Object > ()
          ;
  return tid;
 }

 void
 Print::NodeList (NodeContainer nodes)
 {
  Ptr< Node > node_p;
  Ptr< NetDevice > device_p;

  Ptr<Ipv4> ipv4;
  Ipv4InterfaceAddress iaddr;
  Ipv4Address m_ipAddr;
  Address m_addr;

  NS_LOG_INFO ("PrintNodeList");
  for (uint32_t i = 0; i < nodes.GetN (); i++)
   {
    node_p = nodes.Get (i);
    std::string nodeName = Names::FindName (node_p);
    NS_LOG_INFO ("NodeId: " << node_p->GetId () << " Name: " << nodeName);

    // creates a new one, does not get the installed one.
    Ptr<MobilityModel> mobility = node_p->GetObject <MobilityModel> ();
    Vector pos = mobility->GetPosition ();
    NS_LOG_INFO ("  Mobility Model: " << mobility->GetInstanceTypeId ());
    NS_LOG_INFO ("  Position (x,y,z) [m]: " << pos.x << " " << pos.y << " " << pos.z);
    for (uint32_t j = 0; j < node_p->GetNDevices (); j++)
     {
      ipv4 = node_p->GetObject<Ipv4 > ();
      iaddr = ipv4->GetAddress (j, 0); //I/F, Address index
      m_ipAddr = iaddr.GetLocal ();

      device_p = node_p->GetDevice (j);
      m_addr = device_p->GetAddress ();
      NS_LOG_INFO ("  Device " << j << " IP " << m_ipAddr << " MAC " << m_addr);
     }
   }
 }

 void
 Print::VectorString (std::vector<std::string> vStr)
 {
  std::string str;
  for (uint32_t i = 0; i < vStr.size (); i++)
   {
    str = vStr.at (i);
    NS_LOG_INFO (i << " " << str);
   }
 }

 void
 Print::VectorSubnet (NetDataStruct::VectorSubnetData vSubnetData)
 {
  NetDataStruct::SubnetData subnetData;
  for (uint8_t i = 0; i < vSubnetData.size (); i++)
   {
    subnetData = vSubnetData.at (i);
    NS_LOG_INFO ("  -- subnet name --");
    NS_LOG_INFO (1 + i << "- " << subnetData.name);
    NS_LOG_INFO ("    -- node --       -- role --       -- distance --       -- mode --       -- system --");
    for (uint8_t j = 0; j < subnetData.nodes.size (); j++)
     {
      NS_LOG_INFO ("    " << 1 + j << "- " << subnetData.nodes.at (j) << "    "
              << subnetData.roles.at (j) << "    " << subnetData.distances.at (j)
              << "    " << subnetData.standard.at (j) << "    " << subnetData.systems.at (j));
     }
   }
 }

 void
 Print::Netinfo (NetDataStruct::NetData netData)
 {
  NS_LOG_INFO ("-- -- Netinfo -- --");
  NS_LOG_INFO ("-- general info --");
  VectorString (netData.generalInfo);

  NS_LOG_INFO ("-- nodes info --");
  VectorString (netData.nodesInfo.names);

  NS_LOG_INFO ("-- subnet info --");
  VectorSubnet (netData.vSubnetData);
 }

 void
 Print::NodeData (NetworkConfig::VectorNodeData vectorNodeData)
 {
  NetworkConfig::NodeData nodeData;
  uint32_t i_if = 0; // interface index = device index
  NS_LOG_DEBUG ("Node data: " << vectorNodeData.size () << " nodes.");

  for (uint32_t i = 0; i < vectorNodeData.size (); i++)
   {
    nodeData = vectorNodeData.at (i);
    uint16_t nDev = nodeData.vectorDeviceData.size (); // number of devices
    NS_LOG_DEBUG ("  Node: " << i << " " << nodeData.name << " has " << nDev << " device(s)");
    NS_LOG_DEBUG ("   Position: (x,y,z) [m] " << nodeData.position.x << " " << nodeData.position.y << " " << nodeData.position.z);

    for (uint32_t j = 0; j < nDev; j++)
     {
      NetworkConfig::DeviceData deviceData = nodeData.vectorDeviceData.at (j);
      NS_LOG_DEBUG ("   Device: " << j << " in total " << i_if);
      i_if++;
      NS_LOG_DEBUG ("    Channel ID: " << deviceData.chId);
      NS_LOG_DEBUG ("    IP address: " << deviceData.ipAddress);
      NS_LOG_DEBUG ("    Name: " << deviceData.name);
      NS_LOG_DEBUG ("    MAC type: " << deviceData.wifiMacType);
      NS_LOG_DEBUG ("    Distance to AP: " << deviceData.distance);
     }
   }
 }

 void
 Print::ChannelData (NetworkConfig::VectorChannelData vectorChannelData)
 {
   NetworkConfig::VectorWifiChannelData vWifiChData = vectorChannelData.vWifiChData;
//   NetworkConfig::VectorWifiChannelData vWimaxChData = vectorChannelData.vWimaxChData;

  NetworkConfig::WifiChannelData wifiChData;
  NS_LOG_DEBUG ("Channel data: " << vWifiChData.size () << " channels.");
  for (uint32_t i = 0; i < vectorChannelData.vWifiChData.size (); i++)
   {
    wifiChData = vWifiChData.at (i);
    NS_LOG_DEBUG (" Wifi Ch: " << i + 1 << " Mode: " << wifiChData.wifiMode);
   }
 }

 void
 Print::NetworkData (NetworkConfig::NetworkData networkData)
 {
  NodeData (networkData.vectorNodeData);
  ChannelData (networkData.vectorChannelData);
 }
 
} // namespace ns3
