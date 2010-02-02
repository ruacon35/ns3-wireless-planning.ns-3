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

/*
 * The SendParams class defines the parameters with which Send() function of
 * a particular PHY is called. The sole purpose of this class is to allow
 * defining the pure virtual Send() function in the PHY base-class (WimaxPhy).
 * This class shall be sub-classed every time a new PHY is integrated (i.e.,
 * a new sub-class of WimaxPhy is created) which requires different or
 * additional parameters to call its Send() function. For example as it is
 * seen here, it has been sub-classed for the OFDM PHY layer since its Send()
 * function requires two additional parameters.
 */

#ifndef SEND_PARAMS_H
#define SEND_PARAMS_H

#include <stdint.h>

namespace ns3 {

class WimaxPhy;

class SendParams
{
public:
  SendParams ();
  virtual ~SendParams ();

private:
  virtual void Init (void) = 0;
};

} // namespace ns3

#endif /* SEND_PARAMS_H */

#ifndef SIMPLE_SEND_PARAMS_H
#define SIMPLE_SEND_PARAMS_H

#include <stdint.h>
#include "packet-burst.h"

namespace ns3 {

class SimpleSendParams : public SendParams
{
public:
  // defining modulationType as WimaxPhy::ModulationType did not work for some unknown reasons
  SimpleSendParams (Ptr<PacketBurst> burst, uint8_t modulationType);
  ~SimpleSendParams ();
  void Init (void);
  Ptr<PacketBurst> GetBurst () const
  {
    return m_burst;
  }

  uint8_t GetModulationType () const
  {
    return m_modulationType;
  }

private:
  Ptr<PacketBurst> m_burst;
  uint8_t m_modulationType;
};

} // namespace ns3

#endif /* SIMPLE_SEND_PARAMS_H */

#ifndef OFDM_SEND_PARAMS_H
#define OFDM_SEND_PARAMS_H

#include <stdint.h>
#include "packet-burst.h"

namespace ns3 {

class OfdmSendParams : public SendParams
{
public:
  // defining modulationType as WimaxPhy::ModulationType did not work for some unknown reasons
  OfdmSendParams (Ptr<PacketBurst> burst, uint8_t modulationType,
                  uint8_t direction, uint32_t symbolIndex);
  ~OfdmSendParams ();
  void Init (void);
  Ptr<PacketBurst> GetBurst () const
  {
    return m_burst;
  }

  uint8_t GetModulationType () const
  {
    return m_modulationType;
  }

  uint8_t GetDirection () const
  {
    return m_direction;
  }

  uint32_t GetSymbolIndex () const
  {
    return m_symbolIndex;
  }

private:
  Ptr<PacketBurst> m_burst;
  uint8_t m_modulationType;
  uint8_t m_direction;
  uint32_t m_symbolIndex;
};

} // namespace ns3

#endif /* OFDM_SEND_PARAMS_H */
