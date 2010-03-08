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
        void SetWifiPhy();

        /**
         * @brief Set Ipv4AddressHelper vector
         */
        void SetIpAddresser();

        /**
         * @brief Set non-QoS Wifi MAC
         *
         * UNDER CONSTRUCTION
         */
        NqosWifiMacHelper SetWifiNqosMac(NetworkConfig::DeviceData deviceData);

        /**
         * @brief Set QoS Wifi MAC
         *
         *  UNDER CONSTRUCTION
         */
        QosWifiMacHelper SetWifiQosMac(NetworkConfig::DeviceData deviceData);

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

        /*
         * @ Adapts MAC layer for long distance communication
         *
         * The modifyed values are: ACKTimeout, CTSTimeout and SlotTime
         */
        void WifiLongDistances (NetDeviceContainer device, double distance);

        /**
         * @brief Builds the network installing all the devices to all the nodes
         * and connection all the devices to the proper link.
         *
         * The result is ilustrated in the m_nodes NodeContainer.
         */
        void NetworkBuilding(void);

        /**
         * @brief Sets a device given all the necessary information.
         *
         * UNDER CONSTRUCTION
         * @param deviceData
         * @param node
         */
        NetDeviceContainer WifiDeviceInstallation(NetworkConfig::DeviceData deviceData, Ptr<Node> node);

        NodeContainer m_nodes;// substitude with NodeContainer::GetGlobal()??
        NetworkConfig::NodeData m_nodeData;

        NetworkConfig::NetworkData m_networkData; ///< NetworkData an attribute.
        NetworkConfig::VectorNodeData m_vectorNodeData;
        NetworkConfig::VectorChannelData m_vectorChannelData;
        NetworkConfig::VectorWifiChannelData m_vWifiChData;
        NetworkConfig::VectorWimaxChannelData m_vWimaxChData;


        
        uint32_t m_nNodes;

        std::vector<YansWifiPhyHelper> m_vectorWifiPhy;
        std::vector<Ipv4AddressHelper> m_vectorAddresser;
    };

} // namespace ns3

#endif
