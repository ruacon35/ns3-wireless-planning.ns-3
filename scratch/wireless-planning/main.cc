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

#include "net-test.h"
#include "create-network.h"
#include "network-config.h" 
#include "print.h"
#include "netinfo-reader.h"
#include "report-2-config-data.h"



#include <map>
#include <string>
#include <vector>

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("main");

/**
 * @brief network main Script (read from netinfo text file)
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

string netInfoFile = "cusco-ne-netinfo.txt";
 NS_LOG_INFO ("Getting data in order to create and configure the network...");
 CommandLine cmd;
 cmd.AddValue ("NetInfoFile", "Network Information File", netInfoFile);
 cmd.Parse (argc, argv);
 
 
 NetworkConfig::NetworkData networkData = SetNetworkConfiguration (netInfoFile);
 Print::NetworkData (networkData);

 NS_LOG_INFO ("Creating the network...");
 CreateNetwork createNetwork;
 NodeContainer nodes = createNetwork.Create (networkData);
 Print::NodeList (nodes); //must enable ns_log print

 /*
  * Applications
  */
  NetTest netTest;
    
  //netTest.SetWimaxUlServiceFlow ("Ccatcca", 1, ServiceFlow::SF_TYPE_RTPS);
  netTest.SetWimaxDlServiceFlow ("Kcauri", 1, ServiceFlow::SF_TYPE_RTPS);
  netTest.Echo ("Kcauri", "Urcos", 1);
  
  netTest.EnablePcap("Josjojauarina 2", 1);
  netTest.EnablePcap("Josjojauarina 2", 2);
  netTest.EnablePcap("Kcauri", 1); 
  //netTest.EnablePcap("Huiracochan", 1);

 // OnOff
 /// Short simulations
 /*AppState appState1 (AC_BE);
 netTest.ApplicationSetup ("Urcos", 9 , "Kcauri", 4, 10, "64kbps", 200, &appState1);
 AppState appState2 (AC_VO);
 netTest.ApplicationSetup ("Urpay", 9, "Ccatcca", 6, 8, "64kbps", 200, &appState2);*/

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
 * @return NetworkData All the needed information in order to build a network.
 * 
 * @see CreateNetwork::Create
 */
NetworkConfig::NetworkData
SetNetworkConfiguration (string netInfoFile)
{
 NetworkConfig::NetworkData networkData;
 
 ifstream file (netInfoFile.c_str());
 NS_LOG_INFO ("Reading simplified netinfo: " << netInfoFile);
 NetDataStruct::NetData netData = NetinfoReader::Read (file);
 Print::Netinfo (netData);

 Report2ConfigData r2c;
 networkData = r2c.NetData2NetworkData (netData);

 return networkData;
}
