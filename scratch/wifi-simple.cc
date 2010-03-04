/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/yans-wifi-phy.h"


// Default Network Topology
//
//   Wifi 10.1.3.0
//         STA   AP
//          *    *
//          |    |    
//         n1   n0
//                                                    

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WifiSimple");

int
main (int argc, char *argv[])
{
 bool verbose = true;
 uint32_t nWifi = 2;
 Time ackTimeout = MilliSeconds (100);
 double distance = 100000.0;

 CommandLine cmd;
 cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
 cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
 cmd.AddValue ("ACKTimeout", "Set ACKTimeout for all MACs, 100ms by default", ackTimeout);
 cmd.AddValue ("distance", "Set between nodes, 100.000m by default", distance);

 cmd.Parse (argc, argv);

 if (verbose)
  {
   LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
   LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  }

 NodeContainer wifiApNode;
 wifiApNode.Create (1);

 NodeContainer wifiStaNodes;
 wifiStaNodes.Create (nWifi);

 /*
  * WiFi MAC & PHY configuration
  */

 YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
 YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
 //phy.Set ("RxGain", DoubleValue (10.0) );
 //phy.Set ("TxGain", DoubleValue (20.0) );
 channel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
 channel.AddPropagationLoss ("ns3::FixedRssLossModel", "Rss", DoubleValue (0));//dBm
 //channel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
 //channel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
 phy.SetChannel (channel.Create ());

 WifiHelper wifi = WifiHelper::Default ();

 wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                               "DataMode", StringValue ("wifia-6mbs"),
                               "RtsCtsThreshold", StringValue ("2200")); // disable

 NS_LOG_INFO ("ACKTimeout: " << ackTimeout);
 NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();
 Ssid ssid = Ssid ("simple-wifi-ssid");

 
 mac.SetType ("ns3::NqapWifiMac",
              "Ssid", SsidValue (ssid),
              "BeaconGeneration", BooleanValue (true),
              "BeaconInterval", TimeValue (Seconds (2.5)),
              "AckTimeout", TimeValue (ackTimeout));

 NetDeviceContainer apDevices;
 apDevices = wifi.Install (phy, mac, wifiApNode);

 mac.SetType ("ns3::NqstaWifiMac",
              "Ssid", SsidValue (ssid),
              "ActiveProbing", BooleanValue (false),
              "AckTimeout", TimeValue (ackTimeout));

 NetDeviceContainer staDevices;
 staDevices = wifi.Install (phy, mac, wifiStaNodes.Get (1));

 /*
  * Mobility
  */
 NS_LOG_INFO ("Distance: " << distance);
 MobilityHelper mobility;

 Ptr<ListPositionAllocator> positionAllocAp = CreateObject<ListPositionAllocator > ();
 positionAllocAp->Add (Vector (0.0, 0.0, 0.0));
 mobility.SetPositionAllocator (positionAllocAp);
 mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
 mobility.Install (wifiApNode);

// Ptr<ListPositionAllocator> positionAllocSta= CreateObject<ListPositionAllocator > ();
// positionAllocSta->Add (Vector (dist, 0.0, 0.0));
// mobility.SetPositionAllocator (positionAllocSta);
// mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
 mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel",
     "Position", VectorValue (Vector(distance, 0.0, 0.0)));
 mobility.Install (wifiStaNodes);

 InternetStackHelper stack;
 stack.Install (wifiApNode);
 stack.Install (wifiStaNodes);

 Ipv4AddressHelper address;

 address.SetBase ("10.1.3.0", "255.255.255.0");
 address.Assign (staDevices);
 address.Assign (apDevices);

 UdpEchoServerHelper echoServer (9);

 ApplicationContainer serverApps = echoServer.Install (wifiApNode);
 serverApps.Start (Seconds (1.0));
 serverApps.Stop (Seconds (10.0));

// Ipv4Address serverIpAddr = Util::GetIpAddrFromNode (wifiApNode.Get (0));

  Ptr<Ipv4> ipv4 = wifiApNode.Get (0)->GetObject<Ipv4 > ();
  Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1, 0);
  Ipv4Address serverIpAddr = iaddr.GetLocal ();

 UdpEchoClientHelper echoClient (serverIpAddr, 9);
 echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
 echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
 echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

 ApplicationContainer clientApps =
         echoClient.Install (wifiStaNodes.Get (0));
 clientApps.Start (Seconds (2.0));
 clientApps.Stop (Seconds (10.0));

 Ipv4GlobalRoutingHelper::PopulateRoutingTables ();



//  /*
//  * Setup all the plot system: throughput measurement, gnuplot issues...
//  */
// NetMeasure netMeasure (Seconds (10.0), Seconds (0.1));
// netMeasure.SetupPlot ();
//
// netMeasure.SetFlowMonitor (NodeContainer::GetGlobal ());
// netMeasure.GetFlowStats ();

 phy.EnablePcap ("wifi-simple", wifiApNode);

 Simulator::Stop (Seconds (10.0));

 Simulator::Run ();
 Simulator::Destroy ();
 return 0;
}
