/* 
 * File:   report-2-config-data.h
 * Author: patxi
 *
 * Created on 20 January 2010, 11:55
 */

#ifndef _REPORT_2_CONFIG_DATA_H
#define	_REPORT_2_CONFIG_DATA_H

#include "net-data-struct.h"
#include "network-config.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

using namespace std;

namespace ns3 {

    /**
     * @brief Sets Devices and Channels data to structures
     */
    class Report2ConfigData {
    public:
        Report2ConfigData();
        ~Report2ConfigData();

        static TypeId GetTypeId(void);

        static NetworkConfig::NetworkData NetData2NetworkData(NetDataStruct::NetData net);

    private:
        static void AddDevice2Node(NetworkConfig::NodeData &node, NetDataStruct::SubnetData subnet, uint16_t index, uint16_t chId);

        /**
         * @brief Translates communication standard from string to enum
         *
         * @param communicationStandard
         *
         * @return NetworkConfig::CommunicationStandard WIFI or WIMAX
         */
        static NetworkConfig::CommunicationStandard ReadStandard(string communicationStandard);

        /**
         * @brief Translates role from Radio Mobile to MAC Type
         *
         * e.g. Role = Terminal, Node => MAC = STA, AP
         *
         * @param role
         * 
         * @return NetworkConfig::MacType
         */
        static NetworkConfig::MacType Role2MacType(string role);

        /**
         * @brief Translates role to wimax net device
         *
         * @param role
         *
         * @return WimaxHelper::NetDeviceType BB / SS
         */
        static WimaxHelper::NetDeviceType Role2WimaxDeviceType(string role);

        /**
         * @brief Translates modulation from string to enum
         *
         * @param communicationStandard
         *
         * @return NetworkConfig::CommunicationStandard WIFI or WIMAX
         */
        static enum WimaxPhy::ModulationType ReadModulation(string modulation);
    };

} // namespace ns3

#endif	/* _REPORT_2_CONFIG_DATA_H */
