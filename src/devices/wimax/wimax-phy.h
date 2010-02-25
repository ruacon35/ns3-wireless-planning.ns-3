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

#ifndef WIMAX_PHY_H
#define WIMAX_PHY_H

#include <stdint.h>
#include <list>
#include "ns3/callback.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "bvec.h"
#include "send-params.h"
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/traced-callback.h"

namespace ns3 {

class WimaxChannel;
class WimaxNetDevice;
class NetDevice;
class Packet;

class WimaxPhy : public Object
{
public:
  enum ModulationType // Table 356 and 362
  {
    MODULATION_TYPE_BPSK_12, MODULATION_TYPE_QPSK_12, MODULATION_TYPE_QPSK_34, MODULATION_TYPE_QAM16_12,
    MODULATION_TYPE_QAM16_34, MODULATION_TYPE_QAM64_23, MODULATION_TYPE_QAM64_34
  };

  enum PhyState
  {
    PHY_STATE_IDLE, PHY_STATE_SCANNING, PHY_STATE_TX, PHY_STATE_RX
  };

  enum PhyType
  {
    SimpleWimaxPhy, simpleOfdmWimaxPhy
  };

  static TypeId GetTypeId (void);
  WimaxPhy (void);
  virtual ~WimaxPhy (void);

  void Attach (Ptr<WimaxChannel> channel);
  Ptr<WimaxChannel> GetChannel (void) const;
  void SetDevice (Ptr<WimaxNetDevice> device);
  Ptr<NetDevice> GetDevice (void) const;

  void SetReceiveCallback (Callback<void, Ptr<const PacketBurst> > callback);
  Callback<void, Ptr<const PacketBurst> > GetReceiveCallback (void) const;

  virtual void Send (SendParams *params) = 0;
  virtual PhyType GetPhyType (void) const = 0;

  void SetDuplex (uint64_t rxFrequency, uint64_t txFrequency);
  void SetSimplex (uint64_t frequency);
  uint64_t GetRxFrequency (void) const;
  uint64_t GetTxFrequency (void) const;
  uint64_t GetScanningFrequency (void) const;
  void SetState (PhyState state);
  PhyState GetState (void) const;

  void SetNrCarriers (uint8_t nrCarriers);
  uint8_t GetNrCarriers (void) const;
  void SetFrameDuration (Time frameDuration);
  Time GetFrameDurationSec (void) const;
  Time GetFrameDuration (void) const;
  void SetFrequency (uint32_t frequency);
  uint32_t GetFrequency (void) const;
  void SetChannelBandwidth (uint32_t channelBandwidth);
  uint32_t GetChannelBandwidth (void) const;
  uint16_t GetNfft (void) const;
  double GetSamplingFactor (void) const;
  double GetSamplingFrequency (void) const;
  void SetPsDuration (Time psDuration);
  Time GetPsDuration (void) const;
  void SetSymbolDuration (Time symbolDuration);
  Time GetSymbolDuration (void) const;
  double GetGValue (void) const;
  void SetPsPerSymbol (uint16_t psPerSymbol);
  uint16_t GetPsPerSymbol (void) const;
  void SetPsPerFrame (uint16_t psPerFrame);
  uint16_t GetPsPerFrame (void) const;
  void SetSymbolsPerFrame (uint32_t symbolsPerFrame);
  uint32_t GetSymbolsPerFrame (void) const;

  bool IsDuplex (void) const;
  EventId GetChnlSrchTimeoutEvent (void) const;
  void SetScanningCallback (void) const;

  // calls back with channel frequency.
  void StartScanning (uint64_t frequency, Time timeout, Callback<void, bool, uint64_t> callback);
  void EndScanning (void);

  void GetModulationFecParams (ModulationType modulationType, uint8_t &bitsPerSymbol, double &fecCode) const;
  void SetDataRates (void);
  uint32_t GetDataRate (ModulationType modulationType) const;
  Time GetTransmissionTime (uint32_t size, ModulationType modulationType) const;
  uint64_t GetNrSymbols (uint32_t size, ModulationType modulationType) const;
  uint64_t GetNrBytes (uint32_t symbols, ModulationType modulationType) const;
  uint16_t GetTtg (void) const;
  uint16_t GetRtg (void) const;

  uint8_t GetFrameDurationCode (void) const;
  Time GetFrameDuration (uint8_t frameDurationCode) const;
  void SetPhyParameters (void);
  virtual void DoDispose (void);
  virtual Ptr<Object> GetMobility (void);
  virtual void SetMobility (Ptr<Object> mobility);

private:
  virtual Time DoGetTransmissionTime (uint32_t size, ModulationType modulationType) const = 0;
  virtual void DoAttach (Ptr<WimaxChannel> channel) = 0;
  virtual void DoSetDataRates (void) = 0;
  virtual uint32_t DoGetDataRate (ModulationType modulationType) const = 0;
  virtual uint64_t DoGetNrSymbols (uint32_t size, ModulationType modulationType) const = 0;
  virtual uint64_t DoGetNrBytes (uint32_t symbols, ModulationType modulationType) const = 0;
  virtual uint16_t DoGetTtg (void) const = 0;
  virtual uint16_t DoGetRtg (void) const = 0;

  virtual uint8_t DoGetFrameDurationCode (void) const = 0;
  virtual Time DoGetFrameDuration (uint8_t frameDurationCode) const = 0;
  virtual void DoSetPhyParameters (void) = 0;
  virtual double DoGetSamplingFactor (void) const = 0;
  virtual uint16_t DoGetNfft (void) const = 0;
  virtual double DoGetSamplingFrequency (void) const = 0;
  virtual double DoGetGValue (void) const = 0;

  Ptr<WimaxNetDevice> m_device;
  Ptr<WimaxChannel> m_channel;

  uint64_t m_txFrequency;
  uint64_t m_rxFrequency;
  uint64_t m_scanningFrequency;
  EventId m_dlChnlSrchTimeoutEvent;
  bool m_duplex;
  PhyState m_state;

  Callback<void, Ptr<const PacketBurst> > m_rxCallback;
  Callback<void, bool, uint64_t> m_scanningCallback;

  uint8_t m_nrCarriers;
  Time m_frameDuration; // in seconds
  uint32_t m_frequency; // in KHz
  uint32_t m_channelBandwidth; // in Hz
  Time m_psDuration; // in seconds
  Time m_symbolDuration; // in seconds
  uint16_t m_psPerSymbol;
  uint16_t m_psPerFrame;
  uint32_t m_symbolsPerFrame;
  Ptr<Object> m_mobility;
};

} // namespace ns3

#endif /* WIMAX_PHY_H */
