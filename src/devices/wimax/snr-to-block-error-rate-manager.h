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

#ifndef SNRTOBLOCKERRORRATEMANAGER_H_
#define SNRTOBLOCKERRORRATEMANAGER_H_

#include "ns3/snr-to-block-error-rate-record.h"
#include <vector>
#include "ns3/ptr.h"

namespace ns3 {

class SNRToBlockErrorRateManager
{
public:
  SNRToBlockErrorRateManager ();
  ~SNRToBlockErrorRateManager (void);
  void SetTraceFilePath (char *traceFilePath);
  std::string GetTraceFilePath (void);
  double GetBlockErrorRate (double SNR, uint8_t modulation);
  SNRToBlockErrorRateRecord *
  GetSNRToBlockErrorRateRecord (double SNR, uint8_t modulation);
  void LoadTraces (void);
  void LoadDefaultTraces (void);
  void ReLoadTraces (void);
  void ActivateLoss (bool loss);
  void ClearRecord (void);
private:
  double m_speed; // in m/s
  uint8_t m_activateLoss;
  char m_traceFilePath[1024];

  std::vector<SNRToBlockErrorRateRecord *> * m_recordModulation[7];

};
}

#endif /* SNRTOBLOCKERRORRATEMANAGER_H_ */
