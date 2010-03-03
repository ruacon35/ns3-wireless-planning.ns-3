/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008, 2009 INRIA, UDCAST
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
 *          Amine Ismail <amine.ismail@sophia.inria.fr>
 */

#ifndef WIMAX_HELPER_H
#define WIMAX_HELPER_H

#include <string>
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"
#include "ns3/bs-net-device.h"
#include "ns3/ss-net-device.h"
#include "ns3/deprecated.h"
#include "ns3/service-flow.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/bs-uplink-scheduler.h"
#include "ns3/bs-uplink-scheduler-mbqos.h"
#include "ns3/bs-uplink-scheduler-simple.h"
#include "ns3/bs-uplink-scheduler-rtps.h"
#include "ns3/bs-scheduler.h"
#include "ns3/bs-scheduler-simple.h"
#include "ns3/bs-scheduler-rtps.h"
namespace ns3 {

class WimaxChannel;
class WimaxPhy;
class UplinkScheduler;

/**
 * \brief build a set of WimaxNetDevice objects
 */
class WimaxHelper
{
public:
  /**
   * Net Device Type
   * Distinguish a subscriber station(SS) device from base station(BS) device
   */
  enum NetDeviceType
  {
    DEVICE_TYPE_SUBSCRIBER_STATION, /**< Subscriber station(SS) device */
    DEVICE_TYPE_BASE_STATION
    /**< Base station(BS) device */
  };

  /**
   * WiMAX Physical layer
   * WiMAX Physical layers with different levels of detail
   */
  enum PhyType
  {
    SIMPLE_PHY_TYPE_OFDM
  };

  /**
   * Scheduler Type
   * Different implementations of uplink/downlink scheduler
   */
  enum SchedulerType
  {
    SCHED_TYPE_SIMPLE, /**< A simple priority-based FCFS scheduler */
    SCHED_TYPE_RTPS, /**< A simple scheduler - rtPS based scheduler */
    SCHED_TYPE_MBQOS
    /**< An migration-based uplink scheduler */

  };

  WimaxHelper (void);
  ~WimaxHelper (void);

  static void EnableAsciiForConnection (std::ostream &os,
                                        uint32_t nodeid,
                                        uint32_t deviceid,
                                        char *netdevice,
                                        char *connection);
  static void EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid);
  static void EnableAscii (std::ostream &os, NetDeviceContainer d);
  static void EnableAscii (std::ostream &os, NodeContainer n);
  static void EnableAscii (std::ostream &os);

  /**
   * \param filename filename prefix to use for pcap files.
   * \param nodeid the id of the node to generate pcap output for.
   * \param deviceid the id of the device to generate pcap output for.
   *
   * Generate a pcap file which contains the link-level data observed
   * by the specified deviceid within the specified nodeid. The pcap
   * data is stored in the file prefix-nodeid-deviceid.pcap.
   *
   * This method should be invoked after the network topology has
   * been fully constructed.
   */
  static void EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid);

  /**
   * \param phyType WiMAX Physical layer type
   * \return WiMAX Phy object
   *
   * Creates a physical layer object to be used in simulation.
   */
  Ptr<WimaxPhy> CreatePhy (PhyType phyType);

  /**
   * \param schedulerType Scheduling mechanism
   * \return Uplink scheduler
   *
   * Creates a uplink scheduler to be used by base station
   * according to selected scheduling mechanism.
   */
  Ptr<UplinkScheduler> CreateUplinkScheduler (SchedulerType schedulerType);

  /**
   * \param schedulerType Scheduling mechanism
   * \return Downlink scheduler
   *
   * Creates a downlink scheduler to be used by base station
   * according to selected scheduling mechanism.
   */
  Ptr<BSScheduler> CreateBSScheduler (SchedulerType schedulerType);

  /**
   * \param c a set of nodes
   * \param type device type to create
   * \param phyType a phy to use
   * \param schedulerType a scheduling mechanism
   *
   * For each of the input nodes, a new WiMAX net device (either
   * ns3::SubscriberStationNetDevice or ns3::BaseStationNetDevice
   * depending on the type parameter) is attached to the shared input channel.
   */
  NetDeviceContainer Install (NodeContainer c, NetDeviceType type, PhyType phyType, SchedulerType schedulerType);

  /**
   * \param c A set of nodes.
   * \param deviceType Device type to create.
   * \param phyType PHY type to create.
   * \param channel A channel to use.
   * \param schedulerType The scheduling mechanism.
   *
   * For each of the input nodes, a new WiMAX net device (either
   * ns3::SubscriberStationNetDevice or ns3::BaseStationNetDevice
   * depending on the type parameter) is attached to the shared input channel.
   */
  NetDeviceContainer Install (NodeContainer c,
                              NetDeviceType deviceType,
                              PhyType phyType,
                              Ptr<WimaxChannel> channel,
                              SchedulerType schedulerType);
  /**
   * \param c A set of nodes.
   * \param deviceType Device type to create.
   * \param phyType PHY type to create.
   * \param channel A channel to use.
   * \param schedulerType The scheduling mechanism.
   * \param frameDuration the super farme duration in seconds
   *
   * For each of the input nodes, a new WiMAX net device (either
   * ns3::SubscriberStationNetDevice or ns3::BaseStationNetDevice
   * depending on the type parameter) is attached to the shared input channel.
   */
  NetDeviceContainer Install (NodeContainer c,
                              NetDeviceType deviceType,
                              PhyType phyType,
                              SchedulerType schedulerType,
                              double frameDuration);

  /**
   * \param phyType WiMAX Physical layer type
   * \return WiMAX Phy object
   *
   * Creates a physical layer without a channel
   */
  Ptr<WimaxPhy> CreatePhyWithoutChannel (PhyType phyType);

  /**
   * \param phyType WiMAX Physical layer type
   * \param SNRTraceFilePath of the repository containing the SNR traces files
   * \param activateLoss set to 1 to activate losses 0 otherwise
   * \return WiMAX Phy object
   *
   * Creates a physical layer without a channel
   */
  Ptr<WimaxPhy> CreatePhyWithoutChannel (PhyType phyType, char * SNRTraceFilePath, bool activateLoss);

  /**
   * \param phyType WiMAX Physical layer type
   * \param SNRTraceFilePath of the repository containing the SNR traces files
   * \param activateLoss set to 1 to activate losses 0 otherwise
   * \return WiMAX Phy object
   *
   * Creates a physical layer
   */
  Ptr<WimaxPhy> CreatePhy (PhyType phyType, char * SNRTraceFilePath, bool activateLoss);
  /**
   * \param node Node to be installed.
   * \param deviceType Device type to create.
   * \param phyType PHY type to create.
   * \param channel A channel to use.
   * \param schedulerType The scheduling mechanism.
   *
   * For each of the input nodes, a new WiMAX net device (either
   * ns3::SubscriberStationNetDevice or ns3::BaseStationNetDevice
   * depending on the type parameter) is attached to the shared input channel.
   */
  Ptr<WimaxNetDevice> Install (Ptr<Node> node,
                               NetDeviceType deviceType,
                               PhyType phyType,
                               Ptr<WimaxChannel> channel,
                               SchedulerType schedulerType);

  /**
   * \param direction the Tirection of the service flow: UP or DOWN.
   * \param schedulinType The service scheduling type to be used: UGS, RTPS, NRTPS, BE
   * \param csParam The convergence sub-layer parameters to be used
   *
   * Creates a transport service flow.
   */
  ServiceFlow CreateServiceFlow (ServiceFlow::Direction direction,
                                 ServiceFlow::SchedulingType schedulinType,
                                 IpcsClassifierRecord classifier);

private:
  static void AsciiEnqueueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  static void AsciiDequeueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  static void AsciiDropEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  static void AsciiRxEvent (std::ostream *os, std::string path, Ptr<const Packet> packet, const Mac48Address &source);
  static void AsciiTxEvent (std::ostream *os, std::string path, Ptr<const Packet> packet, const Mac48Address &dest);

  Ptr<WimaxChannel> m_channel;
};

} // namespace ns3

#endif /* WIMAX_HELPER_H */
