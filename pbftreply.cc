//  Network Topology
//
//       10.1.1.0
// n0 -------------- n1--------n2  
//    LAN 10.1.2.0   
//                    ================
//                      LAN 10.1.2.0
//   L1 -> 5Mbps, 2ms
//   L2 -> 10Mbps, 5ms
//  n0 - client, n2- server
//


#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("PBFT reply");

int 
main (int argc, char *argv[])
{
	Time::SetResolution (Time::NS);
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

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
	
	ApplicationContainer serverApps = echoServer.Install (nodes.Get (2));
	ApplicationContainer serverApps1 = echoServer.Install (nodes.Get (3));
	ApplicationContainer serverApps2 = echoServer.Install (nodes.Get (4));

	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (10.0));
	
	UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
	echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
	echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient.SetAttribute ("PacketSize",UintegerValue (1024));

	ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
	clientApps.Start (Seconds (2.0));
	clientApps.Stop (Seconds (10.0));

	AnimationInterface anim ("FBFTRequest.xml");
	anim.SetConstantPosition(nodes.Get(0), 10.0, 10.0);
	anim.SetConstantPosition(nodes.Get(1), 20.0,20.0);
	anim.SetConstantPosition(nodes.Get(2), 30.0, 30.0);
	anim.SetConstantPosition(nodes.Get(3), 40.0, 40.0);
	anim.SetConstantPosition(nodes.Get(4), 50.0, 50.0);

	Simulator::Run();
	Simulator::Destroy();
	
	return 0;





}
