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

using namespace std;

namespace ns3 {

    /**
     * @brief A class that helps to print information.
     *
     * NS_LOG_INFO (leve_info) must be enabled.
     */

    /**
     * @brief A class to take masurementes from the network.
     */
    class NetMeasure {
    public:

        enum PlotType {
            TIMEDOMAIN,
            HISTOGRAM,
        };

        static TypeId GetTypeId(void);

        typedef map< FlowId, FlowMonitor::FlowStats > MonStats;
        typedef string Measurement;

        typedef struct {
            string title;
            string x;
            string y;
        } PlotData;

        typedef struct {
            double width;
            vector < double > data;
            vector < double > freq;
        } HistData;

        typedef struct {
            double initTime;
            vector < double > data;
        } TimeData;

        typedef struct {
            enum PlotType plotType;
            vector < TimeData > flowsTimeData; // for TIMEDOMAIN
            vector < HistData > flowsHistData; // for HISTOGRAM
            PlotData plotData;
        } MeasurementData;
        typedef map < Measurement, MeasurementData > MeasDataSet;


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

        /**
         * @brief Gets FlowNames
         *
         * @return flow names
         */
        vector < string > GetFlowNames();

        /**
         * @brief Sets FlowNames
         *
         * @param flow names
         */
        void SetFlowNames(vector < string > flowNames);

        /**
         * @brief Data Post Proccessing
         * Works over m_measDataSet
         */
        void DataPostProcess();


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
         * @param data Gnuplot data set
         */
        HistData CalcDelayHist(FlowMonitor::FlowStats stas);


        Time m_eos;
        Time m_interval;
        Ptr < FlowMonitor > m_flowMon;
        vector < string > m_flowNames;

        MeasDataSet m_measDataSet;
        vector< FlowMonitor::FlowStats > m_vOldFlowStats; ///< One flow stats per flow
    };

} // namespace ns3

#endif /* NET_MEASURE_H */

