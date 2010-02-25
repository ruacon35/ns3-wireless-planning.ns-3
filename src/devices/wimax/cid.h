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


#ifndef CID_H
#define CID_H

#include <stdint.h>
#include <ostream>

namespace ns3 {

class Cid
{
public:
  enum Type
  {
    BROADCAST = 1,
    INITIAL_RANGING,
    BASIC,
    PRIMARY,
    TRANSPORT,
    MULTICAST,
    PADDING
  };

  // Create a cid of unknown type
  Cid (void);
  Cid (uint16_t cid);
  ~Cid (void);

  uint16_t GetIdentifier (void) const;

  bool IsMulticast (void) const;
  bool IsBroadcast (void) const;
  bool IsPadding (void) const;
  bool IsInitialRanging (void) const;

  static Cid Broadcast (void);
  static Cid Padding (void);
  static Cid InitialRanging (void);

private:
  friend class CidFactory;
  friend bool operator == (const Cid &lhs, const Cid &rhs);
  uint16_t m_identifier;
};

bool operator == (const Cid &lhs, const Cid &rhs);
bool operator != (const Cid &lhs, const Cid &rhs);

std::ostream & operator << (std::ostream &os, const Cid &cid);

} // namespace ns3

#endif /* CID_H */
