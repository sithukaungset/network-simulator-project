/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 
using namespace ns3;



NS_LOG_COMPONENT_DEFINE (“p2p”);
p2p::p2p (uint32_t numSpokes,
PointToPointHelper p2pHelper)
{
m_hub.Create (1);
m_spokes.Create (numSpokes);
for (uint32_t i = 0; i < m_spokes.GetN (); ++i)
{
NetDeviceContainer nd = p2pHelper.Install (m_hub.Get (0), m_spokes.Get (i));
m_hubDevices.Add (nd.Get (0));
m_spokeDevices.Add (nd.Get (1));
}
}
p2p::~p2p ()
{
}

Ptr
p2p::GetHub () const
{
return m_hub.Get (0);
}
Ptr
p2p::GetSpokeNode (uint32_t i) const
{
return m_spokes.Get (i);
}
Ipv4Address
p2p::GetHubIpv4Address (uint32_t i) const
{
return m_hubInterfaces.GetAddress (i);
}

Ipv4Address
p2p::GetSpokeIpv4Address (uint32_t i) const
{
return m_spokeInterfaces.GetAddress (i);
}

Ipv6Address
p2p::GetHubIpv6Address (uint32_t i) const
{
return m_hubInterfaces6.GetAddress (i, 1);
}

Ipv6Address
p2p::GetSpokeIpv6Address (uint32_t i) const
{
return m_spokeInterfaces6.GetAddress (i, 1);
}

uint32_t
p2p::SpokeCount () const
{
return m_spokes.GetN ();
}

void
p2p::InstallStack (InternetStackHelper stack)
{
stack.Install (m_hub);
stack.Install (m_spokes);
}

void
p2p::AssignIpv4Addresses (Ipv4AddressHelper address)
{
for (uint32_t i = 0; i < m_spokes.GetN (); ++i)
{
m_hubInterfaces.Add (address.Assign (m_hubDevices.Get (i)));
m_spokeInterfaces.Add (address.Assign (m_spokeDevices.Get (i)));
address.NewNetwork ();
}
}

void
p2p::AssignIpv6Addresses (Ipv6Address addrBase, Ipv6Prefix prefix)
{
Ipv6AddressGenerator::Init (addrBase, prefix);
Ipv6Address v6network;
Ipv6AddressHelper addressHelper;

for (uint32_t i = 0; i < m_spokes.GetN (); ++i)
{
v6network = Ipv6AddressGenerator::GetNetwork (prefix);
addressHelper.SetBase (v6network, prefix);

Ipv6InterfaceContainer ic = addressHelper.Assign (m_hubDevices.Get (i));
m_hubInterfaces6.Add (ic);
ic = addressHelper.Assign (m_spokeDevices.Get (i));
m_spokeInterfaces6.Add (ic);

Ipv6AddressGenerator::NextNetwork (prefix);
}
}