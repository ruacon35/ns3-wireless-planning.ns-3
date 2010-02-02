/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2007,2008, 2009 INRIA, UDcast
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
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                              <amine.ismail@udcast.com>
 */

#include <list>
#include "wimax-channel.h"
#include "bvec.h"
#include "wimax-phy.h"
#include "ns3/propagation-loss-model.h"

#ifndef SIMPLEOFDMSENDPARAM_H_
#define SIMPLEOFDMSENDPARAM_H_

namespace ns3 {
class simpleOfdmSendParam
{
public:
  simpleOfdmSendParam (void);
  simpleOfdmSendParam (const bvec &fecBlock, uint32_t burstSize,
                       bool isFirstBlock, uint64_t Frequency,
                       WimaxPhy::ModulationType modulationType, uint8_t direction,
                       uint32_t symbolIndex, double rxPowerDbm);
  ~simpleOfdmSendParam (void);
  void SetFecBlock (const bvec &fecBlock);
  void SetBurstSize (uint32_t burstSize);
  void SetIsFirstBlock (bool isFirstBlock);
  void SetFrequency (uint64_t Frequency);
  void SetModulationType (WimaxPhy::ModulationType modulationType);
  void SetDirection (uint8_t direction);
  void SetSymbolIndex (uint32_t symbolIndex);
  void SetRxPowerDbm (double rxPowerDbm);

  bvec GetFecBlock (void);
  uint32_t GetBurstSize (void);
  bool GetIsFirstBlock (void);
  uint64_t GetFrequency (void);
  WimaxPhy::ModulationType GetModulationType (void);
  uint8_t GetDirection (void);
  uint32_t GetSymbolIndex (void);
  double GetRxPowerDbm (void);

private:
  bvec m_fecBlock;
  uint32_t m_burstSize;
  bool m_isFirstBlock;
  uint64_t m_frequency;
  WimaxPhy::ModulationType m_modulationType;
  uint8_t m_direction;
  uint32_t m_symbolIndex;
  double m_rxPowerDbm;

};
} // namespace ns3
#endif /* SIMPLEOFDMSENDPARAM_H_ */
