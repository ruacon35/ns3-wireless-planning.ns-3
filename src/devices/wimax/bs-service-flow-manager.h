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
 *         Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */

#ifndef BS_SERVICE_FLOW_MANAGER_H
#define BS_SERVICE_FLOW_MANAGER_H

#include <stdint.h>
#include "ns3/event-id.h"
#include "mac-messages.h"
#include "ns3/buffer.h"
#include "service-flow-manager.h"
#include "bs-net-device.h"

namespace ns3 {

class Packet;
class ServiceFlow;
class WimaxNetDevice;
class SSRecord;
class WimaxConnection;
class BaseStationNetDevice;

class BsServiceFlowManager : public ServiceFlowManager
{
public:
  enum ConfirmationCode // as per Table 384 (not all codes implemented)
  {
    CONFIRMATION_CODE_SUCCESS, CONFIRMATION_CODE_REJECT
  };

  BsServiceFlowManager (Ptr<BaseStationNetDevice> device);
  ~BsServiceFlowManager (void);
  void DoDispose (void);

  void defineServiceFlow (void);
  void AddServiceFlow (ServiceFlow *serviceFlow);
  ServiceFlow* GetServiceFlow (uint32_t sfid) const;
  ServiceFlow* GetServiceFlow (Cid cid) const;
  std::vector<ServiceFlow*> GetServiceFlows (ServiceFlow::SchedulingType schedulingType) const;
  void SetMaxDsaRspRetries (uint8_t maxDsaRspRetries);

  uint8_t GetMaxDsaRspRetries (void) const;
  EventId GetDsaAckTimeoutEvent (void) const;

  void AllocateServiceFlows (const DsaReq &dsaReq, Cid cid);
  DsaRsp CreateDsaRsp (const ServiceFlow *serviceFlow, uint16_t transactionId);
  void AddMulticastServiceFlow (ServiceFlow sf, enum WimaxPhy::ModulationType modulation);
  void ScheduleDsaRsp (ServiceFlow *serviceFlow, Cid cid);
  void ProcessDsaAck (const DsaAck &dsaAck, Cid cid);
  ServiceFlow* ProcessDsaReq (const DsaReq &dsaReq, Cid cid);

private:
  Ptr<WimaxNetDevice> m_device;
  uint32_t m_sfidIndex;
  uint8_t m_maxDsaRspRetries;
  EventId m_dsaAckTimeoutEvent;
  Cid m_inuseScheduleDsaRspCid;
};

} // namespace ns3

#endif /* BS_SERVICE_FLOW_MANAGER_H */
