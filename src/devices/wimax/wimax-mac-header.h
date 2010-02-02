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

#ifndef MAC_HEADER_TYPE_H
#define MAC_HEADER_TYPE_H

#include <stdint.h>
#include "ns3/header.h"

namespace ns3 {

/*Represents the HT (Header Type) field of generic MAC and bandwidth request headers*/

class MacHeaderType : public Header
{
public:
  enum HeaderType
  {
    HEADER_TYPE_GENERIC, HEADER_TYPE_BANDWIDTH
  };

  MacHeaderType (void);
  MacHeaderType (uint8_t type);
  virtual ~MacHeaderType (void);
  void SetType (uint8_t type);
  uint8_t GetType (void) const;

  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:
  uint8_t m_type;
};

}
; // namespace ns3

#endif /* MAC_HEADER_TYPE_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef GENERIC_MAC_HEADER_H
#define GENERIC_MAC_HEADER_H

#include <stdint.h>
#include "ns3/header.h"
#include "cid.h"

namespace ns3 {

class GenericMacHeader : public Header
{
public:
  GenericMacHeader (void);
  ~GenericMacHeader (void);

  void SetEc (uint8_t ec);
  void SetType (uint8_t type);
  void SetCi (uint8_t ci);
  void SetEks (uint8_t eks);
  void SetLen (uint16_t len);
  void SetCid (Cid cid);
  void SetHcs (uint8_t hcs);
  void SetHt (uint8_t HT);

  uint8_t GetEc (void) const;
  uint8_t GetType (void) const;
  uint8_t GetCi (void) const;
  uint8_t GetEks (void) const;
  uint16_t GetLen (void) const;
  Cid GetCid (void) const;
  uint8_t GetHcs (void) const;
  uint8_t GetHt (void) const;
  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  bool check_hcs (void) const;
private:
  uint8_t m_ht; // Header type
  uint8_t m_ec; // Encryption Control
  uint8_t m_type;
  uint8_t m_esf;
  uint8_t m_ci; // CRC Indicator
  uint8_t m_eks; // Encryption Key Sequence
  uint8_t m_rsv1;
  uint16_t m_len;
  Cid m_cid;
  uint8_t m_hcs; // Header Check Sequence
  uint8_t c_hcs; // calculated header check sequence; this is used to check if the receveid header is correrct or not


};

}
; // namespace ns3

#endif /* GENERIC_MAC_HEADER */

// ----------------------------------------------------------------------------------------------------------

#ifndef BANDWIDTH_REQUEST_HEADER_H
#define BANDWIDTH_REQUEST_HEADER_H

#include <stdint.h>
#include "ns3/header.h"
#include "cid.h"

namespace ns3 {

class BandwidthRequestHeader : public Header
{
public:
  enum HeaderType
  {
    HEADER_TYPE_INCREMENTAL, HEADER_TYPE_AGGREGATE
  };

  BandwidthRequestHeader (void);
  ~BandwidthRequestHeader (void);

  void SetHt (uint8_t HT);
  void SetEc (uint8_t ec);
  void SetType (uint8_t type);
  void SetBr (uint32_t br);
  void SetCid (Cid cid);
  void SetHcs (uint8_t hcs);

  uint8_t GetHt (void) const;
  uint8_t GetEc (void) const;
  uint8_t GetType (void) const;
  uint32_t GetBr (void) const;
  Cid GetCid (void) const;
  uint8_t GetHcs (void) const;

  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  bool check_hcs (void) const;
private:
  uint8_t m_ht; // Header type
  uint8_t m_ec; // Encryption Control
  uint8_t m_type;
  uint32_t m_br; // Bandwidth Request
  Cid m_cid; // Connection identifier
  uint8_t m_hcs; // Header Check Sequence
  uint8_t c_hcs; // calculated header check sequence; this is used to check if the receveid header is correrct or not

};

}
; // namespace ns3

#endif /* BANDWIDTH_REQUEST_HEADER_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef GRANT_MANAGEMENT_SUBHEADER_H
#define GRANT_MANAGEMENT_SUBHEADER_H

#include <stdint.h>
#include "ns3/header.h"

namespace ns3 {

class GrantManagementSubheader : public Header
{
public:
  GrantManagementSubheader (void);
  ~GrantManagementSubheader (void);

  void SetSi (uint8_t si);
  void SetPm (uint8_t pm);
  void SetPbr (uint16_t pbr);

  uint8_t GetSi (void) const;
  uint8_t GetPm (void) const;
  uint16_t GetPbr (void) const;

  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);

private:
  // size of the Grant Management Subheader shall actually be 2 bytes

  uint8_t m_si; // Slip Indicator
  uint8_t m_pm; // Poll-Me bit (byte in this case)
  uint16_t m_pbr; // PiggyBack Request
};

}
; // namespace ns3

#endif /* GRANT_MANAGEMENT_SUBHEADER_H */
