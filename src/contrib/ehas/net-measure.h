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
#ifndef NET_MEASURE_H
#define NET_MEASURE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "ns3/ptr.h"
#include "ns3/socket.h"
#include "ns3/ipv4-address.h"
#include "ns3/gnuplot.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

namespace ns3 {

    /**
     * @brief A class that helps to print information.
     *
     * NS_LOG_INFO (leve_info) must be enabled.
     */

    /**
     * @brief A class to test a net.
     *
     * -Calculates the throughput and plots it.
     * -Sends echos.
     * -Runs OnOff application.
     */
    class NetMeasure {
    public:
        /**
         * Constructor:
         */
        NetMeasure();
        /**
         * @brief NetMeasure
         *
         * @param eos End Of Simulation in seconds
         * @param interval in seconds
         */
        NetMeasure(Time eos, Time interval);
        /**
         * Destructor
         */
        ~NetMeasure();

        static TypeId GetTypeId(void);

        typedef std::map< FlowId, FlowMonitor::FlowStats > MonStats;
        typedef std::string Measurement;
        typedef std::map < Measurement, std::vector< Gnuplot2dDataset > > MeasurementDataset;


        /**
         * @brief Defines the name of the gnuplot resultant image
         *
         * ./waf --run scratch/NetMeasure
         * gnuplot NetMeasure.plt
         * -Then open the *.png file.
         *
         * @param Number of realization.
         *
         * @return The name of the image.
         */
        std::string ImageName(int i);


        /**
         * @brief Image representation setup
         *
         * @param eos
         * @return Gnuplot
         */
        void SetupPlot();


        /**
         * @brief Sets FlowMonitor to the given nodes.
         *
         * @param nodes a NodeContainer
         *
         * @return FlowMonitor ptr
         * 
         * @see FlowMonitor
         */
        void SetFlowMonitor(NodeContainer &nodes);

        /**
         * @brief Gets FlowMonitor stats from the flow monitor.
         * 
         * Gets new stats every m_interval seconds.
         *
         * @param m_flowMon
         *
         * @return MonStats
         */
        void GetFlowStats();

    private:

        /*
         * @brief Expands all the needed vectors and initializes FlowStats
         *
         * @param oldSize
         * @param newSize
         */
        void ExpandFlowStats(uint32_t oldSize, uint32_t newSize);

        /*
         * @brief Initializes FlowMonitor::FlowStats structure.
         *
         * @param Address of the flow stats
         */
        void InitFlowStats(FlowMonitor::FlowStats &flowStats);

        /**
         * @brief Ends the plot generating the output file .ptl
         */
        void EndPlot(void);

        /**
         * @brief Ends the flow monitor generating the output file .xml
         */
        void EndFlowMonitor(void);

        /**
         * @brief Calculates the throughput a mac level.
         * 
         * Uses FlowStats::rxBytes
         *
         * @param news New statistics
         * @param olds Old statistics
         *
         * @return Throughput
         */
        double CalcThroughput(FlowMonitor::FlowStats news, FlowMonitor::FlowStats olds);
        /**
         * @brief Calculates the mean delay.
         *
         * Uses FlowStats::delaySum and rxPackets
         * meanDelay = delaySum / rxPackets
         *
         * @param news New statistics
         * @param olds Old statistics
         *
         * @return meanDelay
         */

        double CalcMeanDelay(FlowMonitor::FlowStats news);
        /**
         * @brief Calculates delay histogram.
         *
         * Uses FlowStats::delayHistogram
         * Translates it to gnuplot dataset
         *
         * @param stats EOS statistics
         *
         * @return data Gnuplot data set
         */
        Gnuplot2dDataset CalcDelayHist(FlowMonitor::FlowStats lasts);




        Time m_eos;
        Time m_interval;
        Ptr<FlowMonitor> m_flowMon;

        std::vector < std::string > m_flowMonMeas; ///< List of measurements
        std::vector < Measurement > m_measurements; ///< List of measurements

        MeasurementDataset m_mMeasurementDataset; ///< One data set vector per measurement
        //std::map < std::string, std::vector<Gnuplot2dDataset>  > m_mvPlotDataSetFlow; ///< One data set vector per measurement
        std::vector< FlowMonitor::FlowStats > m_vOldFlowStats; ///< One flow stats per flow
    };

} // namespace ns3

#endif /* NET_MEASURE_H */

