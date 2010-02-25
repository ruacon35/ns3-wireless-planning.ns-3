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
#ifndef CREATE_NETWORK_H
#define CREATE_NETWORK_H

#include <iostream>
#include <string>
#include <fstream>

#include "ns3/node-container.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/nqos-wifi-mac-helper.h"
#include "ns3/qos-wifi-mac-helper.h"
#include "ns3/type-id.h"
#include "ns3/object.h"



#include "network-config.h"

namespace ns3 {

    /**
     * @brief This Class creates all the network given the needed information, NetworkData
     */
    class CreateNetwork {
    public:
        /**
         * Constructor:
         */
        CreateNetwork();
        /**
         * Destructor
         */
        ~CreateNetwork();

        static TypeId GetTypeId(void);

        /**
         * @brief Creates the network from the given data as a NodeContainer
         *
         * This is the main function of this class.
         *
         * @param networkData All the necessary data to build the network.
         *
         * @return m_nodes is the NodeContainer of the configured network.
         */
        NodeContainer Create(NetworkConfig::NetworkData networkData);

    private:
        /**
         * @brief Set PHY
         */
        void SetPhy();

        /**
         * @brief Set Ipv4AddressHelper vector
         */
        void SetIpAddresser();

        //
        /**
         * @brief Set Wifi MAC
         *
         *  UNDER CONSTRUCTION
         */
        // WifiMacHelper SetMac (enum NetworkConfig::MacType type);

        /**
         * @brief Set non-QoS Wifi MAC
         *
         * UNDER CONSTRUCTION
         */
        NqosWifiMacHelper SetNqosMac(NetworkConfig::DeviceData deviceData);

        /**
         * @brief Set QoS Wifi MAC
         *
         *  UNDER CONSTRUCTION
         */
        QosWifiMacHelper SetQosMac(NetworkConfig::DeviceData deviceData);

        /**
         * @brief Assigns an Ipv4 address to a device.
         * Based on Ipv4AddressHelper::Assign
         * Unused
         *
         * @param NetDevice
         * @param Ipv4Address can be given in string mode, e.g. "192.168.2.1"
         *
         * @return Ipv4InterfaceContainer contains all the interfaces.
         */
        void SetIpAddress (NetDevice &device, Ipv4Address address);

        /**
         * @brief Sets all the devices of a node given all the necessary information.
         *
         * NEEDS TO RENAME
         *
         * The new device will be pushed to the tail of the device list.
         */
        void SetAllNodes(void);

        /**
         * @brief Sets a device given all the necessary information.
         *
         * UNDER CONSTRUCTION
         * @param deviceData
         * @param node
         */
        NetDeviceContainer DeviceInstallation(NetworkConfig::DeviceData deviceData, Ptr<Node> node);

        NetDeviceContainer WifiInstall (const WifiPhyHelper &phyHelper,
                     const WifiMacHelper &macHelper, NodeContainer c);

        NetworkConfig m_networkDataStruct; ///< NetworkConfig object.
        NetworkConfig::NetworkData m_networkData; ///< NetworkData an attribute.
        NetworkConfig::VectorNodeData m_vectorNodeData;
        NetworkConfig::VectorChannelData m_vectorChannelData;
        NodeContainer m_nodes;
        NetworkConfig::NodeData m_nodeData;
        uint32_t m_nNodes;

    private:
        std::vector<YansWifiPhyHelper> m_vectorWifiPhy;
        std::vector<Ipv4AddressHelper> m_vectorAddresser;
    };

} // namespace ns3

#endif
