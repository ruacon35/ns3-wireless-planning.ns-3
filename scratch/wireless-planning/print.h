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

#ifndef PRINT_H
#define PRINT_H

#include "ns3/node-container.h"

#include "net-data-struct.h"
#include "network-config.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

#include <iostream>
#include <string>
#include <fstream>

namespace ns3 {

    /**
     * @brief A class that helps to print information.
     *
     * NS_LOG_INFO (leve_info) must be enabled.
     */

    class Print {
    public:
        Print();
        ~Print();

        static TypeId GetTypeId(void);
        
        /**
         * @brief Prints information of a node container. Its nodes, the devices of
         * each node and the physical address of each device
         *
         * Notice that using the function GetGlobal () in any node container you get
         * all the nodes created in the script.
         *
         * @param A node container
         */
        static void NodeList(NodeContainer nodes);

        /**
         * @brief Prints Vector Strings
         *
         * @param vString
         */
        static void VectorString(std::vector<std::string> vStr);

        /**
         * @brief Prints all VectorSubnetData
         * 
         * @param vSubnetData
         */
        static void VectorSubnet(NetDataStruct::VectorSubnetData);

        /**
         * @brief Prints all information extracted from a netinfo text file
         *
         * @param netData
         */
        static void Netinfo(NetDataStruct::NetData netData);

        /**
         * @brief Prints all the network data set by the user.
         *
         * It is basically separated y data related to channels and data related to nodes.
         * This is information about the desired net.
         * It is not information about the build network
         *
         * @see print class to print the build network information.
         *
         * @param NetworkConfig::NetworkData The structure of the data to describe a network
         *
         * @see PrinteNodeData
         * @see ChannelData
         */
        static void NetworkData(NetworkConfig::NetworkData networkData);

        /**
         * @brief Prints information about the nodes set by the user.
         *
         * This is information about the desired nodes.
         * It is not information about the build network
         *
         * @param NetworkConfig::VectorNodeData A vector containing data of the nodes.
         *
         */
        static void NodeData(NetworkConfig::VectorNodeData vectorNodeData);

        /**
         * @brief Prints information about the channels set by the user.
         *
         * This is information about the desired channels.
         * It is not information about the build network.
         *
         * @param NetworkConfig::VectorChannelData A vector containing data of the channels.
         *
         */
        static void ChannelData(NetworkConfig::VectorChannelData vectorChannelData);
    };

}// namespace ns3

#endif /* PRINT_H */
