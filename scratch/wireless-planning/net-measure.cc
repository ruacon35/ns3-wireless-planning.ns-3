/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-- */
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
#include "ns3/flow-monitor.h"

#include <string>
#include <sstream>
#include <map>

#include "net-measure.h"

NS_LOG_COMPONENT_DEFINE ("net-measure");

namespace ns3 {

  NetMeasure::NetMeasure ()
  : m_eos (Seconds (100)),
  m_interval (Seconds (1))
  {
    NS_LOG_INFO ("EOS " << m_eos << " Interval " << m_interval);
  }

  NetMeasure::NetMeasure (Time eos, Time interval)
  : m_eos (eos),
  m_interval (interval)
  {
    NS_LOG_INFO ("EOS " << m_eos << " Interval " << m_interval);
  }

  NetMeasure::~NetMeasure ()
  {
  }

  TypeId
  NetMeasure::GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::NetMeasure")
            .SetParent<Object > ()
            ;
    return tid;
  }

  void
  NetMeasure::SetupPlot ()
  {
    NS_LOG_INFO ("Setup plot");

    PlotData plotData = {"Throughput", "Time [sec]", "Throughput [Mbps]", TIMEDOMAIN};
    m_MeasPlotData["Throughput"] = plotData;

    plotData.title = "Delay Histogram";
    plotData.x = "Delay [sec]";
    plotData.y = "Frequency [%]";
    plotData.plotType = HISTOGRAM;
    m_MeasPlotData["Delay"] = plotData;


    for (MeasPlotData::const_iterator it = m_MeasPlotData.begin (); it != m_MeasPlotData.end (); ++it)
    {
      std::vector< Gnuplot2dDataset > vDataSet;
      m_mMeasurementDataset [it->first] = vDataSet;
    }
    Simulator::Schedule (m_eos, &NetMeasure::EndPlot, this); // call to EndPlot at End of Simulation
  }

  void
  NetMeasure::EndPlot ()
  {
    NS_LOG_INFO ("End Plot");

    for (MeasurementDataset::const_iterator it = m_mMeasurementDataset.begin (); it != m_mMeasurementDataset.end (); ++it)
    {
      std::string measurement = it->first;
      std::vector < Gnuplot2dDataset > vDataSetFlow = it->second;
      
      PlotData plotData = m_MeasPlotData[measurement];
      // svg or png
      Gnuplot gnuplot ("net-measure_"+ measurement + ".png", plotData.title);
      gnuplot.SetTerminal ("png"); // non detected terminal
      gnuplot.SetLegend (plotData.x, plotData.y);

      NS_LOG_INFO ("Plot name: " << measurement);
      for (uint32_t j = 0; j < vDataSetFlow.size (); j++)
      {
        NS_LOG_DEBUG ("it:j " << measurement << ":" << j);
        gnuplot.AddDataset (vDataSetFlow.at (j));
      }

      std::string file = "net-measure_" + measurement + ".plt";
      std::ofstream outfile (file.c_str ());
      gnuplot.GenerateOutput (outfile);
    }
  }

  void
  NetMeasure::EndFlowMonitor ()
  {
    m_flowMon->SerializeToXmlFile ("flowMonitor.xml", true, true);
  }

  void
  NetMeasure::SetFlowMonitor (NodeContainer &nodes)
  {
    NS_LOG_INFO ("Set the flow monitor");
    FlowMonitorHelper flowmonHelper;
    m_flowMon = flowmonHelper.Install (nodes);

    Simulator::Schedule (m_eos, &NetMeasure::EndFlowMonitor, this); // call to EndPlot at End of Simulation
  }

  void
  NetMeasure::InitFlowStats (FlowMonitor::FlowStats &flowStats)
  {
    // alt: flowStat = {0};
    flowStats.delaySum = Seconds (0);
    flowStats.jitterSum = Seconds (0);
    flowStats.lastDelay = Seconds (0);
    flowStats.txBytes = 0;
    flowStats.rxBytes = 0;
    flowStats.txPackets = 0;
    flowStats.rxPackets = 0;
    flowStats.lostPackets = 0;
    flowStats.timesForwarded = 0;
    flowStats.delayHistogram.SetDefaultBinWidth (0.1);// this values is not set propertly
    //flowStats.jitterHistogram.SetDefaultBinWidth (m_jitterBinWidth);
    //flowStats.packetSizeHistogram.SetDefaultBinWidth (m_packetSizeBinWidth);
  }

  void
  NetMeasure::ExpandFlowStats (uint32_t oldSize, uint32_t newSize)
  {
    NS_LOG_INFO ("Old vs New size " << oldSize << "/" << newSize);

    m_vOldFlowStats.resize (newSize);

    // Flows
    for (uint32_t i = oldSize; i < newSize; i++)
    {
      std::ostringstream flowName;
      flowName << "Flow " << i + 1;
      NS_LOG_DEBUG ("flowName: i " << i << " " << flowName.str () << " measurement size " << m_mMeasurementDataset.size ());
      // Measurements
      for (MeasurementDataset::const_iterator it = m_mMeasurementDataset.begin (); it != m_mMeasurementDataset.end (); ++it)
      {
        Measurement measurement = it->first;
        std::vector < Gnuplot2dDataset > vDataSet = it->second;
        Gnuplot2dDataset newDataSet;
        newDataSet.SetTitle (flowName.str ());

        NS_LOG_DEBUG ("measurement " << measurement);

        if (measurement.compare ("Delay") != 0) // It's not usefull for HISTOGRAMS
        {
          //Put previous instant to zero.
          double preInitTime = Simulator::Now ().GetSeconds () - m_interval.GetSeconds ();
          if (preInitTime < 0)
          {
            preInitTime = 0;
          }
          newDataSet.Add (preInitTime, 0);
        }
        vDataSet.push_back (newDataSet);
        m_mMeasurementDataset[measurement] = vDataSet;
      }
      //FlowStats init:
      InitFlowStats (m_vOldFlowStats.at (i));
    }
  }

  double
  NetMeasure::CalcThroughput (FlowMonitor::FlowStats news, FlowMonitor::FlowStats olds)
  {
    //Throughput at MAC level: Data + UDP header 8 bytes + IP header 20 bytes
    double rate = ((((news.rxBytes - olds.rxBytes) * 8.0) / 1000000) / m_interval.GetSeconds ());
    NS_LOG_DEBUG (" Throughput " << rate << " Mbps ");

    return rate;
  }

  double
  NetMeasure::CalcMeanDelay (FlowMonitor::FlowStats news)
  {
    double meanDelay = news.delaySum.GetSeconds () / news.rxPackets;
    NS_LOG_DEBUG (" mean delay " << meanDelay << " sec");

    return meanDelay;
  }

  void
  NetMeasure::CalcDelayHist (FlowMonitor::FlowStats stats, Gnuplot2dDataset& plotDataSet)
  {
    NS_LOG_INFO ("Calculating delay histogram");
    //gnuplot.SetLegend ("Histogram, Frecuency"); in EndPlot?

    // FSTEP see gnuplot help styles fsteps vs histeps
    // style histograms not implemented in gnuplot.cc
    enum Gnuplot2dDataset::Style style = Gnuplot2dDataset::FSTEPS;
    plotDataSet.SetStyle (style);

    Histogram hist = stats.delayHistogram;
    double delay;
    double freq;
    double width = hist.GetBinWidth (0); // All bins have the same width
    uint32_t rxPackets = stats.rxPackets;

    for (uint32_t index = 0; index < hist.GetNBins (); index++)
    {
      delay = index * width;
      double count = hist.GetBinCount (index);
      freq = count / rxPackets * 100; //Normalize to 100

      plotDataSet.Add (delay, freq);
    }
  }

  void
  NetMeasure::GetFlowStats ()
  {
    MonStats stats;
    NS_LOG_INFO ("Get flow stats");

    m_flowMon->CheckForLostPackets ();
    stats = m_flowMon->GetFlowStats ();

    uint32_t oldSize = m_vOldFlowStats.size ();
    uint32_t newSize = stats.size ();
    if (newSize > oldSize)
    {
      ExpandFlowStats (oldSize, newSize);
    }

    FlowMonitor::FlowStats newStats;

    for (MonStats::const_iterator it = stats.begin (); it != stats.end (); ++it)
    {
      // Loading: FlowID = uint32_t, beigins from 1.
      uint32_t i = it->first - 1;
      newStats = it->second;

      NS_LOG_DEBUG (" FlowID " << it->first);

      FlowMonitor::FlowStats oldStats = m_vOldFlowStats.at (i);

      //Calculate:
      for (MeasurementDataset::const_iterator ite = m_mMeasurementDataset.begin (); ite != m_mMeasurementDataset.end (); ++ite)
      {
        Measurement measurement = ite->first;
        std::vector < Gnuplot2dDataset > vDataSet = ite->second;
        Gnuplot2dDataset plotDataSet = vDataSet.at (i);
        enum PlotType plotType = m_MeasPlotData[measurement].plotType;

        NS_LOG_DEBUG ("measurement " << measurement);
        Time time = Simulator::Now ();
        double timeD = time.GetSeconds ();

        switch (plotType)
        {
          case TIMEDOMAIN:
            double data;
            if (measurement.compare ("Throughput") == 0)
            {
              data = CalcThroughput (newStats, oldStats);
            } else if (measurement.compare ("delaySum") == 0)
            {
              data = CalcMeanDelay (newStats);
            } else if (measurement.compare ("lostPackets") == 0)
            {
              data = newStats.lostPackets;
            }
            plotDataSet.Add (timeD, data);

            break;
          case HISTOGRAM:// Works only in the last iteration
          {

            Time lastIteration = m_eos - m_interval;
            double lastIterationD = lastIteration.GetSeconds (); // errors debugging with timeD
            /// Doesn't work comparing directly time.GetSeconds () == lastIteration.GetSeconds ()
            if (timeD == lastIterationD)
            {
              NS_LOG_INFO ("last iteration:  " << timeD);
              if (measurement.compare ("Delay") == 0)
              {
                CalcDelayHist (newStats, plotDataSet);
              }
            }// end of Last Iteration
            break;
          }
          default:
            NS_LOG_ERROR ("  No correct plot type selected");
            exit (-1);
        }
        //Restore plot data set
        vDataSet.at (i) = plotDataSet;
      }
      // update
      oldStats = newStats;
      // Restore old stats
      m_vOldFlowStats.at (i) = oldStats;
    }
    Simulator::Schedule (m_interval, &NetMeasure::GetFlowStats, this);
  }

} // namespace ns3
