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
 
 NetDataStruct::NetData netData = NetinfoReader::Read ((netInfoFile));
 Print::Netinfo (netData);
 NetworkConfig::NetworkData networkData = Report2ConfigData::NetData2NetworkData (netData);
 Print::NetworkData (networkData);

 NS_LOG_INFO ("Creating the network...");
 CreateNetwork createNetwork;
 NodeContainer nodes = createNetwork.Create (networkData);
 Print::NodeList (nodes);

 /*
  * Applications
  */
  NetTest netTest;
      
  //netTest.Echo ("Josjo 1", "Josjo 2", 1);
  
  netTest.EnablePcap("Josjo 1", 1);
  netTest.EnablePcap("Josjo 2", 1);

  AppState appState1 (AC_BE);
  netTest.ApplicationSetup ("Josjo 1", "Josjo 2", 9 , 3, 6, "8964kbps", 1024, &appState1);

  AppState appState2 (AC_BE);
  netTest.ApplicationSetup ("Josjo 1", "Josjo 2", 4, 5, "8964kbps", 1024, &appState2);


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
