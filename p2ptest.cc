/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
 * Author: Pengfei Cui
 * Date: June 19, 2013
 * 
 * Description: 
 *	
 *	Step1: Verify the Friis channel model can set frequency
 *	Step2: Set up 2 band at the same time
 *
 * Notes:
 *	Config::SetDefault( ) can change the frequency
 *	YanswifiChannelHelper.AddPropagationLossModel could set the propagation loss model and config the frequency
 *
 * Questions:
 *		Could 2 frequency co-exist in one script?
 *
 *
 *	In process
 *		Put the networks in different wifi channel 
 *		Build networks have 2 bands
 *		Support multiband simultaneous
 *		Output data properly
 *
 *	Status:
 *
 *		Modify the experiment function to access 2 channel propagation wavelength
 *		2 networks can works simultaneous in P2P networks
 *
 *  
 */



#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("P2P Test Script");



int main (int argc, char*argv[])
{
	LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);


	NodeContainer c2;
	c2.Create (2);


	NodeContainer c5;
	c5.Create (2);


	PointToPointHelper p2p2,p2p5;


	p2p2.SetDeviceAttribute ("DataRate", StringValue ("6Mbps"));
	p2p2.SetChannelAttribute ("Delay", StringValue ("2ms"));

	p2p5.SetDeviceAttribute ("DataRate", StringValue ("6Mbps"));
	p2p5.SetChannelAttribute ("Delay", StringValue ("2ms"));


	p2p2.SetChannelAttribute ("Delay", StringValue ("2ms"));
	p2p5.SetChannelAttribute ("Delay", StringValue ("2ms"));

	NetDeviceContainer devices2;
	devices2 = p2p2.Install (c2);


	NetDeviceContainer devices5;
	devices5 = p2p5.Install (c5);

	InternetStackHelper stack2;
	stack2.Install (c2);

	InternetStackHelper stack5;
	stack5.Install (c5);



	Ipv4AddressHelper address2;
	address2.SetBase ("10.1.1.0", "255.255.255.0");

	Ipv4AddressHelper address5;
	address5.SetBase ("10.1.2.0", "255.255.255.0");

	Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);
	Ipv4InterfaceContainer interfaces5 = address5.Assign (devices5);


	UdpEchoServerHelper echoServer2 (9);
	UdpEchoServerHelper echoServer5 (10);

	ApplicationContainer serverApp2 = echoServer2.Install (c2.Get(1));
	ApplicationContainer serverApp5 = echoServer5.Install (c5.Get(1));


	serverApp2.Start (Seconds (1.0));
	serverApp2.Stop (Seconds (10.0));

	serverApp5.Start (Seconds (1.0));
	serverApp5.Stop (Seconds (10.0));

	UdpEchoClientHelper echoClient2 (interfaces2.GetAddress (1), 9);
	UdpEchoClientHelper echoClient5 (interfaces5.GetAddress (1), 10);
	
	
	echoClient2.SetAttribute ("MaxPackets", UintegerValue (1));
	echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));
	
	
	echoClient5.SetAttribute ("MaxPackets", UintegerValue (1));
	echoClient5.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient5.SetAttribute ("PacketSize", UintegerValue (1024));


	ApplicationContainer clientApps2 = echoClient2.Install (c2.Get (0));
	clientApps2.Start (Seconds (2.0));
	clientApps2.Stop (Seconds (10.0));

	

	ApplicationContainer clientApps5 = echoClient5.Install (c5.Get (0));
	clientApps5.Start (Seconds (5.0));
	clientApps5.Stop (Seconds (10.0));

	Simulator::Run();

	Simulator::Destroy ();
	return 0;







}


