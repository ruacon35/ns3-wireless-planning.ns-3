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

#ifndef SERVICE_FLOW_RECORD_H
#define SERVICE_FLOW_RECORD_H

#include <stdint.h>
#include "ns3/nstime.h"
#include "bs-uplink-scheduler.h"
#include "bs-net-device.h"
#include "ns3/ptr.h"

namespace ns3 {
class ServiceFlowRecord
{
public:
  ServiceFlowRecord (void);
  ~ServiceFlowRecord (void);

  void SetGrantSize (uint32_t grantSize);
  uint32_t GetGrantSize (void) const;

  void SetGrantTimeStamp (Time grantTimeStamp);
  Time GetGrantTimeStamp (void) const;

  void SetDlTimeStamp (Time dlTimeStamp);
  Time GetDlTimeStamp (void) const;

  void SetPktsSent (uint32_t pktsSent);
  void UpdatePktsSent (uint32_t pktsSent);
  uint32_t GetPktsSent (void) const;

  void SetPktsRcvd (uint32_t pktsRcvd);
  void UpdatePktsRcvd (uint32_t pktsRcvd);
  uint32_t GetPktsRcvd (void) const;

  void SetBytesSent (uint32_t bytesSent);
  void UpdateBytesSent (uint32_t bytesSent);
  uint32_t GetBytesSent (void) const;

  void SetBytesRcvd (uint32_t bytesRcvd);
  void UpdateBytesRcvd (uint32_t bytesRcvd);
  uint32_t GetBytesRcvd (void) const;

  void SetRequestedBandwidth (uint32_t requestedBandwidth);
  void UpdateRequestedBandwidth (uint32_t requestedBandwidth);
  uint32_t GetRequestedBandwidth (void);

  void SetGrantedBandwidth (uint32_t grantedBandwidth);
  void UpdateGrantedBandwidth (uint32_t grantedBandwidth);
  uint32_t GetGrantedBandwidth (void);

  void SetGrantedBandwidthTemp (uint32_t grantedBandwidthTemp);
  void UpdateGrantedBandwidthTemp (uint32_t grantedBandwidthTemp);
  uint32_t GetGrantedBandwidthTemp (void);

  void SetBwSinceLastExpiry (uint32_t bwSinceLastExpiry);
  void UpdateBwSinceLastExpiry (uint32_t bwSinceLastExpiry);
  uint32_t GetBwSinceLastExpiry (void);

  void SetLastGrantTime (Time grantTime);
  Time GetLastGrantTime (void) const;

  void SetBacklogged (uint32_t backlogged);
  void IncreaseBacklogged (uint32_t backlogged);
  uint32_t GetBacklogged (void) const;

  void SetBackloggedTemp (uint32_t backloggedTemp);
  void IncreaseBackloggedTemp (uint32_t backloggedTemp);
  uint32_t GetBackloggedTemp (void) const;

private:
  uint32_t m_grantSize; // only used for UGS flow
  Time m_grantTimeStamp; // allocation (for data) for UGS flows and unicast poll (for bandwidth requests) for non-UGS flows
  Time m_dlTimeStamp; // time when this service flow's traffic was last sent

  // stats members
  uint32_t m_pktsSent;
  uint32_t m_pktsRcvd;

  uint32_t m_bytesSent;
  uint32_t m_bytesRcvd;

  uint32_t m_requestedBandwidth;
  uint32_t m_grantedBandwidth;
  uint32_t m_grantedBandwidthTemp; // Temporary variable used to sort list. Necessary to keep original order

  /* bandwidth granted since last expiry of minimum reserved traffic rate interval,
   /only for nrtPS, to make sure minimum reserved traffic rate is maintained */
  uint32_t m_bwSinceLastExpiry;
  Time m_lastGrantTime;
  int32_t m_backlogged;
  int32_t m_backloggedTemp;

};

} // namespace ns3

#endif /* SERVICE_FLOW_RECORD_H */
