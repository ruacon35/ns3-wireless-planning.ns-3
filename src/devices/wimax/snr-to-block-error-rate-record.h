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

#ifndef SNRTOBLOCKERRORRATERECORD_H_
#define SNRTOBLOCKERRORRATERECORD_H_
namespace ns3 {

class SNRToBlockErrorRateRecord
{
public:
  SNRToBlockErrorRateRecord (double snrValue, double bitErrorRate,
                             double BlockErrorRate, double sigma2, double I1, double I2);
  ~SNRToBlockErrorRateRecord (void);

  double GetSNRValue (void);
  double GetBitErrorRate (void);
  double GetBlockErrorRate (void);
  double GetSigma2 (void);
  double GetI1 (void);
  double GetI2 (void);
  SNRToBlockErrorRateRecord *
  Copy ();

  void SetSNRValue (double);
  void SetBitErrorRate (double);
  void SetBlockErrorRate (double);
  void SetI1 (double);
  void SetI2 (double);

private:
  double m_snrValue;
  double m_bitErrorRate;
  double m_blockErrorRate;
  double m_sigma2;
  double m_i1; // for the confidence interval
  double m_i2; // for the confidence interval
};

}

#endif /* SNRTOBLOCKERRORRATE_H_ */
