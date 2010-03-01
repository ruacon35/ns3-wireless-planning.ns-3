#include "network-config.h"

namespace ns3 {

  NetworkConfig::NetworkConfig ()
  {
  }

  NetworkConfig::~NetworkConfig ()
  {
  }

  TypeId
  NetworkConfig::GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::NetworkConfig")
            .SetParent<Object > ()
            ;
    return tid;
  }

  NetworkConfig::DeviceData
  NetworkConfig::SetDeviceData (std::string name, Ipv4Address address,
          uint16_t chId, enum NetworkConfig::MacType macType)
  {
    NetworkConfig::DeviceData deviceData;
    deviceData.name = name;
    deviceData.ipAddress = address;
    deviceData.chId = chId;
    deviceData.macType = macType;
    deviceData.distance = 1;

    return deviceData;
  }

  NetworkConfig::DeviceData
  NetworkConfig::SetDeviceData (uint16_t chId, 
  enum NetworkConfig::MacType macType, double distance)
  {
    NetworkConfig::DeviceData deviceData;
    deviceData.name = "";
    deviceData.ipAddress = "";
    deviceData.chId = chId;
    deviceData.macType = macType;
    deviceData.distance = distance;

    return deviceData;
  }

  void
  NetworkConfig::SetChannelData (uint16_t id, std::string mode, NetworkConfig::VectorChannelData& vectorChannelData)
  {
    NetworkConfig::ChannelData ch;
    ch.id = id;
    ch.mode = mode;
    vectorChannelData.push_back (ch);
  }

} // namespace ns3

