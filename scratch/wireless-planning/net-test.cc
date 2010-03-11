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
    
    WimaxHelper wimax;

    Ptr<SubscriberStationNetDevice> clientDevice;
    Ptr<SubscriberStationNetDevice> serverDevice;

    clientDevice = clientNode->GetDevice (1)->GetObject<SubscriberStationNetDevice> ();
    if (clientDevice != NULL)
      {      
        IpcsClassifierRecord DlClassifierUgs (Ipv4Address ("0.0.0.0"),
                                              Ipv4Mask ("0.0.0.0"),
                                              clientIpAddr,
                                              Ipv4Mask ("255.255.255.255"),
                                              0,
                                              65000,
                                              1,
                                              65000,
                                              17,
                                              1);
        ServiceFlow DlServiceFlowUgs = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_DOWN,
                                                                ServiceFlow::SF_TYPE_RTPS,
                                                                DlClassifierUgs);
        IpcsClassifierRecord UlClassifierUgs2 (clientIpAddr,
                                              Ipv4Mask ("255.255.255.255"),
                                              Ipv4Address ("0.0.0.0"),
                                              Ipv4Mask ("0.0.0.0"),
                                              0,
                                              65000,
                                              1,
                                              65000,
                                              17,
                                              1);
                                                                
        ServiceFlow UlServiceFlowUgs2 = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_UP,
                                                                ServiceFlow::SF_TYPE_RTPS,
                                                                UlClassifierUgs2);
        clientDevice->AddServiceFlow (DlServiceFlowUgs);
        clientDevice->AddServiceFlow (UlServiceFlowUgs2);
      }

    serverDevice = serverNode->GetDevice (1)->GetObject<SubscriberStationNetDevice> ();
    if (serverDevice != NULL)
      {
        IpcsClassifierRecord UlClassifierUgs (serverIpAddr,
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
                                                                ServiceFlow::SF_TYPE_RTPS,
                                                                UlClassifierUgs);
        IpcsClassifierRecord DlClassifierUgs2 (Ipv4Address ("0.0.0.0"),
                                              Ipv4Mask ("0.0.0.0"),
                                              serverIpAddr,
                                              Ipv4Mask ("255.255.255.255"),
                                              0,
                                              65000,
                                              1,
                                              65001,
                                              17,
                                              1);
        ServiceFlow DlServiceFlowUgs2 = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_DOWN,
                                                                ServiceFlow::SF_TYPE_RTPS,
                                                                DlClassifierUgs2);
        serverDevice->AddServiceFlow (UlServiceFlowUgs);
        serverDevice->AddServiceFlow (DlServiceFlowUgs2);
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


