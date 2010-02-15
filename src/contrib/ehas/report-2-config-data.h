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


namespace ns3 {

    /**
     * @brief Sets Devices and Channels data to structures
     */
    class Report2ConfigData {
    public:
        Report2ConfigData();
        ~Report2ConfigData();

        static TypeId GetTypeId(void);

        NetworkConfig::NetworkData NetData2NetworkData(NetDataStruct::NetData net);

    private:
        void AddDevice2Node(NetworkConfig::NodeData &node, NetDataStruct::SubnetData subnet, uint16_t index, uint16_t chId);

        /**
         * @brief Translates role from Radio Mobile to MAC Type
         *
         * e.g. Role = Terminal, Node => MAC = STA, AP
         *
         * @param role
         * 
         * @return NetworkConfig::MacType
         */
        NetworkConfig::MacType Role2MacType(string role);


        NetworkConfig m_config;

    };

} // namespace ns3

#endif	/* _REPORT_2_CONFIG_DATA_H */