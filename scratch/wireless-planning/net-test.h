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
#ifndef NET_TEST_H
#define NET_TEST_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "ns3/ptr.h"
#include "ns3/socket.h"
#include "ns3/ipv4-address.h"
#include "ns3/gnuplot.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

#include "net-measure.h"

namespace ns3 {

    class AppState {
    public:
        uint8_t m_tid;
        enum AccessClass m_ac;
        AppState(enum AccessClass ac);
        void
        OnOffTxPacketCallback(std::string context, Ptr<const Packet> packet);
    };

    /**
     * @brief A class to test a net.
     *
     * -Sends echos.
     * -Runs OnOff application.
     */
    class NetTest {
    public:
        /**
         * Constructor:
         */
        NetTest();
        /**
         * Destructor
         */
        ~NetTest();

        static TypeId GetTypeId(void);

        /**
         * @brief Image representation setup
         *
         * @param eos End of Simultaion in seconds
         *
         * @return Gnuplot
         */
        void SetupPlot(uint32_t eos);

        /**
         * @brief Ends the plot generating the output file .ptl
         */
        void EndPlot(void);

        /**
         * @brief It setups an OnOff application with QoS.
         *
         * @param Server node name.
         * @param server port
         * @param Client node name.
         * @param Start time in seconds.
         * @param Stop time in seconds.
         * @param Rate in string mode.
         * @param PacketSize in bytes.
         * @param AccessClass for QoS.
         */
        void ApplicationSetup(std::string server, uint8_t port, std::string client,
                double start, double stop, std::string rate,
                uint32_t packetSize, AppState *appState);

        /**
         * @brief Packet Callback
         *
         * It is used to put QoS tags to packets
         *
         * @param context
         * @param packet
         */
        void
        OnOffTxPacketCallback(std::string context, Ptr<const Packet> packet);

        /**
         * @brief UcpEcho application: Just to test connectivity
         *
         * Sends a single packet in the given time, form client node to server node.
         * Port 9 is used by default.
         *
         * @param server node name.
         * @param client node name.
         * @param start The instant to be sent packet.
         */
        void Echo(std::string server, std::string client, double start);

        /**
         * @brief UcpEcho application: Just to test connectivity
         *
         * Sends a single packet in the given time, form client node to server node.
         *
         * @param server node name.
         * @param server port
         * @param client node name.
         * @param start The instant to be sent packet.
         */
        void Echo(std::string server, uint8_t port, std::string client, double start);

        /*
         * @brief Returns the node pointer if there exists a node with that name.
         *
         * @para name Node name.
         *
         * @retun node Node pointer.
         */
        Ptr<Node> GetNodeFromName(string name);

    private:

    };

} // namespace ns3



#endif /* NET_TEST_H */

