/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008 INRIA, UDcast
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
 *         Mohamed Amine Ismail <Amine.Ismail@sophia.inria.fr>
 *                              <Amine.Ismail@UDcast.com>
 */

#include "wimax-mac-queue.h"
#include "ns3/packet.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/uinteger.h"
#include "ns3/simulator.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED ( WimaxMacQueue);

WimaxMacQueue::QueueElement::QueueElement (void)
  : m_packet (Create<Packet> ()),
    m_hdrType (MacHeaderType ()),
    m_hdr (
      GenericMacHeader ()),
    m_timeStamp (Seconds (0))
{
}

WimaxMacQueue::QueueElement::QueueElement (Ptr<Packet> packet,
                                           const MacHeaderType &hdrType, const GenericMacHeader &hdr, Time timeStamp)
  : m_packet (packet),
    m_hdrType (hdrType),
    m_hdr (hdr),
    m_timeStamp (timeStamp)
{
}

uint32_t
WimaxMacQueue::QueueElement::GetSize (void) const
{
  uint32_t size = m_packet->GetSize () + m_hdrType.GetSerializedSize ();

  /*check because may be it is a bandwidth request packet (in which case a Bandwidth Request Header
   has already been added to the packet) in which case Generic MAC Header will not be added to it.
   this will only happen in the case of SS as only SS sends the bandwidth request packet. */
  if (m_hdrType.GetType () == MacHeaderType::HEADER_TYPE_GENERIC)
    {
      size += m_hdr.GetSerializedSize ();
    }

  return size;
}

TypeId
WimaxMacQueue::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WimaxMacQueue")
    .SetParent<Object> ()
    .AddAttribute (
      "MaxSize",
      "Maximum size",
      UintegerValue (1024),
      MakeUintegerAccessor (&WimaxMacQueue::GetMaxSize,
                            &WimaxMacQueue::SetMaxSize),
      MakeUintegerChecker<uint32_t> ())
    .AddTraceSource ("Enqueue",
                     "Enqueue trace",
                     MakeTraceSourceAccessor (&WimaxMacQueue::m_traceEnqueue))
    .AddTraceSource ("Dequeue",
                     "Dequeue trace",
                     MakeTraceSourceAccessor (&WimaxMacQueue::m_traceDequeue))
    .AddTraceSource ("Drop",
                     "Drop trace",
                     MakeTraceSourceAccessor (&WimaxMacQueue::m_traceDrop))
  ;
  return tid;
}

WimaxMacQueue::WimaxMacQueue (void)
  : m_maxSize (0),
    m_bytes (0),
    m_nrDataPackets (0),
    m_nrRequestPackets (0)
{
}

WimaxMacQueue::WimaxMacQueue (uint32_t maxSize)
  : m_maxSize (maxSize),
    m_bytes (0),
    m_nrDataPackets (0),
    m_nrRequestPackets (0)
{
}

WimaxMacQueue::~WimaxMacQueue (void)
{
}

void
WimaxMacQueue::SetMaxSize (uint32_t maxSize)
{
  m_maxSize = maxSize;
}

uint32_t
WimaxMacQueue::GetMaxSize (void) const
{
  return m_maxSize;
}

bool
WimaxMacQueue::Enqueue (Ptr<Packet> packet, const MacHeaderType &hdrType,
                        const GenericMacHeader &hdr)
{

  if (m_queue.size () == m_maxSize)
    {

      m_traceDrop (packet);
      return false;
    }

  m_traceEnqueue (packet);
  QueueElement element (packet, hdrType, hdr, Simulator::Now ());
  m_queue.push_back (element);

  if (hdrType.GetType () == MacHeaderType::HEADER_TYPE_GENERIC)
    {
      m_nrDataPackets++;
    }
  else
    {
      m_nrRequestPackets++;
    }

  m_bytes += element.GetSize ();
  return true;
}

Ptr<Packet>
WimaxMacQueue::Dequeue (MacHeaderType::HeaderType packetType)
{
  if (!IsEmpty ())
    {
      QueueElement element = Front (packetType);
      Pop (packetType);

      if (element.m_hdrType.GetType () == MacHeaderType::HEADER_TYPE_GENERIC)
        {
          m_nrDataPackets--;
          NS_ASSERT_MSG (m_nrDataPackets >= 0,
                         "Can not enqueue more packets: no space left in the queue");
        }
      else
        {
          m_nrRequestPackets--;
          NS_ASSERT_MSG (m_nrRequestPackets >= 0,
                         "Can not enqueue more packets: no space left in the queue");
        }

      m_bytes -= element.GetSize ();
      Ptr<Packet> packet = element.m_packet;

      /*check because may be it is a bandwidth request packet (in which case a Bandwidth Request Header
       has already been added to the packet) in which case Generic MAC Header will not be added to it.
       this will only happen in the case of SS as only SS sends the bandwidth request packet. */
      if (element.m_hdrType.GetType () == MacHeaderType::HEADER_TYPE_GENERIC)
        {
          packet->AddHeader (element.m_hdr);
        }
      m_traceDequeue (packet);
      return packet;
    }

  return 0;
}

Ptr<Packet>
WimaxMacQueue::Peek (GenericMacHeader &hdr) const
{
  if (!IsEmpty ())
    {
      QueueElement element = m_queue.front ();
      hdr = element.m_hdr;
      Ptr<Packet> packet = element.m_packet->Copy ();

      // this function must not be used by SS as it may be then a bandwidth request header
      packet->AddHeader (element.m_hdr);
      return packet;
    }

  return 0;
}

Ptr<Packet>
WimaxMacQueue::Peek (GenericMacHeader &hdr, Time &timeStamp) const
{
  if (!IsEmpty ())
    {
      QueueElement element = m_queue.front ();
      hdr = element.m_hdr;
      timeStamp = element.m_timeStamp;
      Ptr<Packet> packet = element.m_packet->Copy ();

      // this function must not be used for by SS as it may be then a bandwidth request header
      packet->AddHeader (element.m_hdr);
      return packet;
    }

  return 0;
}

Ptr<Packet>
WimaxMacQueue::Peek (MacHeaderType::HeaderType packetType) const
{
  if (!IsEmpty ())
    {
      QueueElement element = Front (packetType);
      Ptr<Packet> packet = element.m_packet->Copy ();

      /*check because may be it is a bandwidth request packet (in which case a Bandwidth Request Header
       has already been added to the packet) in which case Generic MAC Header will not be added to it.
       this will only happen in the case of SS as only SS sends the bandwidth request packet. */
      if (element.m_hdrType.GetType () == MacHeaderType::HEADER_TYPE_GENERIC)
        {
          packet->AddHeader (element.m_hdr);
        }
      return packet;
    }

  return 0;
}

Ptr<Packet>
WimaxMacQueue::Peek (MacHeaderType::HeaderType packetType,
                     Time &timeStamp) const
{
  if (!IsEmpty ())
    {
      QueueElement element = Front (packetType);
      timeStamp = element.m_timeStamp;
      Ptr<Packet> packet = element.m_packet->Copy ();

      /*check because may be it is a bandwidth request packet (in which case a Bandwidth Request Header
       has already been added to the packet) in which case Generic MAC Header will not be added to it.
       this will only happen in the case of SS as only SS sends the bandwidth request packet. */
      if (element.m_hdrType.GetType () == MacHeaderType::HEADER_TYPE_GENERIC)
        {
          packet->AddHeader (element.m_hdr);
        }
      return packet;
    }

  return 0;
}

uint32_t
WimaxMacQueue::GetSize (void) const
{
  return m_queue.size ();
}

uint32_t
WimaxMacQueue::GetNBytes (void) const
{
  return m_bytes;
}

WimaxMacQueue::QueueElement
WimaxMacQueue::Front (MacHeaderType::HeaderType packetType) const
{
  QueueElement element;

  for (std::deque<QueueElement>::const_iterator iter = m_queue.begin (); iter
       != m_queue.end (); ++iter)
    {
      element = *iter;
      if (element.m_hdrType.GetType () == packetType)
        {
          break;
        }
    }

  return element;
}

void
WimaxMacQueue::Pop (MacHeaderType::HeaderType packetType)
{
  QueueElement element;

  for (std::deque<QueueElement>::iterator iter = m_queue.begin (); iter
       != m_queue.end (); ++iter)
    {
      element = *iter;
      if (element.m_hdrType.GetType () == packetType)
        {
          m_queue.erase (iter);
          break;
        }
    }
}

bool
WimaxMacQueue::IsEmpty (void) const
{
  return m_queue.empty ();
}

bool
WimaxMacQueue::IsEmpty (MacHeaderType::HeaderType packetType) const
{
  if (packetType == MacHeaderType::HEADER_TYPE_GENERIC)
    {
      return m_nrDataPackets == 0;
    }
  else
    {
      return m_nrRequestPackets == 0;
    }

  return true;
}

const WimaxMacQueue::PacketQueue &
WimaxMacQueue::GetPacketQueue (void) const
{
  return m_queue;
}
} // namespace ns3
