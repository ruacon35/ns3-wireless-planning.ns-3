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

#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/names.h"
#include "ns3/ipv4.h"

#include "util.h"

NS_LOG_COMPONENT_DEFINE ("util");

namespace ns3
{

 Util::Util () { }

 Util::~Util () { }

 TypeId
 Util::GetTypeId (void)
 {
  static TypeId tid = TypeId ("ns3::Util")
          .SetParent<Object > ()
          ;
  return tid;
 }

 Ipv4Address
 Util::GetIpAddrFromNode (Ptr<Node> node)
 {
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4 > ();
  Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1, 0);
  Ipv4Address ipAddr = iaddr.GetLocal (); //Get's the address of the 1st device?

  NS_LOG_INFO ("Node: " << node << " Ipv4Address: " << ipAddr);

  return ipAddr;
 }

 Ipv4Address
 Util::GetIpAddrFromName (std::string name)
 {
  Ptr<Node> node = Names::Find <Node> (name);
  Ipv4Address ipAddr = GetIpAddrFromNode (node);

  NS_LOG_INFO ("Node: " << name << " Ipv4Address: " << ipAddr);

  return ipAddr;
 }

 uint8_t
 Util::QosUtilsMapAcToTid (enum AccessClass ac)
 {
  uint8_t tid = 9; // >8 by default
  switch (ac)
   {
   case 0:// AC_VO
    tid = 6; // 6,7
    break;
   case 1:// AC_VI
    tid = 4; // 4,5
    break;
   case 2:// AC_BE
    tid = 0;
    break;
   case 3:// AC_BK
    tid = 1; // 1,2
    break;
   case 4:// AC_BE_NQOS
    tid = 3; // ???
    break;
   case 5:// AC_UNDEF
    tid = 8; // >7 AC_VI;
    break;
   }
  return tid;
 }

 int16_t
 Util::FindStrInVectorStr (string str, vector<string> vStr)
 {
  int16_t index = -1;

  for (uint16_t i = 0; i < vStr.size (); i++)
   {
    int16_t match = str.compare (vStr.at (i));
    NS_LOG_INFO ("match: " << match << " str: " << str << " vStr(" << i << "): " << vStr.at (i));

    if (match == 0)
     {
      index = i;
      break; //stops search
     }
   }
  return index;
 }

} // namespace ns3


