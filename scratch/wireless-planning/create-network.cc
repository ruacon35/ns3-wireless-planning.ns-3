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
#include <iostream>
#include <errno.h>

#include "ns3/log.h"
#include "ns3/wifi-mac.h"
#include "ns3/qap-wifi-mac.h"
#include "ns3/qsta-wifi-mac.h"
#include "ns3/string.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/object.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/names.h"
#include "ns3/net-device.h"
#include "ns3/wifi-net-device.h"
#include "ns3/boolean.h"
#include "ns3/double.h"

#include "create-network.h"



NS_LOG_COMPONENT_DEFINE ("create-network");

namespace ns3 {

  CreateNetwork::CreateNetwork ()
  {
  }

  CreateNetwork::~CreateNetwork ()
  {
  }

  TypeId
  CreateNetwork::GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::CreateNetwork")
            .SetParent<Object > ()
            ;
    return tid;
  }

  void
  CreateNetwork::SetWifiPhy (void)
  {
    for (uint32_t i = 0; i < m_vWifiChData.size (); i++)
    {
      YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
      YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();

      channel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
      channel.AddPropagationLoss ("ns3::FixedRssLossModel", "Rss", DoubleValue (0));

      phy.SetChannel (channel.Create ());
      m_vectorWifiPhy.push_back (phy);
    }
  }

  void
  CreateNetwork::SetIpAddresser (void)
  {
    std::ostringstream network;
    for (uint32_t i = 0; i < m_vWifiChData.size (); i++)
    {
      Ipv4AddressHelper addressHelper;
      network.str ("");
      network << "10.1." << i + 1 << ".0";
      std::string net = network.str ();
      NS_LOG_DEBUG ("Wifi Network: " << network.str ());
      addressHelper.SetBase (net.c_str (), "255.255.255.0");
      m_vectorWifiAddresser.push_back (addressHelper);
    }
    for (uint32_t i = 0; i < m_vWimaxChData.size (); i++)
    {
      Ipv4AddressHelper addressHelper;
      network.str ("");
      network << "10.2." << i + 1 << ".0";
      NS_LOG_DEBUG ("Wimax Network: " << network.str ());
      std::string net = network.str ();
      addressHelper.SetBase (net.c_str (), "255.255.255.0");
      m_vectorWimaxAddresser.push_back (addressHelper);
    }
  }

  NqosWifiMacHelper
  CreateNetwork::SetWifiNqosMac (NetworkConfig::DeviceData deviceData)
  {
    NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();

    //ssid name
    std::ostringstream oss;
    oss.str ("");
    oss << "ssid-" << deviceData.chId;
    Ssid ssid = Ssid (oss.str ());

    switch (deviceData.wifiMacType)
    {
      case NetworkConfig::AP :
                mac.SetType ("ns3::NqapWifiMac",
                "Ssid", SsidValue (ssid),
                "BeaconGeneration", BooleanValue (true),
                "BeaconInterval", TimeValue (Seconds (10)));
        NS_LOG_DEBUG ("  Access Point");
        break;
      case NetworkConfig::STA :
                mac.SetType ("ns3::NqstaWifiMac",
                "Ssid", SsidValue (ssid),
                "ActiveProbing", BooleanValue (false));
        NS_LOG_DEBUG ("  Station");
        break;
      case NetworkConfig::ADHOC :
                mac.SetType ("ns3::AdhocWifiMac");
        NS_LOG_DEBUG ("  Ad-Hoc");
        break;
      default:
        NS_LOG_ERROR ("  No correct MAC type selected");
        exit (-1);
    }
    return mac;
  }

  QosWifiMacHelper
  CreateNetwork::SetWifiQosMac (NetworkConfig::DeviceData deviceData)
  {
    QosWifiMacHelper macHelper = QosWifiMacHelper::Default (); // Qsta

    //ssid name
    std::ostringstream oss;
    oss.str ("");
    oss << "ssid-" << deviceData.chId;
    Ssid ssid = Ssid (oss.str ());

    switch (deviceData.wifiMacType)
    {
      case NetworkConfig::QAP :
      {
        macHelper.SetType ("ns3::QapWifiMac",
                "Ssid", SsidValue (ssid),
                "BeaconGeneration", BooleanValue (true),
                "BeaconInterval", TimeValue (Seconds (2.5)));
        break;
      }
      case NetworkConfig::QSTA :
      {
        macHelper.SetType ("ns3::QstaWifiMac",
                "Ssid", SsidValue (ssid),
                "ActiveProbing", BooleanValue (false));
        NS_LOG_DEBUG ("  QoS Station");
        break;
      }
      case NetworkConfig::QADHOC :
                macHelper.SetType ("ns3::QadhocWifiMac");
        NS_LOG_DEBUG ("  QoS Ad-Hoc");
        break;
      default:
        NS_LOG_ERROR ("  No correct MAC type selected");
        exit (-1);
    }
    return macHelper;
  }

  void
  CreateNetwork::SetIpAddress (NetDevice &device, Ipv4Address address)
  {
    NS_LOG_FUNCTION_NOARGS ();

    Ptr<Node> node = device.GetNode ();
    NS_ASSERT_MSG (node, "Ipv4AddressHelper::Allocate(): Bad node");

    Ptr<Ipv4> ipv4 = node->GetObject<Ipv4 > ();
    NS_ASSERT_MSG (ipv4, "Ipv4AddressHelper::Allocate(): Bad ipv4");

    int32_t interface = ipv4->GetInterfaceForDevice (&device);
    if (interface == -1)
    {

      interface = ipv4->AddInterface (&device);
    }
    NS_ASSERT_MSG (interface >= 0, "Ipv4AddressHelper::Allocate(): "
            "Interface index not found");

    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (address, "255.255.255.0");
    NS_LOG_DEBUG ("Ipv4Addr: " << ipv4Addr);
    ipv4->AddAddress (interface, ipv4Addr);
    ipv4->SetMetric (interface, 1);
    ipv4->SetUp (interface);
  }

  void
  CreateNetwork::NetworkBuilding (void)
  {
    NetDeviceContainer devices;
    for (uint32_t i = 0; i < m_nNodes; i++)// loop  throw NODES
    {
      /*
       * Node installation
       */
      NetworkConfig::NodeData nodeData = m_vectorNodeData[i];
      NS_LOG_INFO ("Node installation: " << nodeData.name << " - " << i << " of " << m_nNodes);
      Names::Add (nodeData.name, m_nodes.Get (i));

      /*
       * Mobility
       */
      NS_LOG_INFO ("Mobility");
      MobilityHelper mobility;
      Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator > ();
      positionAlloc->Add (nodeData.position);
      mobility.SetPositionAllocator (positionAlloc);
      mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

      //          mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel",
      //                                     "Position", VectorValue (nodeData.position)); /// It doesn't work!!!
      mobility.Install (m_nodes.Get (i));

      /*
       * Device installation
       */
      NS_LOG_INFO ("  Device Installation");
      for (uint32_t j = 0; j < nodeData.vectorDeviceData.size (); j++)// loop  throw DEVICES
      {
        NS_LOG_INFO ("  Device " << j);
        NetworkConfig::DeviceData deviceData = nodeData.vectorDeviceData.at (j);
        NetDeviceContainer device;
        switch (deviceData.commStandard)
        {
          case NetworkConfig::WIFI :
          {
            device = WifiDeviceInstallation (deviceData, m_nodes.Get (i));
            break;
          }
          case NetworkConfig::WIMAX :
          {
            NS_LOG_DEBUG ("wimax");
            device = WimaxDeviceInstallation (deviceData, m_nodes.Get (i));
            break;
          }
          default:
            NS_ASSERT_MSG (0, "The communication standard selected is not correct.");
        }
        //devices.Add (device);
      }
    }
    /*
     * Routing
     */
    //Turn on global static routing
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  }

  NetDeviceContainer
  CreateNetwork::WifiDeviceInstallation (NetworkConfig::DeviceData deviceData, Ptr<Node> node)
  {
    NS_LOG_DEBUG ("Ch ID: " << deviceData.chId << " MAC type: " << deviceData.wifiMacType);

    uint16_t index = deviceData.chId - 1;

    WifiHelper wifi = WifiHelper::Default ();

    YansWifiPhyHelper phy;
    phy = m_vectorWifiPhy.at (index);

    Ipv4AddressHelper address;
    address = m_vectorWifiAddresser.at (index);


    std::string mode = m_vWifiChData.at (index).wifiMode;
    wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
            "DataMode", StringValue (mode),
            "RtsCtsThreshold", StringValue ("2200")); // disable

    NetDeviceContainer device;

    if (deviceData.wifiMacType <= NetworkConfig::ADHOC)
    {
      NqosWifiMacHelper mac = SetWifiNqosMac (deviceData);
      device = wifi.Install (phy, mac, node);
    } else
    {
      QosWifiMacHelper mac = SetWifiQosMac (deviceData);
      device = wifi.Install (phy, mac, node);
    }

    WifiLongDistances (device, deviceData.max_distance);

    if (deviceData.ipAddress.IsEqual (""))
    {
      address.Assign (device);
    } else
    {

      SetIpAddress (*device.Get (0), Ipv4Address (deviceData.ipAddress));
    }
    m_vectorWifiAddresser.at (index) = address; ///< Need to update the object!?, const reference vs reference

    return device;
  }

  NetDeviceContainer
  CreateNetwork::WimaxDeviceInstallation (NetworkConfig::DeviceData deviceData, Ptr<Node> node)
  {
    NS_LOG_DEBUG ("Ch ID: " << deviceData.chId << " Wimax device type: " << deviceData.wimaxDeviceType);

    uint16_t index = deviceData.chId - 1;

    WimaxHelper wimax = m_vWimaxChData.at (index).wimax;
    Ipv4AddressHelper address;
    address = m_vectorWimaxAddresser.at (index);
    NetDeviceContainer device;

    WimaxHelper::SchedulerType scheduler = WimaxHelper::SCHED_TYPE_RTPS;
    
    device = wimax.Install (node,
                            deviceData.wimaxDeviceType,
                            WimaxHelper::SIMPLE_PHY_TYPE_OFDM,
                            scheduler);

    if (deviceData.wimaxDeviceType == WimaxHelper::DEVICE_TYPE_SUBSCRIBER_STATION)
      {
        Ptr<NetDevice> device2 = device.Get(0);
        Ptr<SubscriberStationNetDevice> wimaxDevice;
        wimaxDevice = device2->GetObject<SubscriberStationNetDevice> ();
        wimaxDevice->SetModulationType (deviceData.modulationType);
        
        IpcsClassifierRecord FakeDlClassifierUgs (Ipv4Address ("0.0.0.0"),
                                              Ipv4Mask ("0.0.0.0"),
                                              Ipv4Address ("0.0.0.0"),
                                              Ipv4Mask ("0.0.0.0"),
                                              0,
                                              0,
                                              0,
                                              0,
                                              16,
                                              1);
        ServiceFlow FakeDlServiceFlowUgs = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_DOWN,
                                                                ServiceFlow::SF_TYPE_BE,
                                                                FakeDlClassifierUgs);
        wimaxDevice->AddServiceFlow (FakeDlServiceFlowUgs);        
      }

    Ipv4InterfaceContainer interfaces;
    if (deviceData.ipAddress.IsEqual (""))
    {
      interfaces = address.Assign (device);      
    } 
    else
    {
      SetIpAddress (*device.Get (1), Ipv4Address (deviceData.ipAddress));
    }
    m_vectorWimaxAddresser.at (index) = address; ///< Need to update the object!?, const reference vs reference
    // Wimax helper must be re-used, otherwise it will segfault at WimaxNetDevice::GetChannel
    m_vWimaxChData.at (index).wimax = wimax; 

    return device;
  }

  void
  CreateNetwork::WifiLongDistances (NetDeviceContainer device, double distance)
  {

    double c0 = 3e8; // speed of light [m/sec]
    double maxPropagationDelay = distance / c0; // sec

    Ptr< WifiNetDevice > wifiNetDev;
    Ptr< NetDevice > netDev;
    netDev = device.Get (0);

    wifiNetDev = netDev->GetObject<WifiNetDevice > ();

    Ptr< WifiMac > mac;
    mac = wifiNetDev->GetMac ();

    NS_LOG_DEBUG ("Distance: " << distance << " m");
    NS_LOG_DEBUG ("Max propagation delay: " << Seconds (maxPropagationDelay));
    mac->SetMaxPropagationDelay (Seconds (maxPropagationDelay));

    //ACK Timeout
    NS_LOG_DEBUG ("ACKTimeout: " << mac->GetAckTimeout ().GetMicroSeconds () << "us");
    Time ackTimeout = Seconds (mac->GetEifsNoDifs ().GetSeconds ()
            + mac->GetSlot ().GetSeconds ()
            + mac->GetMaxPropagationDelay ().GetSeconds () * 2);
    NS_LOG_DEBUG ("New ACKTimeout: " << ackTimeout.GetMicroSeconds () << "us");
    ackTimeout = Time (NanoSeconds (int(ackTimeout.GetNanoSeconds ()))); // reconvertion
    NS_LOG_DEBUG ("New ACKTimeout: " << ackTimeout.GetSeconds () << "s");

    mac->SetAckTimeout (ackTimeout);
    // CTS Timeout
    NS_LOG_DEBUG ("CTSTimeout: " << mac->GetCtsTimeout ().GetMicroSeconds () << "us");
    mac->SetCtsTimeout (ackTimeout);
    NS_LOG_DEBUG ("New CTSTimeout: " << mac->GetCtsTimeout ().GetMicroSeconds () << "us");

    // Slot Time
    NS_LOG_DEBUG ("Slot Time: " << mac->GetSlot ().GetMicroSeconds () << "us");
    Time slotTime = NanoSeconds (mac->GetSlot ().GetNanoSeconds ()
            + mac->GetMaxPropagationDelay ().GetNanoSeconds () * 2);
    NS_LOG_DEBUG ("2x Max prop delay: " << Seconds (mac->GetMaxPropagationDelay ().GetSeconds () * 2));

    mac->SetSlot (slotTime);
    NS_LOG_DEBUG ("New Slot Time: " << mac->GetSlot ().GetMicroSeconds () << "us");
  }

  NodeContainer
  CreateNetwork::Create (NetworkConfig::NetworkData networkData)
  {
    // Reading network information data
    m_networkData = networkData;
    m_vectorNodeData = m_networkData.vectorNodeData;
    m_vectorChannelData = m_networkData.vectorChannelData;
    m_vWifiChData = m_vectorChannelData.vWifiChData;
    m_vWimaxChData = m_vectorChannelData.vWimaxChData;
    m_nNodes = m_vectorNodeData.size (); //number of nodes

    m_nodes.Create (m_nNodes);

    NS_LOG_INFO ("Creating " << m_nNodes << " nodes.");

    // Install network stacks on the nodes
    InternetStackHelper internet;
    internet.Install (m_nodes);

    NS_LOG_INFO ("Create channels.");
    SetWifiPhy ();

    SetIpAddresser ();

    //Node Configuration
    NetworkBuilding ();

    /*
     * Traces: pcap
     */
    NS_LOG_INFO ("Enabling pcap traces");
    //m_vectorWifiPhy.at (0).EnablePcapAll ("cusco"); //We only need a YansWifiPhyHelper
    ///< enable pcap for wimax

    // mobility traces: no movements => empty file
    //  std::ofstream os;
    //  os.open ("net.mob");
    //  MobilityHelper::EnableAsciiAll (os);
    return m_nodes; // return all nodes
  }

} // namespace ns3
