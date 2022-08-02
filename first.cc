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
#include "ns3/netanim-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;  
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps")); //tells the PointToPointHelper object to use the value 5Mbps as the DataRate 
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms")); //2ms as the value of transmission delay of every point to point channel

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);//Two nodes,each with an installed point-to-point net device and a single point-to-point channel between them.
  //Both devices will be configured to transmit data at five megabits per second over the channel which has a two millisecond transmission delay.

  InternetStackHelper stack; // install Internet Stack(TCP, UDP, IP. etc) on each of the nodes in the node container
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0"); //IP address and net mask (10.1.1.1/10.1.1.2)

  Ipv4InterfaceContainer interfaces = address.Assign (devices); //makes the association between an IP address and a device using Ipv4Interface object
  // Now, we have a point-to-point network built with stacks installed and IP addresses assigned. 

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));// the udp echo server application to start (enable itself) at one second into the simulation and it lasts 10 seconds
                                

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1)); //tells the client maximum number of packets allowed to send during simulation
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0))); // tells how long to wait between packets 
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));//tells how large the packet payloads should be
  //tells client to send one 1024-byte packet.


  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));//starts the client one second after the server is enabled
  clientApps.Stop (Seconds (10.0));


  AnimationInterface anim ("anim1.xml");
  anim.SetConstantPosition(nodes.Get(0), 1.0, 2.0);
  anim.SetConstantPosition(nodes.Get(1), 2.0, 3.0);
   
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
  // the client has sent one 1024 byte packet to the Echo server on 10.1.1.2
  // the server has received the packet from the client on 10.1.1.1
