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
 * You
 *  should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 */

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/common-module.h" 
#include "ns3/contrib-module.h" 
#include "ns3/ehas-module.h"

#include <map>
#include <string>
#include <vector>

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("main");

/**
 * @brief network main Script (reading from report.txt)
 */

NetworkConfig::NetworkData SetNetworkConfiguration (string netInfoFile);

int
main (int argc, char *argv[])
{

Time eos = Seconds (15);// End Of Simulation in seconds, necessary to finish some methods.
//g_eos??

 /*
  * Network Creation
  */
 NS_LOG_INFO ("Getting data in order to create and configure the network...");
 CommandLine cmd;
 cmd.Parse (argc, argv);
 
 NetworkConfig::NetworkData networkData = SetNetworkConfiguration ("cusco-ne-netinfo.txt");
 Print::NetworkData (networkData);

 NS_LOG_INFO ("Creating the network...");
 CreateNetwork createNetwork;
 NodeContainer nodes = createNetwork.Create (networkData);

 Print::NodeList (nodes); //must enable ns_log print

 /*
  * Applications
  */
 NetTest netTest;
 // Echos
 //netTest.Echo ("URCOS", "KCAURI", 1);
 //netTest.Echo ("URPAY", "HUIRACOCHAN", 2);
 //netTest.Echo ("ACOMAYO", "RED SUR CUSCO", 3);

 
 // OnOff
 /// Short simulations
 NS_LOG_DEBUG ("AC_VO: " << AC_VO);
 NS_LOG_DEBUG ("AC_VI: " << AC_VI);


netTest.ApplicationSetup ("Urcos", 9 , "Kcauri", 4, 10, "15Mbps", 200, AC_VO);
netTest.ApplicationSetup ("Urpay", 9, "Ccatcca", 6, 8, "15Mbps", 200, AC_VO);

//netTest.ApplicationSetup ("URCOS", 9 , "KCAURI", 4, 10, "15Mbps", 200, AC_VO);
//netTest.ApplicationSetup ("URPAY", 9, "CCATCCA", 6, 8, "15Mbps", 200, AC_VO);

// netTest.ApplicationSetup ("URCOS", 9, "URPAY", 4, 10, "15Mbps", 200, AC_VO);
// netTest.ApplicationSetup ("CCATCCA", 9, "KCAURI", 6, 8, "15Mbps", 200, AC_VO);


 /*
  * Setup all the plot system: throughput measurement, gnuplot issues...
  */
 NetMeasure netMeasure (eos, Seconds (0.1));
 netMeasure.SetupPlot ();

 netMeasure.SetFlowMonitor (nodes);
 netMeasure.GetFlowStats ();

 Simulator::Stop (eos);
 NS_LOG_INFO ("Starting simulation...");

 Simulator::Run ();
 Simulator::Destroy ();

 /*
  * After simulation
  */


 NS_LOG_INFO ("Done.");

 return 0;
}

/**
 * @brief Inside this method all the information of the network is hand written 
 * 
 * 
 * @return NetworkData All the needed information in order to build a network.
 * 
 * @see CreateNetwork::Create
 */
NetworkConfig::NetworkData
SetNetworkConfiguration (string netInfoFile)
{
 NetworkConfig::NetworkData networkData;

 ifstream file (netInfoFile.c_str());
 NS_LOG_INFO ("Reading simplified netinfo: " << file);
 RmReportReader rrr;
 NetDataStruct::NetData netData = rrr.ReadRmReport (file);
 Print::RmReportInfo (netData);

 Report2ConfigData r2c;
 networkData = r2c.NetData2NetworkData (netData);
 Print::NetworkData (networkData);

 return networkData;
}
