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
#ifndef UTIL_H
#define UTIL_H

#include "ns3/node.h"
#include "ns3/ipv4-address.h"
#include "ns3/qos-utils.h"
#include "ns3/type-id.h"

using namespace std;

namespace ns3 {
 
/**
 * @brief A class that offers utilities.
 * 
 * -Get's an IP address from the name of the node.
 * -
 */  
class Util
{
public:
  /**
   * Constructor:
   */ 
  Util ();
  /**
   * Destructor
   */ 
  ~Util ();

  static TypeId GetTypeId(void);

  /**
   * @brief Get the IP address of a node.
   * 
   * @param node Node 
   * 
   * @return Ipv4Address
   */ 
   static Ipv4Address GetIpAddrFromNode (Ptr<Node> node);   
   
  /**
   * @brief Get node IP address from name
   * 
   * @param name Node name
   * 
   * @return Ipv4Address
   */ 
   static Ipv4Address GetIpAddrFromName (string name);

  /**
   * @brief Get Tag ID for QoS from the access class.
   *
   * This method sould be included in devices/wifi/qos-utils.cc
   * Let's get apart ehas contributions for the moment.
   *
   * @param ac AccessClass
   *
   * @return tid
   */
   static uint8_t QosUtilsMapAcToTid (enum AccessClass ac);

  /**
   * @brief Finds a string in a vector string
   *
   * It finds only once. A string listed twice in the vector is not contemplated.
   *
   * @return Index: not found = -1;
   */
    static int16_t FindStrInVectorStr (string str, vector<string> vStr);

  uint8_t m_init;


};

} // namespace ns3 

#endif /* UTIL_H */
  
