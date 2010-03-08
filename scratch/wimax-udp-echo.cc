/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2007,2008, 2009 INRIA, UDcast
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                              <amine.ismail@udcast.com>
 */

//
// Default network topology includes a base station (BS) and 2
// subscriber station (SS).

//      +-----+
//      | SS0 |
//      +-----+
//     10.1.1.1
//      -------
//        ((*))
//
//                  10.1.1.7
//               +------------+
//               |Base Station| ==((*))
//               +------------+
//
//        ((*))
//       -------
//      10.1.1.2
//       +-----+
//       | SS1 |
//       +-----+

#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/mobility-module.h"
#include "ns3/contrib-module.h"
#include "ns3/wimax-module.h"
#include <iostream>
#include "ns3/global-route-manager.h"
#include "ns3/ipcs-classifier-record.h"
#include "ns3/service-flow.h"

NS_LOG_COMPONENT_DEFINE ("wimaxIpV4Simulation");

using namespace ns3;

int main (int argc, char *argv[])
{
  LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_FUNCTION);
  LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_FUNCTION);
  //LogComponentEnable ("SubscriberStationNetDevice", LOG_LEVEL_INFO);
  // LogComponentEnable ("BaseStationNetDevice", LOG_LEVEL_INFO);
   LogComponentEnable ("IpcsClassifier", LOG_LEVEL_INFO);
   LogComponentEnable ("IpcsClassifierRecord", LOG_LEVEL_INFO);
  // LogComponentEnable ("BsServiceFlowManager", LOG_LEVEL_INFO);
  // LogComponentEnable (SSScheduler", LOG_LEVEL_INFO);
  // LogComponentEnable ("BSSchedulerSimple", LOG_LEVEL_INFO);
  // default values
  int duration = 2, schedType = 0;
  WimaxHelper::SchedulerType scheduler = WimaxHelper::SCHED_TYPE_SIMPLE;

  CommandLine cmd;
  cmd.AddValue ("scheduler", "type of scheduler to use with the network devices", schedType);
  cmd.AddValue ("duration", "duration of the simulation in seconds", duration);
  cmd.Parse (argc, argv);

  switch (schedType)
    {
    case 0:
      scheduler = WimaxHelper::SCHED_TYPE_SIMPLE;
      break;
    case 1:
      scheduler = WimaxHelper::SCHED_TYPE_MBQOS;
      break;
    case 2:
      scheduler = WimaxHelper::SCHED_TYPE_RTPS;
      break;
    default:
      scheduler = WimaxHelper::SCHED_TYPE_SIMPLE;
    }

  NodeContainer ssNodes;
  NodeContainer bsNodes;

  ssNodes.Create (2);
  bsNodes.Create (1);

  WimaxHelper wimax;

  NetDeviceContainer ssDevs, bsDevs;

/* enum  	NetDeviceType  { DEVICE_TYPE_SUBSCRIBER_STATION, DEVICE_TYPE_BASE_STATION  }
enum  	PhyType { SIMPLE_PHY_TYPE_OFDM }
enum  	SchedulerType { SCHED_TYPE_SIMPLE, SCHED_TYPE_RTPS, SCHED_TYPE_MBQOS }

*/

/* NetDeviceContainer  ns3::WimaxHelper::Install  	(  	NodeContainer   	 c,
		NetDeviceType  	deviceType,
		PhyType  	phyType,
		Ptr< WimaxChannel >  	channel,
		SchedulerType  	schedulerType	 
	) 			

Parameters:
    	c 	A set of nodes.
    	deviceType 	Device type to create.
    	phyType 	PHY type to create.
    	channel 	A channel to use.
    	schedulerType 	The scheduling mechanism.*/
  ssDevs = wimax.Install (ssNodes,
                          WimaxHelper::DEVICE_TYPE_SUBSCRIBER_STATION,
                          WimaxHelper::SIMPLE_PHY_TYPE_OFDM,
                          scheduler);
  bsDevs = wimax.Install (bsNodes, 
                          WimaxHelper::DEVICE_TYPE_BASE_STATION,
                          WimaxHelper::SIMPLE_PHY_TYPE_OFDM, 
                          scheduler);

  Ptr<SubscriberStationNetDevice> ss[2];

  for (int i = 0; i < 2; i++)
    {
      ss[i] = ssDevs.Get (i)->GetObject<SubscriberStationNetDevice> ();
      ss[i]->SetModulationType (WimaxPhy::MODULATION_TYPE_QAM16_12);
    }

  Ptr<BaseStationNetDevice> bs;

  bs = bsDevs.Get (0)->GetObject<BaseStationNetDevice> ();

  InternetStackHelper stack;
  stack.Install (bsNodes);
  stack.Install (ssNodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer SSinterfaces = address.Assign (ssDevs);
  Ipv4InterfaceContainer BSinterface = address.Assign (bsDevs);

  /*------------------------------*/

  ApplicationContainer serverApps;
  ApplicationContainer clientApps;

  UdpEchoServerHelper echoServer (9);
  serverApps = echoServer.Install (ssNodes.Get (0));
  serverApps.Start (Seconds (1));
  serverApps.Stop (Seconds (duration));

  UdpEchoClientHelper echoClient (SSinterfaces.GetAddress (0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  clientApps = echoClient.Install (ssNodes.Get (1));
  clientApps.Start (Seconds (1));
  clientApps.Stop (Seconds (duration));

  Simulator::Stop (Seconds (duration + 0.1));

/* enum  	Direction { SF_DIRECTION_DOWN, SF_DIRECTION_UP  }
enum  	Type { SF_TYPE_PROVISIONED, SF_TYPE_ADMITTED, SF_TYPE_ACTIVE }
enum  	SchedulingType {
  SF_TYPE_NONE = 0, SF_TYPE_UNDEF = 1, SF_TYPE_BE = 2, SF_TYPE_NRTPS = 3,
  SF_TYPE_RTPS = 4, SF_TYPE_UGS = 6, SF_TYPE_ALL = 255
}
enum  	CsSpecification {
  ATM = 99, IPV4 = 100, IPV6 = 101, ETHERNET = 102,
  VLAN = 103, IPV4_OVER_ETHERNET = 104, IPV6_OVER_ETHERNET = 105, IPV4_OVER_VLAN = 106,
  IPV6_OVER_VLAN = 107
}
enum  	ModulationType {
  MODULATION_TYPE_BPSK_12, MODULATION_TYPE_QPSK_12, MODULATION_TYPE_QPSK_34, MODULATION_TYPE_QAM16_12,
  MODULATION_TYPE_QAM16_34, MODULATION_TYPE_QAM64_23, MODULATION_TYPE_QAM64_34
}*/

/* IpcsClassifierRecord (Ipv4Address  srcAddress, Ipv4Mask  srcMask, Ipv4Address  dstAddress, Ipv4Mask  dstMask, uint16_t srcPortLow, uint16_t srcPortHigh, uint16_t dstPortLow, uint16_t dstPortHigh, uint8_t protocol, uint8_t priority) */

  IpcsClassifierRecord DlClassifierUgs (Ipv4Address ("0.0.0.0"),
                                        Ipv4Mask ("0.0.0.0"),
                                        SSinterfaces.GetAddress (0),
                                        Ipv4Mask ("255.255.255.255"),
                                        0,
                                        65000,
                                        0,
                                        65001,
                                        17,
                                        1);
  ServiceFlow DlServiceFlowUgs = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_DOWN,
                                                          ServiceFlow::SF_TYPE_RTPS,
                                                          DlClassifierUgs);
  IpcsClassifierRecord UlClassifierUgs2 (SSinterfaces.GetAddress (0),
                                        Ipv4Mask ("255.255.255.255"),
                                        Ipv4Address ("0.0.0.0"),
                                        Ipv4Mask ("0.0.0.0"),
                                        0,
                                        65000,
                                        0,
                                        65002,
                                        17,
                                        1);
                                                          
  ServiceFlow UlServiceFlowUgs2 = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_UP,
                                                          ServiceFlow::SF_TYPE_RTPS,
                                                          UlClassifierUgs2);
  ss[0]->AddServiceFlow (DlServiceFlowUgs);
  ss[0]->AddServiceFlow (UlServiceFlowUgs2);
                                                          
/* ServiceFlow  ns3::WimaxHelper::CreateServiceFlow  	(  	ServiceFlow::Direction   	 direction,
		ServiceFlow::SchedulingType  	schedulinType,
		IpcsClassifierRecord  	classifier	 
	) 			

Parameters:
    	direction 	the Tirection of the service flow: UP or DOWN.
    	schedulinType 	The service scheduling type to be used: UGS, RTPS, NRTPS, BE
    	csParam 	The convergence sub-layer parameters to be used */
    	
  IpcsClassifierRecord UlClassifierUgs (SSinterfaces.GetAddress (1),
                                        Ipv4Mask ("255.255.255.255"),
                                        Ipv4Address ("0.0.0.0"),
                                        Ipv4Mask ("0.0.0.0"),
                                        0,
                                        65000,
                                        0,
                                        65000,
                                        17,
                                        1);
  ServiceFlow UlServiceFlowUgs = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_UP,
                                                          ServiceFlow::SF_TYPE_RTPS,
                                                          UlClassifierUgs);
  IpcsClassifierRecord DlClassifierUgs2 (Ipv4Address ("0.0.0.0"),
                                        Ipv4Mask ("0.0.0.0"),
                                        SSinterfaces.GetAddress (1),
                                        Ipv4Mask ("255.255.255.255"),
                                        0,
                                        65000,
                                        0,
                                        65000,
                                        17,
                                        1);
  ServiceFlow DlServiceFlowUgs2 = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_DOWN,
                                                          ServiceFlow::SF_TYPE_RTPS,
                                                          DlClassifierUgs2);
  ss[1]->AddServiceFlow (UlServiceFlowUgs);
  ss[1]->AddServiceFlow (DlServiceFlowUgs2);

  NS_LOG_INFO ("Starting simulation.....");
  Simulator::Run ();

  ss[0] = 0;
  ss[1] = 0;
  bs = 0;

  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  return 0;
}
