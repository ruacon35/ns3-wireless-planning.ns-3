/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 INRIA, UDcast
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
 *  Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */

#ifndef SS_SERVICE_FLOW_MANAGER_H
#define SS_SERVICE_FLOW_MANAGER_H

#include <stdint.h>
#include "ns3/event-id.h"
#include "mac-messages.h"
#include "ns3/buffer.h"
#include "service-flow-manager.h"
#include "ss-net-device.h"

namespace ns3 {

class Packet;
class ServiceFlow;
class WimaxNetDevice;
class WimaxConnection;
class SubscriberStationNetDevice;

class SsServiceFlowManager : public ServiceFlowManager
{
public:
  enum ConfirmationCode // as per Table 384 (not all codes implemented)
  {
    CONFIRMATION_CODE_SUCCESS, CONFIRMATION_CODE_REJECT
  };

  SsServiceFlowManager (Ptr<SubscriberStationNetDevice> device);

  ~SsServiceFlowManager (void);
  void DoDispose (void);

  void AddServiceFlow (ServiceFlow *serviceFlow);
  void AddServiceFlow (ServiceFlow serviceFlow);
  void SetMaxDsaReqRetries (uint8_t maxDsaReqRetries);

  uint8_t GetMaxDsaReqRetries (void) const;
  uint8_t GetMaxDsaRspRetries (void) const;
  EventId GetDsaRspTimeoutEvent (void) const;
  EventId GetDsaAckTimeoutEvent (void) const;

  void AllocateServiceFlows (const DsaReq &dsaReq, Cid cid);

  void InitiateServiceFlows (void);

  DsaReq CreateDsaReq (const ServiceFlow *serviceFlow);

  Ptr<Packet> CreateDsaAck (void);

  void ScheduleDsaReq (const ServiceFlow *serviceFlow);

  void ProcessDsaRsp (const DsaRsp &dsaRsp);


private:
  Ptr<SubscriberStationNetDevice> m_device;

  uint32_t m_sfidIndex;

  uint8_t m_maxDsaReqRetries;

  EventId m_dsaRspTimeoutEvent;
  EventId m_dsaAckTimeoutEvent;

  DsaReq m_dsaReq;
  DsaAck m_dsaAck;

  uint16_t m_currentTransactionId;
  uint16_t m_transactionIdIndex;
  uint8_t m_dsaReqRetries;

  // pointer to the service flow currently being configured
  ServiceFlow *m_pendingServiceFlow;


};

} // namespace ns3

#endif /* SS_SERVICE_FLOW_MANAGER_H */
