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
  NetworkConfig::SetWifiDeviceData (std::string name, Ipv4Address address,
          uint16_t chId, enum NetworkConfig::MacType macType)
  {
    NetworkConfig::DeviceData deviceData;
    deviceData.name = name;
    deviceData.ipAddress = address;
    deviceData.chId = chId;
    //WIFI
    deviceData.wifiMacType = macType;
    deviceData.distance = 1;

    return deviceData;
  }

  NetworkConfig::DeviceData
  NetworkConfig::SetWifiDeviceData (uint16_t chId,
          enum NetworkConfig::MacType macType, double distance)
  {
    NetworkConfig::DeviceData deviceData;
    deviceData.name = "";
    deviceData.ipAddress = "";
    deviceData.chId = chId;
    deviceData.distance = distance;
    deviceData.comStandard = NetworkConfig::WIFI;
    deviceData.wifiMacType = macType;

    return deviceData;
  }

  NetworkConfig::DeviceData
  NetworkConfig::SetWimaxDeviceData (uint16_t chId, double distance,
          WimaxHelper::NetDeviceType wimaxDeviceType,
          WimaxHelper::SchedulerType scherdulerType,
          WimaxPhy::ModulationType modulationType)
  {
    NetworkConfig::DeviceData deviceData;
    deviceData.name = "";
    deviceData.ipAddress = "";
    deviceData.chId = chId;
    deviceData.distance = distance;
    deviceData.comStandard = NetworkConfig::WIMAX;
    deviceData.wimaxDeviceType = wimaxDeviceType;
    deviceData.scherdulerType = scherdulerType;
    deviceData.modulationType = modulationType;

    return deviceData;
  }

  void
  NetworkConfig::SetWifiChannelData (uint16_t id, std::string mode, NetworkConfig::VectorWifiChannelData& vectorWifiChannelData)
  {
    NetworkConfig::WifiChannelData ch;
    ch.id = id;
    ch.wifiMode = mode;
    vectorWifiChannelData.push_back (ch);
  }

} // namespace ns3

