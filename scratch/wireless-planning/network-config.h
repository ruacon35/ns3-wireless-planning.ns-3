#ifndef NETWORK_DATA_STRUCT_H
#define NETWORK_DATA_STRUCT_H

#include <vector>

#include "ns3/ipv4-address.h"
#include "ns3/type-id.h"
#include "ns3/object.h"
#include "ns3/vector.h"


namespace ns3 {

    /**
     * @brief The objetive of this class is to enable the access to NetworkData struct.
     *
     *  Comes from the need to create a node, taken information manually.
     *
     *
     * The names of the attributes are self-explanatory.
     *
     * Many typedef are defined to make the code more readable.
     *
     *
     * How can I define new types without constructing a class?
     */
    class NetworkConfig {
    public:

        /**
         * @enum macType
         *
         * @brief Enumeration of the possible types of MAC.
         *
         */
        enum MacType {
            AP, ///< Access Point non-QoSWIFI.   0
            STA, ///< Station non-QoS        .   1
            ADHOC, ///< Ad hoc non-QoS       .   2
            QAP, ///< QoS Access Point       .   3
            QSTA, ///< QoS Station           .   4
            QADHOC, ///< QoS Ad hoc          .   5
            BS, ///< Base Station  WIMAX     .   6
            SS, ///< Suscriber Station       .   7
        };

        /**
         * @struct Device Data
         */
        struct DeviceData {
            std::string name; ///< e.g. "eth0", "ath1": unused
            uint16_t chId; ///< ID of the channel wich is connected to.
            enum MacType macType;
            double distance; ///< km
            Ipv4Address ipAddress;
        };
        typedef struct DeviceData DeviceData;

        typedef std::vector<DeviceData> VectorDeviceData;

        struct NodeData {
            std::string name;
            Vector position;
            VectorDeviceData vectorDeviceData; ///< Contains all the devices of a node. Each interface corresponds to a device.
        };
        typedef struct NodeData NodeData;

        typedef std::vector<NodeData> VectorNodeData;

        struct ChannelData {
            uint16_t id; ///< channel number
            std::string mode; ///< e.g. wifia-6mbs @see WifiPhy
        };
        typedef struct ChannelData ChannelData;

        typedef std::vector<ChannelData> VectorChannelData;

        struct NetworkData {
            VectorChannelData vectorChannelData;
            VectorNodeData vectorNodeData;
        };
        typedef struct NetworkData NetworkData;


        NetworkConfig();
        ~NetworkConfig();

        static TypeId GetTypeId(void);

        /**
         * @brief Sets device data into DeviceData struct.
         *
         * @param name
         * @param IP address
         * @param channel number (Unused, remove it?)
         * @param macType AP, STA or ADHOC (enum type)
         *
         * @return NetworkConfig::DeviceData
         */
        NetworkConfig::DeviceData SetDeviceData(std::string name, Ipv4Address address,
                uint16_t chId, enum NetworkConfig::MacType macType);

        /**
         * @brief Sets device data into DeviceData struct.
         *
         * @param channel id (link id)
         * @param macType AP, STA or ADHOC (enum type)
         * @param distance to AP in km
         *
         * @return NetworkConfig::DeviceData
         */
        NetworkConfig::DeviceData SetDeviceData(uint16_t chId,
                enum NetworkConfig::MacType macType, double distance);

        /**
         * @brief Sets channel data into a vector of channel data struct.
         *
         * @param ID number
         * @param mode @see WifiPhy
         * @param channel number (Unused, remove it?)
         * @param vectorChannelData stores all the information of all the channels of the network. When we set a channel it is automatically added.
         */
        void SetChannelData(uint16_t id, std::string mode, NetworkConfig::VectorChannelData& vectorChannelData);


        NetworkData m_networkData;

    };

} // namespace ns3

#endif /* NETWORK_DATA_STRUCT_H */
