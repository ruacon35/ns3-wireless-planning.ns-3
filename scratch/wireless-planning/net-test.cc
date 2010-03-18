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
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/packet-socket-address.h"
#include "ns3/on-off-helper.h"
#include "ns3/random-variable.h"
#include "ns3/udp-echo-helper.h"
#include "ns3/data-rate.h"
#include "ns3/uinteger.h"
#include "ns3/object.h"
#include "ns3/ipv4.h"
#include "ns3/names.h"
#include "ns3/qos-utils.h"
#include "ns3/qos-tag.h"
#include "ns3/config.h"
#include "ns3/wimax-module.h"
#include "ns3/wifi-module.h"
#include "ns3/ipcs-classifier-record.h"
#include "ns3/service-flow.h"
#include "ns3/helper-module.h"

#include "net-measure.h"
#include "net-test.h"
#include "util.h"

NS_LOG_COMPONENT_DEFINE ("net-test");

namespace ns3 {

  AppState::AppState (enum AccessClass ac)
  : m_ac (ac)
  {
    m_tid = Util::QosUtilsMapAcToTid (ac);
  }

  void
  AppState::OnOffTxPacketCallback (std::string context, Ptr<const Packet> packet)
  {
    NS_LOG_FUNCTION ("Packet tagged" << context << " AC: " << m_ac << " Tid:" << (uint16_t) m_tid);
    QosTag qosTag;
    qosTag.SetTid (m_tid);
    packet->AddPacketTag (qosTag);
  }

  NetTest::NetTest ()
  {
  }

  NetTest::~NetTest ()
  {
  }

  TypeId
  NetTest::GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::NetTest")
            .SetParent<Object > ()
            ;
    return tid;
  }

  void
  NetTest::ApplicationSetup (std::string server, uint8_t port, std::string client,
          double start, double stop, std::string rate,
          uint32_t packetSize, AppState *appState)
  {
    // Obtain server node pointer from server name
    Ptr<Node> serverNode = GetNodeFromName (server);
    Ptr<Node> clientNode = GetNodeFromName (client);

    Ipv4Address serverIpAddr = Util::GetIpAddrFromName (server);

    // Create a packet sink to receive these packets
    ///Address sinkLocalAddress(InetSocketAddress (ipAddr, port));
    Address sinkLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
    PacketSinkHelper sinkHelper ("ns3::UdpSocketFactory", sinkLocalAddress);

    ApplicationContainer sinkApp = sinkHelper.Install (server);
    sinkApp.Start (Seconds (start));
    sinkApp.Stop (Seconds (stop));

    /*
     * On Off Application
     */
    NS_LOG_INFO ("Setup application: OnOff");
    NS_LOG_DEBUG (" Server: " << server << " Client: " << client << " Start: " << start << " Stop: " << stop << " Rate: " << rate << " Packet size: " << packetSize);

    // Create the OnOff applications to send UDP to the server
    OnOffHelper onOff ("ns3::UdpSocketFactory", Address ());
    onOff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
    onOff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
    onOff.SetAttribute ("DataRate", DataRateValue (DataRate (rate)));
    onOff.SetAttribute ("PacketSize", UintegerValue (packetSize));

    ApplicationContainer clientApps;

    AddressValue remoteAddress (InetSocketAddress (serverIpAddr, port));

    onOff.SetAttribute ("Remote", remoteAddress);
    clientApps.Add (onOff.Install (client));

    std::ostringstream oss;
    oss << "/NodeList/" << clientNode->GetId () <<
            "/ApplicationList/0/$ns3::OnOffApplication/Tx";
    Config::Connect (oss.str (), MakeCallback (&AppState::OnOffTxPacketCallback, appState));
    NS_LOG_DEBUG ("set callback: " << oss.str () << " AC: " << appState->m_ac << " Tid:" << (uint16_t) appState->m_tid);

    clientApps.Start (Seconds (start));
    clientApps.Stop (Seconds (stop));

    uint32_t nApp = serverNode->GetNApplications ();
    NS_LOG_DEBUG ("nApp of server node " << serverNode->GetId () << " : " << nApp);

    nApp = clientNode->GetNApplications ();
    NS_LOG_DEBUG ("nApp of client node " << clientNode->GetId () << " : " << nApp);
  }

  void
  NetTest::Echo (std::string server, std::string client, double start)
  {
    NetTest::Echo (server, 9, client,  start);
  }

  void
  NetTest::Echo (std::string server, uint8_t port, std::string client, double start)
  {
    NS_LOG_INFO ("UDP echo server/client application: " << server << " / " << client);
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    
    Ptr<Node> serverNode = GetNodeFromName (server);
    Ptr<Node> clientNode = GetNodeFromName (client);

    UdpEchoServerHelper echoServer (port); //Port
    ApplicationContainer serverAppCont = echoServer.Install (server); //Node
    serverAppCont.Start (Seconds (start));
    serverAppCont.Stop (Seconds (start + 1)); // does it really stop?

    //Ptr<Application> serverApp = serverAppCont.Get (0);
    //Simulator::Schedule (start+1,serverApp->Stop(Seconds(start+1)),this);

    Ipv4Address serverIpAddr = Util::GetIpAddrFromName (server);
    Ipv4Address clientIpAddr = Util::GetIpAddrFromName (client);

    NS_LOG_INFO ("server: " << server << " address: " << serverIpAddr 
      << " Client: " << client << " address: " << clientIpAddr);

    UdpEchoClientHelper echoClient (serverIpAddr, port); // server IP and Port
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
    ApplicationContainer clientAppCont = echoClient.Install (client); 
    clientAppCont.Start (Seconds (start));
    clientAppCont.Stop (Seconds (start + 1));

    //Ptr< Application > 	GetApplication (uint32_t index) const
    uint32_t nApp = serverNode->GetNApplications ();
    NS_LOG_DEBUG ("nApp of server node " << serverNode->GetId () << " : " << "nApp " << nApp);

    nApp = clientNode->GetNApplications ();
    NS_LOG_DEBUG ("nApp of client node " << clientNode->GetId () << " : " << "nApp " << nApp);
    
    SetWimaxServiceFlow (client, server, 1, ServiceFlow::SF_TYPE_RTPS, 17, ServiceFlow::SF_DIRECTION_DOWN);        
    SetWimaxServiceFlow (server, client, 1, ServiceFlow::SF_TYPE_RTPS, 17, ServiceFlow::SF_DIRECTION_DOWN);
  }

  void
  NetTest::SetWimaxServiceFlow (std::string sourceNodeName, std::string destNodeName, 
    uint8_t device_index, ServiceFlow::SchedulingType schedulinType, 
    uint8_t protocol, ServiceFlow::Direction direction)
  {
    WimaxHelper wimax;    
    Ipv4Address sourceIpv4Address;
    Ipv4Mask sourceIpv4Mask;
    
    if (!sourceNodeName.empty()) 
      {    
        sourceIpv4Address = Util::GetIpAddrFromName (sourceNodeName);
        sourceIpv4Mask = Ipv4Mask ("255.255.255.255");
      }
    else
      {
        sourceIpv4Address = Ipv4Address ("0.0.0.0");
        sourceIpv4Mask = Ipv4Mask ("0.0.0.0");
      }

    Ipv4Address destIpv4Address = Util::GetIpAddrFromName (destNodeName);
    Ptr<Node> destNode = Names::Find<Node>(destNodeName);
    Ptr<NetDevice> destNetDevice = destNode->GetDevice(device_index);
    Ptr<SubscriberStationNetDevice> SSDevice;
    SSDevice = destNetDevice->GetObject<SubscriberStationNetDevice> ();
    //NS_ASSERT_MSG(SSDevice, "destination node must be a Subscriber Station");
    
    if (SSDevice != NULL)
      {      
        IpcsClassifierRecord ClassifierUgs (sourceIpv4Address,
                                              sourceIpv4Mask,
                                              destIpv4Address,
                                              Ipv4Mask ("255.255.255.255"),
                                              0,
                                              65000,
                                              1,
                                              65000,
                                              protocol,
                                              1);
        ServiceFlow ServiceFlowUgs = wimax.CreateServiceFlow (direction,
                                                                schedulinType,
                                                                ClassifierUgs);
        SSDevice->AddServiceFlow (ServiceFlowUgs);
      }  
  } 

  void
  NetTest::SetWimaxUlServiceFlow (std::string nodeName, uint8_t device_index, ServiceFlow::SchedulingType schedulinType)
  {
    WimaxHelper wimax;
    Ipv4Address ipv4Address = Util::GetIpAddrFromName (nodeName);
    Ptr<Node> node = Names::Find<Node>(nodeName);    
    Ptr<NetDevice> netDevice = node->GetDevice(device_index);
    Ptr<SubscriberStationNetDevice> SSDevice;
    SSDevice = netDevice->GetObject<SubscriberStationNetDevice> ();    
    
    if (SSDevice != NULL)
      {      
        IpcsClassifierRecord DlClassifierUgs (ipv4Address,
                                              Ipv4Mask ("255.255.255.255"),
                                              Ipv4Address ("0.0.0.0"),
                                              Ipv4Mask ("0.0.0.0"),
                                              0,
                                              65000,
                                              1,
                                              65000,
                                              17,
                                              1);
        ServiceFlow UlServiceFlowUgs = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_UP,
                                                                schedulinType,
                                                                DlClassifierUgs);
        SSDevice->AddServiceFlow (UlServiceFlowUgs);
      }
  
  } 
  
  void
  NetTest::EnablePcap(string nodeName, uint8_t deviceNum) {
    Ptr<Node> node = Names::Find<Node>(nodeName);
    int position = nodeName.find(" ");
    while (position != -1) 
     {      
        nodeName.replace(position, 1, "_");
        position = nodeName.find(" ", position + 1);
     } 
   
    Ptr<NetDevice> netDevice = node->GetDevice(deviceNum);
    Ptr<WifiNetDevice> wifiDevice;
    wifiDevice  = netDevice->GetObject<WifiNetDevice> ();
    if (wifiDevice != NULL) 
      {
        YansWifiPhyHelper wifi;
        wifi.EnablePcap(nodeName, node->GetId(), netDevice->GetIfIndex ());
      }
    else 
      {
        WimaxHelper wimax;
        wimax.EnablePcap(nodeName, node->GetId(), netDevice->GetIfIndex ());
    }
  }

  Ptr<Node>
  NetTest::GetNodeFromName (string name)
  {
    Ptr<Node> node = Names::Find<Node > (name);
    NS_ASSERT_MSG (node != 0,
            "NetTest::GetNodeFromName "
            " '" << name << "' node not found");
    return node;
  }

} // namespace ns3
