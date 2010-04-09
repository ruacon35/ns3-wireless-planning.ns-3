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
// Default network topology includes a base station (BS) 
// and a subscriber station (SS).

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
#include "ns3/flow-monitor-module.h"
NS_LOG_COMPONENT_DEFINE ("wimax-ptp");

using namespace ns3;

int main (int argc, char *argv[])
{
  bool verbose = false;

  double duration = 1.25;
  int schedType = 0;
  WimaxHelper::SchedulerType scheduler = WimaxHelper::SCHED_TYPE_SIMPLE;

  CommandLine cmd;
  cmd.AddValue ("scheduler", "type of scheduler to use with the network devices", schedType);
  cmd.AddValue ("duration", "duration of the simulation in seconds", duration);
  cmd.AddValue ("verbose", "turn on all WimaxNetDevice log components", verbose);
  cmd.Parse (argc, argv);
  LogComponentEnable ("OnOffApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);
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

  ssNodes.Create (1);
  bsNodes.Create (1);

  WimaxHelper wimax;

  NetDeviceContainer ssDevs, bsDevs;

  ssDevs = wimax.Install (ssNodes,
                          WimaxHelper::DEVICE_TYPE_SUBSCRIBER_STATION,
                          WimaxHelper::SIMPLE_PHY_TYPE_OFDM,
                          scheduler);
  bsDevs = wimax.Install (bsNodes, WimaxHelper::DEVICE_TYPE_BASE_STATION, WimaxHelper::SIMPLE_PHY_TYPE_OFDM, scheduler);

  wimax.EnableAscii ("bs-devices", bsDevs);
  wimax.EnableAscii ("ss-devices", ssDevs);

  Ptr<SubscriberStationNetDevice> ss[1];

  for (int i = 0; i < 1; i++)
    {
      ss[i] = ssDevs.Get (i)->GetObject<SubscriberStationNetDevice> ();
      ss[i]->SetModulationType (WimaxPhy::MODULATION_TYPE_QAM16_12);
    }
    // MODULATION_TYPE_BPSK_12, MODULATION_TYPE_QPSK_12, MODULATION_TYPE_QPSK_34, MODULATION_TYPE_QAM16_12, MODULATION_TYPE_QAM16_34, MODULATION_TYPE_QAM64_23, MODULATION_TYPE_QAM64_34

  Ptr<BaseStationNetDevice> bs;

  bs = bsDevs.Get (0)->GetObject<BaseStationNetDevice> ();

  InternetStackHelper stack;
  stack.Install (bsNodes);
  stack.Install (ssNodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer SSinterfaces = address.Assign (ssDevs);
  Ipv4InterfaceContainer BSinterface = address.Assign (bsDevs);

  if (verbose)
    {
      wimax.EnableLogComponents ();  // Turn on all wimax logging
    }
    
  // FlowMonitor example based on examples/wireless/wifi-hidden-terminal.cc 
  // 8. Install FlowMonitor on all nodes
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll(); 
    
  /*------------------------------*/
  // Create the OnOff application
  NS_LOG_INFO ("Create Source");
  InetSocketAddress dst = InetSocketAddress (SSinterfaces.GetAddress (0));
  OnOffHelper onoff = OnOffHelper ("ns3::Ipv4RawSocketFactory", dst);
  onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1.0)));
  onoff.SetAttribute ("DataRate", DataRateValue (3000000));//bps
  onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0.0)));
  onoff.SetAttribute ("PacketSize", UintegerValue (1480));

  ApplicationContainer clientApps = onoff.Install (bsNodes.Get (0));
  
  clientApps.Start (Seconds (1));
  clientApps.Stop (Seconds (duration));

  NS_LOG_INFO ("Create Sink.");
  PacketSinkHelper sink = PacketSinkHelper ("ns3::Ipv4RawSocketFactory", dst);
  ApplicationContainer serverApps = sink.Install (ssNodes.Get (0));
  serverApps.Stop (Seconds (duration + 0.25));
  serverApps.Start (Seconds (0.9));

  Simulator::Stop (Seconds (duration + 0.5));

  wimax.EnablePcap ("wimax-simple-ss0", ssNodes.Get (0)->GetId (), ss[0]->GetIfIndex ());
  wimax.EnablePcap ("wimax-simple-bs0", bsNodes.Get (0)->GetId (), bs->GetIfIndex ());

  IpcsClassifierRecord DlClassifierUgs (Ipv4Address ("0.0.0.0"),
                                        Ipv4Mask ("0.0.0.0"),
                                        SSinterfaces.GetAddress (0),
                                        Ipv4Mask ("255.255.255.255"),
                                        0,
                                        65000,
                                        100,
                                        100,
                                        17,
                                        1);
  ServiceFlow DlServiceFlow = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_DOWN,
                                                          ServiceFlow::SF_TYPE_BE,
                                                          DlClassifierUgs);

  ss[0]->AddServiceFlow (DlServiceFlow);

  NS_LOG_INFO ("Starting simulation.....");
  Simulator::Run ();
 
  // 10. Print per flow statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
      std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
      std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
      std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 10.0 / 1024 / 1024  << " Mbps\n";
    }

  monitor->SerializeToXmlFile ("wimax-ptp-flowmon.xml", true, true);

  ss[0] = 0;
  bs = 0;

  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  return 0;
}


    
        
