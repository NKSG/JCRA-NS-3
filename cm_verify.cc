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
 * Author: Pengfei Cui<bitcpf@gmail.com>
 * Date: June 18, 2013
 * Discription: Verify the channel model has multiband capacity
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/tools-module.h"
#include "ns3/wifi-module.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/yans-wifi-helper.h"

#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("VerifyFriimodelPropagation");


int main (int argc, char *argv[])
{

	LogComponentEnable ("UdpEchoClientApplication",LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication",LOG_LEVEL_INFO);


	//Output data
	Gnuplot2dDataset dataset;

	// Define frequency parameter
	// wavelength at 2.4 GHz is 0.125m
	Config::SetDefault ("ns3::FriisPropagationLossModel::Lambda", DoubleValue (0.125));                                              
	Config::SetDefault ("ns3::FriisPropagationLossModel::SystemLoss", DoubleValue (1.0));
	// Select a reference transmit power
	//Pt = 10^(17.0206/10)/10^3 = .05035702 W
//	double txPowerW = 0.05035702;
//	double txPowerdBm = 10 * log10 (txPowerW) + 30;

	NodeContainer friitest;
	friitest.Create (2);

	WifiHelper wifi;//= WifiHelper::Default ();




	PacketSocketHelper packetSocket;
	packetSocket.Install(friitest);

	YansWifiPhyHelper phy; //modify for a function
	YansWifiChannelHelper wifichannel;

	wifichannel.AddPropagationLoss("ns3::FriisPropagationLossModel");
	phy.SetChannel (wifichannel.Create());

	NqosWifiMacHelper mac; //Modify for a function
	
	
	
	NetDeviceContainer devices = wifi.Install (phy,mac,friitest);





}
