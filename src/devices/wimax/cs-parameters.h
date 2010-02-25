/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2009 INRIA, UDcast
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
 *                              <amine.ismail@udcast.com>
 *
 */

#include "ipcs-classifier-record.h"
#include "wimax-tlv.h"
#ifndef CSPARAMETERS_H_
#define CSPARAMETERS_H_
namespace ns3 {

class CsParameters
{
public:
  enum Action
  {
    ADD = 0,
    REPLACE = 1,
    DELETE = 2
  };
  CsParameters ();
  ~CsParameters ();
  CsParameters (Tlv tlv);
  CsParameters (enum Action classifierDscAction, IpcsClassifierRecord classifier);
  void SetClassifierDscAction (enum Action action);
  void SetPacketClassifierRule (IpcsClassifierRecord packetClassifierRule);
  enum Action GetClassifierDscAction (void) const;
  IpcsClassifierRecord GetPacketClassifierRule (void) const;
  Tlv ToTlv (void) const;

private:
  enum Action m_classifierDscAction;
  IpcsClassifierRecord m_packetClassifierRule;
};

}
#endif /* CSPARAMETERS_H_ */
