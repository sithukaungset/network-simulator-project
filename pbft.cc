//  Network Topology
//
//       10.1.1.0
// n0 -------------- n1--------n2 , n3,  n4
//    LAN 10.1.2.0   
//                    ================
//                      LAN 10.1.2.0
//   L1 -> 5Mbps, 2ms
//   L2 -> 10Mbps, 5ms
//  n0 - client, n2- server, n3- server. n4 -server
//  


#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Script tutorial");

int 
main (int argc, char *argv[])
{
	Time::SetResolution (Time::NS);
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_ALL);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_ALL);

	NodeContainer nodes;
	nodes.Create(5);
	
	InternetStackHelper stack;
	stack.Install (nodes);

	// channel1 between client and leader
	PointToPointHelper p2p1;
	p2p1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	//DataRate: The transmission data rate to be provided to devices connected to the channel
	// Delay: Transmission delay throught the channel
	p2p1.SetChannelAttribute("Delay", StringValue ("1ms"));

	// channel2 between leader node and member node 1 
	PointToPointHelper p2p2;
	p2p2.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
	p2p2.SetChannelAttribute ("Delay", StringValue ("5ms"));

	// channel3 between leader node and member node 2
	PointToPointHelper p2p3;
	p2p2.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
	p2p2.SetChannelAttribute ("Delay", StringValue ("5ms"));

	// channel4 between leader node and member node 3
	PointToPointHelper p2p4;
	p2p2.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
	p2p2.SetChannelAttribute ("Delay", StringValue ("5ms"));

	// client and leader node
	Ipv4AddressHelper address;
	address.SetBase ("10.1.1.0","255.255.255.0");
	NetDeviceContainer devices;
	devices = p2p1.Install (nodes.Get (0), nodes.Get (1));
	Ipv4InterfaceContainer interfaces = address.Assign (devices);

	// leader node and member node1
	devices = p2p2.Install (nodes.Get (1), nodes.Get (2));
	address.SetBase ("10.1.2.0","255.255.255.0"); 
	interfaces = address.Assign (devices);

	// leader node and member node2
	devices = p2p3.Install (nodes.Get (1), nodes.Get (3));
	address.SetBase ("10.1.3.0","255.255.255.0"); 
	interfaces = address.Assign (devices);

	// leader node and member node3
	devices = p2p4.Install (nodes.Get (1), nodes.Get (4));
	address.SetBase ("10.1.4.0","255.255.255.0"); 
	interfaces = address.Assign (devices);


	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
	
	UdpEchoServerHelper echoServer (9);
	UdpEchoServerHelper echoServer1 (9);
	UdpEchoServerHelper echoServer2 (9);
	
	ApplicationContainer serverApps = echoServer.Install (nodes.Get (2));
	ApplicationContainer serverApps1 = echoServer1.Install (nodes.Get (3));
	ApplicationContainer serverApps2 = echoServer2.Install (nodes.Get (4));

	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (10.0));

	serverApps1.Start (Seconds (1.0));
	serverApps1.Stop (Seconds (10.0));

	serverApps2.Start (Seconds (1.0));
	serverApps2.Stop (Seconds (10.0));
	
	UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
	echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
	echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient.SetAttribute ("PacketSize",UintegerValue (1024));

	

	ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
	clientApps.Start (Seconds (2.0));
	clientApps.Stop (Seconds (10.0));

	UdpEchoClientHelper echoClient1 (interfaces.GetAddress (1), 9);
	echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
	echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient1.SetAttribute ("PacketSize",UintegerValue (1024));

	ApplicationContainer clientApps1 = echoClient1.Install (nodes.Get (1));
	clientApps1.Start (Seconds (2.0));
	clientApps1.Stop (Seconds (10.0));

	AnimationInterface anim ("FBFTRequest.xml");
	// anim.SetConstantPosition(nodes.Get(0), 10.0, 10.0);
	// anim.SetConstantPosition(nodes.Get(1), 20.0,20.0);
	// anim.SetConstantPosition(nodes.Get(2), 30.0, 40.0);
	// anim.SetConstantPosition(nodes.Get(3), 40.0, 50.0);
	// anim.SetConstantPosition(nodes.Get(4), 50.0, 60.0);

	Simulator::Run();
	Simulator::Destroy();
	
	return 0;





}
