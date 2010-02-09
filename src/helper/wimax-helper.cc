/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008 INRIA
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
 * Authors: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *          Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */

#include "wimax-helper.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include <string>
#include "ns3/config.h"
#include "ns3/wimax-net-device.h"
#include "ns3/bs-net-device.h"
#include "ns3/ss-net-device.h"
#include "ns3/wimax-channel.h"
#include "ns3/simple-ofdm-wimax-channel.h"
#include "ns3/wimax-phy.h"
#include "ns3/simple-ofdm-wimax-phy.h"
#include "ns3/pointer.h"

#include "ns3/pcap-writer.h"

NS_LOG_COMPONENT_DEFINE ("WimaxHelper");

namespace ns3 {

WimaxHelper::WimaxHelper (void)
  : m_channel (0)
{
}

WimaxHelper::~WimaxHelper (void)
{
}

void WimaxHelper::EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid)
{

  std::ostringstream oss;
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WimaxNetDevice/Rx";
  Config::Connect (oss.str (), MakeBoundCallback (&WimaxHelper::AsciiRxEvent, &os));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WimaxNetDevice/Tx";
  Config::Connect (oss.str (), MakeBoundCallback (&WimaxHelper::AsciiTxEvent, &os));

  // queue has to be traced on per connection basis, as below
  EnableAsciiForConnection (os, nodeid, deviceid, (char*) "WimaxNetDevice", (char*) "InitialRangingConnection");
  EnableAsciiForConnection (os, nodeid, deviceid, (char*) "WimaxNetDevice", (char*) "BroadcastConnection");
  EnableAsciiForConnection (os, nodeid, deviceid, (char*) "SubscriberStationNetDevice", (char*) "BasicConnection");
  EnableAsciiForConnection (os, nodeid, deviceid, (char*) "SubscriberStationNetDevice", (char*) "PrimaryConnection");

  /*some updates will have to be made to trace transport connection
   queues, because there are multiple transport connections per SS*/
}

void WimaxHelper::EnableAsciiForConnection (std::ostream &os,
                                            uint32_t nodeid,
                                            uint32_t deviceid,
                                            char *netdevice,
                                            char *connection)
{
  std::ostringstream oss;
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::" << netdevice << "/" << connection
      << "/TxQueue/Enqueue";
  Config::Connect (oss.str (), MakeBoundCallback (&WimaxHelper::AsciiEnqueueEvent, &os));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::" << netdevice << "/" << connection
      << "/TxQueue/Dequeue";
  Config::Connect (oss.str (), MakeBoundCallback (&WimaxHelper::AsciiDequeueEvent, &os));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::" << netdevice << "/" << connection
      << "/TxQueue/Drop";
  Config::Connect (oss.str (), MakeBoundCallback (&WimaxHelper::AsciiDropEvent, &os));
}

void WimaxHelper::EnableAscii (std::ostream &os, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnableAscii (os, dev->GetNode ()->GetId (), dev->GetIfIndex ());
    }
}

void WimaxHelper::EnableAscii (std::ostream &os, NodeContainer n)
{
  NetDeviceContainer devs;
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      for (uint32_t j = 0; j < node->GetNDevices (); ++j)
        {
          devs.Add (node->GetDevice (j));
        }
    }
  EnableAscii (os, devs);
}

void WimaxHelper::EnableAscii (std::ostream &os)
{
  EnableAscii (os, NodeContainer::GetGlobal ());
}

Ptr<WimaxPhy> WimaxHelper::CreatePhy (PhyType phyType)
{
  Ptr<WimaxPhy> phy;
  switch (phyType)
    {
    case SIMPLE_PHY_TYPE_OFDM:
      phy = CreateObject<SimpleOfdmWimaxPhy> ();
      if (!m_channel)
        {
          m_channel = CreateObject<SimpleOfdmWimaxChannel> ();
        }
      break;
    default:
      NS_FATAL_ERROR ("Invalid physical type");
      break;
    }

  return phy;
}

Ptr<WimaxPhy> WimaxHelper::CreatePhy (PhyType phyType, char * SNRTraceFilePath, bool activateLoss)
{
  Ptr<WimaxPhy> phy;
  Ptr<SimpleOfdmWimaxPhy> sphy;
  switch (phyType)
    {
    case SIMPLE_PHY_TYPE_OFDM:
      sphy = CreateObject<SimpleOfdmWimaxPhy> ();
      phy = sphy;
      sphy->SetSNRToBlockErrorRateTracesPath (SNRTraceFilePath);
      sphy->ActivateLoss (activateLoss);
      if (!m_channel)
        {
          m_channel = CreateObject<SimpleOfdmWimaxChannel> ();
        }
      break;
    default:
      NS_FATAL_ERROR ("Invalid physical type");
      break;
    }

  return phy;
}

Ptr<WimaxPhy> WimaxHelper::CreatePhyWithoutChannel (PhyType phyType)
{
  Ptr<WimaxPhy> phy;
  switch (phyType)
    {
    case SIMPLE_PHY_TYPE_OFDM:
      phy = CreateObject<SimpleOfdmWimaxPhy> ();
      break;
    default:
      NS_FATAL_ERROR ("Invalid physical type");
      break;
    }

  return phy;
}

Ptr<WimaxPhy> WimaxHelper::CreatePhyWithoutChannel (PhyType phyType, char * SNRTraceFilePath, bool activateLoss)
{
  Ptr<WimaxPhy> phy;
  Ptr<SimpleOfdmWimaxPhy> sphy;
  switch (phyType)
    {
    case SIMPLE_PHY_TYPE_OFDM:
      sphy = CreateObject<SimpleOfdmWimaxPhy> ();
      phy = sphy;
      sphy->SetSNRToBlockErrorRateTracesPath (SNRTraceFilePath);
      sphy->ActivateLoss (activateLoss);
      break;
    default:
      NS_FATAL_ERROR ("Invalid physical type");
      break;
    }

  return phy;
}

Ptr<UplinkScheduler> WimaxHelper::CreateUplinkScheduler (SchedulerType schedulerType)
{
  Ptr<UplinkScheduler> uplinkScheduler;
  switch (schedulerType)
    {
    case SCHED_TYPE_SIMPLE:
      uplinkScheduler = CreateObject<UplinkSchedulerSimple> ();
      break;
    case SCHED_TYPE_RTPS:
      uplinkScheduler = CreateObject<UplinkSchedulerRtps> ();
      break;
    case SCHED_TYPE_MBQOS:
      uplinkScheduler = CreateObject<UplinkSchedulerMBQoS> (Seconds (0.25));
      break;
    default:
      NS_FATAL_ERROR ("Invalid scheduling type");
      break;
    }
  return uplinkScheduler;
}

Ptr<BSScheduler> WimaxHelper::CreateBSScheduler (SchedulerType schedulerType)
{
  Ptr<BSScheduler> bsScheduler;
  switch (schedulerType)
    {
    case SCHED_TYPE_SIMPLE:
      bsScheduler = CreateObject<BSSchedulerSimple> ();
      break;
    case SCHED_TYPE_RTPS:
      bsScheduler = CreateObject<BSSchedulerRtps> ();
      break;
    case SCHED_TYPE_MBQOS:
      bsScheduler = CreateObject<BSSchedulerSimple> ();
      break;
    default:
      NS_FATAL_ERROR ("Invalid scheduling type");
      break;
    }
  return bsScheduler;
}

NetDeviceContainer WimaxHelper::Install (NodeContainer c,
                                         NetDeviceType deviceType,
                                         PhyType phyType,
                                         SchedulerType schedulerType,
                                         double frameDuration)
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;
      Ptr<WimaxPhy> phy = CreatePhy (phyType);

      // Set SuperFrame Duration
      phy->SetFrameDuration (Seconds (frameDuration));

      Ptr<WimaxNetDevice> device;
      Ptr<UplinkScheduler> uplinkScheduler = CreateUplinkScheduler (schedulerType);
      Ptr<BSScheduler> bsScheduler = CreateBSScheduler (schedulerType);

      if (deviceType == DEVICE_TYPE_BASE_STATION)
        {
          // attach phy
          Ptr<BaseStationNetDevice> deviceBS;
          deviceBS = CreateObject<BaseStationNetDevice> (node, phy, uplinkScheduler, bsScheduler);
          device = deviceBS;
          uplinkScheduler->SetBs (deviceBS);
          bsScheduler->SetBs (deviceBS);
        }
      else
        {
          device = CreateObject<SubscriberStationNetDevice> (node, phy);
        }
      device->SetAddress (Mac48Address::Allocate ());
      phy->SetDevice (device);
      device->Start ();
      device->Attach (m_channel); // attach channel

      node->AddDevice (device);

      devices.Add (device);
    }
  return devices;
}

NetDeviceContainer WimaxHelper::Install (NodeContainer c,
                                         NetDeviceType deviceType,
                                         PhyType phyType,
                                         SchedulerType schedulerType)
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;
      Ptr<WimaxPhy> phy = CreatePhy (phyType);
      Ptr<WimaxNetDevice> device;
      Ptr<UplinkScheduler> uplinkScheduler = CreateUplinkScheduler (schedulerType);
      Ptr<BSScheduler> bsScheduler = CreateBSScheduler (schedulerType);

      if (deviceType == DEVICE_TYPE_BASE_STATION)
        {
          // attach phy
          Ptr<BaseStationNetDevice> deviceBS;
          deviceBS = CreateObject<BaseStationNetDevice> (node, phy, uplinkScheduler, bsScheduler);
          device = deviceBS;
          uplinkScheduler->SetBs (deviceBS);
          bsScheduler->SetBs (deviceBS);
        }
      else
        {
          device = CreateObject<SubscriberStationNetDevice> (node, phy);
        }
      device->SetAddress (Mac48Address::Allocate ());
      phy->SetDevice (device);
      device->Start ();
      device->Attach (m_channel); // attach channel

      node->AddDevice (device);

      devices.Add (device);
    }
  return devices;
}

NetDeviceContainer WimaxHelper::Install (NodeContainer c,
                                         NetDeviceType deviceType,
                                         PhyType phyType,
                                         Ptr<WimaxChannel> channel,
                                         SchedulerType schedulerType)
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;

      Ptr<WimaxPhy> phy = CreatePhyWithoutChannel (phyType, (char*) "dummy", 0);
      Ptr<WimaxNetDevice> device;
      Ptr<UplinkScheduler> uplinkScheduler = CreateUplinkScheduler (schedulerType);
      Ptr<BSScheduler> bsScheduler = CreateBSScheduler (schedulerType);

      if (deviceType == DEVICE_TYPE_BASE_STATION)
        {
          Ptr<BaseStationNetDevice> deviceBS;
          deviceBS = CreateObject<BaseStationNetDevice> (node, phy, uplinkScheduler, bsScheduler);
          device = deviceBS;
          uplinkScheduler->SetBs (deviceBS);
          bsScheduler->SetBs (deviceBS);
        }
      else
        {
          device = CreateObject<SubscriberStationNetDevice> (node, phy);
        }
      device->SetAddress (Mac48Address::Allocate ());
      phy->SetDevice (device);
      device->Start ();
      device->Attach (channel);

      node->AddDevice (device);
      devices.Add (device);
    }
  return devices;
}

Ptr<WimaxNetDevice> WimaxHelper::Install (Ptr<Node> node,
                                          NetDeviceType deviceType,
                                          PhyType phyType,
                                          Ptr<WimaxChannel> channel,
                                          SchedulerType schedulerType)
{

  // Ptr<WimaxPhy> phy = CreatePhyWithoutChannel (phyType);
  Ptr<WimaxPhy> phy = CreatePhyWithoutChannel (phyType, (char*) "dummy", 0);
  Ptr<WimaxNetDevice> device;
  Ptr<UplinkScheduler> uplinkScheduler = CreateUplinkScheduler (schedulerType);
  Ptr<BSScheduler> bsScheduler = CreateBSScheduler (schedulerType);

  if (deviceType == DEVICE_TYPE_BASE_STATION)
    {
      Ptr<BaseStationNetDevice> deviceBS;
      deviceBS = CreateObject<BaseStationNetDevice> (node, phy, uplinkScheduler, bsScheduler);
      device = deviceBS;
      uplinkScheduler->SetBs (deviceBS);
      bsScheduler->SetBs (deviceBS);
    }
  else
    {
      device = CreateObject<SubscriberStationNetDevice> (node, phy);
    }
  device->SetAddress (Mac48Address::Allocate ());
  phy->SetDevice (device);
  device->Start ();
  device->Attach (channel);

  node->AddDevice (device);

  return device;
}

void WimaxHelper::AsciiEnqueueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "+ " << Simulator::Now ().GetSeconds () << " ";
  *os << path << std::endl;
}

void WimaxHelper::AsciiDequeueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "- " << Simulator::Now ().GetSeconds () << " ";
  *os << path << std::endl;
}

void WimaxHelper::AsciiDropEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "d " << Simulator::Now ().GetSeconds () << " ";
  *os << path << std::endl;
}

void WimaxHelper::AsciiRxEvent (std::ostream *os,
                                std::string path,
                                Ptr<const Packet> packet,
                                const Mac48Address &source)
{
  *os << "r " << Simulator::Now ().GetSeconds () << " from: " << source << " ";
  *os << path << std::endl;
}

void WimaxHelper::AsciiTxEvent (std::ostream *os, std::string path, Ptr<const Packet> packet, const Mac48Address &dest)
{
  *os << "t " << Simulator::Now ().GetSeconds () << " to: " << dest << " ";
  *os << path << std::endl;
}

static void PcapPhyTxEvent (Ptr<PcapWriter> writer,
                            Ptr<const PacketBurst> burst,
                            WimaxPhy::ModulationType modulationType,
                            uint16_t m_nrBlocks,
                            uint16_t m_blockSize)
{

  std::list<Ptr<Packet> > packets = burst->GetPackets ();

  for (std::list<Ptr<Packet> >::iterator iter = packets.begin (); iter != packets.end (); ++iter)
    {
      writer->WriteWimaxPacket (*iter);
    }

}

static void PcapPhyRxEvent (Ptr<PcapWriter> writer, Ptr<const PacketBurst> burst)
{
  std::list<Ptr<Packet> > packets = burst->GetPackets ();

  for (std::list<Ptr<Packet> >::iterator iter = packets.begin (); iter != packets.end (); ++iter)
    {
      writer->WriteWimaxPacket (*iter);
    }
}

void WimaxHelper::EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid)
{
  std::ostringstream oss;
  oss << filename << "-" << nodeid << "-" << deviceid << ".pcap";
  Ptr<PcapWriter> pcap = Create<PcapWriter> ();
  pcap->Open (oss.str ());
  pcap->WriteWimaxM2MHeader ();
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WimaxNetDevice/Phy/Tx";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&PcapPhyTxEvent, pcap));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WimaxNetDevice/Phy/Rx";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&PcapPhyRxEvent, pcap));
}

ServiceFlow WimaxHelper::CreateServiceFlow (ServiceFlow::Direction direction,
                                            ServiceFlow::SchedulingType schedulinType,
                                            IpcsClassifierRecord classifier)
{
  CsParameters csParam (CsParameters::ADD, classifier);
  ServiceFlow serviceFlow = ServiceFlow (direction);
  serviceFlow.SetConvergenceSublayerParam (csParam);
  serviceFlow.SetCsSpecification (ServiceFlow::IPV4);
  serviceFlow.SetServiceSchedulingType (schedulinType);
  serviceFlow.SetMaxSustainedTrafficRate (100);
  serviceFlow.SetMinReservedTrafficRate (1000000);
  serviceFlow.SetMinTolerableTrafficRate (1000000);
  serviceFlow.SetMaximumLatency (100);
  serviceFlow.SetMaxTrafficBurst (2000);
  serviceFlow.SetTrafficPriority (1);
  serviceFlow.SetUnsolicitedGrantInterval (1);
  serviceFlow.SetMaxSustainedTrafficRate (70);
  serviceFlow.SetToleratedJitter (10);
  serviceFlow.SetSduSize (49);
  serviceFlow.SetRequestTransmissionPolicy (0);
  return serviceFlow;
}
} // namespace ns3
