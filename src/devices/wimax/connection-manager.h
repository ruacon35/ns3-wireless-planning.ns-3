/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008,2009 INRIA, UDcast
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *          Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                               <amine.ismail@UDcast.com>
 */

#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <stdint.h>
#include "cid.h"
#include "wimax-connection.h"
#include "ns3/mac48-address.h"

namespace ns3 {

class CidFactory;
class SSRecord;
class RngRsp;
class WimaxNetDevice;
class SubscriberStationNetDevice;

/*
 The same connection manager class serves both for BS and SS though some functions are exclusive to only one of them.
 */

class ConnectionManager : public Object
{
public:
  ConnectionManager (void);
  ~ConnectionManager (void);
  void DoDispose (void);
  void SetCidFactory (CidFactory *cidFactory);
  void AllocateManagementConnections (SSRecord *ssRecord, RngRsp *rngrsp);
  Ptr<WimaxConnection> CreateConnection (Cid::Type type);
  void AddConnection (Ptr<WimaxConnection> connection, Cid::Type type);
  Ptr<WimaxConnection> GetConnection (Cid cid);
  std::vector<Ptr<WimaxConnection> > GetConnections (Cid::Type type) const;
  uint32_t GetNPackets (Cid::Type type, ServiceFlow::SchedulingType schedulingType) const;
  bool HasPackets (void) const;
private:
  std::vector<Ptr<WimaxConnection> > m_basicConnections;
  std::vector<Ptr<WimaxConnection> > m_primaryConnections;
  std::vector<Ptr<WimaxConnection> > m_transportConnections;
  std::vector<Ptr<WimaxConnection> > m_multicastConnections;
  // only for BS
  CidFactory *m_cidFactory;
};

}
; // namespace ns3

#endif /* CONNECTION_MANAGER_H */

