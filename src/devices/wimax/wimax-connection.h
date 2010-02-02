/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008, 2009 INRIA, UDcast
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
 * Author: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *         Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */

#ifndef WIMAX_CONNECTION_H
#define WIMAX_CONNECTION_H

#include <stdint.h>
#include <ostream>
#include "cid.h"
#include "wimax-mac-header.h"
#include "wimax-mac-queue.h"
#include "ns3/object.h"
#include "service-flow.h"

namespace ns3 {

class ServiceFlow;
class Cid;

class WimaxConnection : public Object
{
public:
  static TypeId GetTypeId (void);

  WimaxConnection (Cid cid, enum Cid::Type type);
  ~WimaxConnection (void);

  Cid GetCid (void) const;

  enum Cid::Type GetType (void) const;

  Ptr<WimaxMacQueue> GetQueue (void) const;

  void SetServiceFlow (ServiceFlow *serviceFlow);
  ServiceFlow* GetServiceFlow (void) const;

  // wrapper functions
  uint8_t GetSchedulingType (void) const;
  bool Enqueue (Ptr<Packet> packet, const MacHeaderType &hdrType, const GenericMacHeader &hdr);
  Ptr<Packet> Dequeue (MacHeaderType::HeaderType packetType = MacHeaderType::HEADER_TYPE_GENERIC);
  bool HasPackets (void) const;
  bool HasPackets (MacHeaderType::HeaderType packetType) const;

  std::string GetTypeStr (void) const;
private:
  virtual void DoDispose (void);

  Cid m_cid;
  enum Cid::Type m_cidType;
  Ptr<WimaxMacQueue> m_queue;
  ServiceFlow *m_serviceFlow;
};

} // namespace ns3

#endif /* WIMAX_CONNECTION_H */
