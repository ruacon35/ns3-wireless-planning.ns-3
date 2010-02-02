/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008 INRIA
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
 */

#ifndef PACKET_BURST_H
#define PACKET_BURST_H

#include <stdint.h>
#include <list>
#include "ns3/object.h"

namespace ns3 {

class Packet;

class PacketBurst : public Object
{
public:
  static TypeId GetTypeId (void);
  PacketBurst (void);
  virtual ~PacketBurst (void);

  Ptr<PacketBurst> Copy (void) const;
  void AddPacket (Ptr<Packet> packet);
  std::list<Ptr<Packet> > GetPackets (void) const;
  uint32_t GetNPackets (void) const;
  uint32_t GetSize (void) const;

  std::list<Ptr<Packet> >::const_iterator Begin (void) const;
  std::list<Ptr<Packet> >::const_iterator End (void) const;

  void Print (void) const;
private:
  void DoDispose (void);
  friend bool operator == (const PacketBurst &a, const PacketBurst &b);
  std::list<Ptr<Packet> > m_packets;
};
} // namespace ns3

#endif /* PACKET_BURST */
