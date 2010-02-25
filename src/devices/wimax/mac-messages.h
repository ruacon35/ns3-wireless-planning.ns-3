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


/*
 *This file does not contain all MAC messages, the rest of MAC messages have
 *This been categorized as DL and UL messages and are placed in
 *This dl-mac-messages.h and ul-mac-messages.h files.
 */

#ifndef MANAGEMENT_MESSAGE_TYPE_H
#define MANAGEMENT_MESSAGE_TYPE_H

#include <stdint.h>
#include "ns3/header.h"

namespace ns3 {

class ManagementMessageType : public Header
{
public:
  enum MessageType
  {
    MESSAGE_TYPE_DL_MAP,
    MESSAGE_TYPE_DCD,
    MESSAGE_TYPE_UL_MAP,
    MESSAGE_TYPE_UCD,
    MESSAGE_TYPE_RNG_REQ,
    MESSAGE_TYPE_RNG_RSP,
    MESSAGE_TYPE_REG_REQ,
    MESSAGE_TYPE_REG_RSP,
    MESSAGE_TYPE_DSA_REQ,
    MESSAGE_TYPE_DSA_RSP,
    MESSAGE_TYPE_DSA_ACK
  };

  ManagementMessageType (void);
  ManagementMessageType (uint8_t type);
  virtual ~ManagementMessageType (void);
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

#endif /* MANAGEMENT_MESSAGE_TYPE_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef RNG_REQ_H
#define RNG_REQ_H

#include <stdint.h>
#include "ns3/header.h"
#include "ns3/mac48-address.h"
#include "service-flow.h"

namespace ns3 {

class RngReq : public Header
{
public:
  RngReq (void);
  virtual ~RngReq (void);

  void SetReqDlBurstProfile (uint8_t reqDlBurstProfile);
  void SetMacAddress (Mac48Address macAddress);
  void SetRangingAnomalies (uint8_t rangingAnomalies);

  uint8_t GetReqDlBurstProfile (void) const;
  Mac48Address GetMacAddress (void) const;
  uint8_t GetRangingAnomalies (void) const;

  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  void PrintDebug (void) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:
  uint8_t m_reserved; // changed as per the amendment 802.16e-2005

  // TLV Encoded Information
  uint8_t m_reqDlBurstProfile;
  Mac48Address m_macAddress;
  uint8_t m_rangingAnomalies;

  // fields to be implemented later on:
  // uint8_t m_aasBroadcastCapability;
  // uint8_t m_macVersion;
};

} // namespace ns3

#endif /* RNG_REQ_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef RNG_RSP_H
#define RNG_RSP_H

#include <stdint.h>
#include "ns3/header.h"
#include "ns3/mac48-address.h"
#include "service-flow.h"
#include "cid.h"

namespace ns3 {

class RngRsp : public Header
{
public:
  RngRsp (void);
  virtual ~RngRsp (void);

  void SetTimingAdjust (uint32_t timingAdjust);
  void SetPowerLevelAdjust (uint8_t powerLevelAdjust);
  void SetOffsetFreqAdjust (uint32_t offsetFreqAdjust);
  void SetRangStatus (uint8_t rangStatus);
  void SetDlFreqOverride (uint32_t dlFreqOverride);
  void SetUlChnlIdOverride (uint8_t ulChnlIdOverride);
  void SetDlOperBurstProfile (uint16_t dlOperBurstProfile);
  void SetMacAddress (Mac48Address macAddress);

  void SetBasicCid (Cid basicCid);
  void SetPrimaryCid (Cid primaryCid);

  void SetAasBdcastPermission (uint8_t aasBdcastPermission);
  void SetFrameNumber (uint32_t frameNumber);
  void SetInitRangOppNumber (uint8_t initRangOppNumber);
  void SetRangSubchnl (uint8_t rangSubchnl);

  uint32_t GetTimingAdjust (void) const;
  uint8_t GetPowerLevelAdjust (void) const;
  uint32_t GetOffsetFreqAdjust (void) const;
  uint8_t GetRangStatus (void) const;
  uint32_t GetDlFreqOverride (void) const;
  uint8_t GetUlChnlIdOverride (void) const;
  uint16_t GetDlOperBurstProfile (void) const;
  Mac48Address GetMacAddress (void) const;
  Cid GetBasicCid (void) const;
  Cid GetPrimaryCid (void) const;
  uint8_t GetAasBdcastPermission (void) const;
  uint32_t GetFrameNumber (void) const;
  uint8_t GetInitRangOppNumber (void) const;
  uint8_t GetRangSubchnl (void) const;

  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:
  uint8_t m_reserved; // changed as per the amendment 802.16e-2005

  // TLV Encoded Information

  // Tx timing offset adjustment (signed 32-bit). The time required to advance SS transmission so frames
  // arrive at the expected time instance at the BS.
  uint32_t m_timingAdjust;

  // Specifies the relative change in transmission power level that the SS is to make in order that
  // transmissions arrive at the BS at the desired power. When subchannelization is employed, the
  // subscriber shall interpret the power offset adjustment as a required change to the transmitted power
  // density.
  uint8_t m_powerLevelAdjust;

  // Specifies the relative change in transmission frequency that the SS is to make in order to better match
  // the BS. This is fine-frequency adjustment within a channel, not reassignment to a different channel.
  uint32_t m_offsetFreqAdjust;
  uint8_t m_rangStatus;

  // Center frequency, in kHz, of new downlink channel where the SS should redo initial ranging.
  uint32_t m_dlFreqOverride;

  // Licensed bands: The identifier of the uplink channel with which the SS is to redo initial ranging (not
  // used with PHYs without channelized uplinks).
  uint8_t m_ulChnlIdOverride;

  // This parameter is sent in response to the RNG-REQ Requested Downlink Burst Profile parameter.
  // Byte 0: Specifies the least robust DIUC that may be used by the BS for transmissions to the SS.
  // Byte 1: Configuration Change Count value of DCD defining the burst profile associated with DIUC.
  uint16_t m_dlOperBurstProfile;

  Mac48Address m_macAddress;
  Cid m_basicCid;
  Cid m_primaryCid;
  uint8_t m_aasBdcastPermission;

  // Frame number where the associated RNG_REQ message was detected by the BS. Usage is mutually
  // exclusive with SS MAC Address
  uint32_t m_frameNumber;

  // Initial Ranging opportunity (1–255) in which the associated RNG_REQ message was detected by the BS.
  // Usage is mutually exclusive with SS MAC Address
  uint8_t m_initRangOppNumber;

  // Used to indicate the OFDM subchannel reference that was used to transmit the initial ranging message
  // (OFDM with subchannelization).
  uint8_t m_rangSubchnl;
};

} // namespace ns3

#endif /* RNG_RSP_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef DSA_REQ_H
#define DSA_REQ_H

#include <stdint.h>
#include "ns3/header.h"
#include "ns3/buffer.h"
#include "cid.h"
#include "service-flow.h"

namespace ns3 {
class serviceFlow;
class DsaReq : public Header
{
public:
  DsaReq ();
  ~DsaReq ();
  DsaReq (ServiceFlow sf);
  void SetTransactionId (uint16_t transactionId);
  void SetSfid (uint32_t sfid);
  void SetCid (Cid cid);
  void SetServiceFlow (ServiceFlow sf);
  ServiceFlow GetServiceFlow (void) const;
  uint16_t GetTransactionId (void) const;
  uint32_t GetSfid (void) const;
  Cid GetCid (void) const;
  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);

private:
  uint16_t m_transactionId;
  // TLV Encoded Information
  uint32_t m_sfid;
  Cid m_cid;
  ServiceFlow m_serviceFlow;
};

} // namespace ns3

#endif /* DSA_REQ_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef DSA_RSP_H
#define DSA_RSP_H

#include <stdint.h>
#include "ns3/header.h"
#include "ns3/buffer.h"
#include "cid.h"

namespace ns3 {

class DsaRsp : public Header
{
public:
  DsaRsp (void);
  ~DsaRsp (void);
  DsaRsp (ServiceFlow sf);

  void SetTransactionId (uint16_t transactionId);
  uint16_t GetTransactionId (void) const;

  void SetConfirmationCode (uint16_t confirmationCode);
  uint16_t GetConfirmationCode (void) const;

  void SetSfid (uint32_t sfid);
  uint32_t GetSfid (void) const;

  void SetCid (Cid cid);
  Cid GetCid (void) const;

  void SetServiceFlow (ServiceFlow sf);
  ServiceFlow GetServiceFlow (void) const;

  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:
  uint16_t m_transactionId;
  uint8_t m_confirmationCode;
  // TLV Encoded Information
  ServiceFlow m_serviceFlow;
  uint32_t m_sfid;
  Cid m_cid;

};

} // namespace ns3

#endif /* DSA_RSP_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef DSA_ACK_H
#define DSA_ACK_H

#include <stdint.h>
#include "ns3/header.h"
#include "ns3/buffer.h"

namespace ns3 {

class DsaAck : public Header
{
public:
  DsaAck (void);
  ~DsaAck (void);

  void SetTransactionId (uint16_t transactionId);
  uint16_t GetTransactionId (void) const;

  void SetConfirmationCode (uint16_t confirmationCode);
  uint16_t GetConfirmationCode (void) const;

  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:
  uint16_t m_transactionId;
  uint8_t m_confirmationCode;
};

} // namespace ns3

#endif /* DSA_ACK_H */

