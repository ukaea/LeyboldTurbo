#####################################################################################################
#																									#
#	Module:																							#
#		setrunning.py																				#
#																									#
#	Description:																					#
#		Uses pyepics to set (or unset) the 'Running' state of the pump.								#
#		NB, this allows the software user to turn the pump on electronically.						#
#		It is also possible for the equipment user to physically turn the pump on or off.			#
#		Pyepics is used for the convenience of cross-platform scripting.							#
#																									#
#	Author:  Peter Heesterman (Tessella plc). Date: 03 Sep 2015.									#
#	Written for CCFE (Culham Centre for Fusion Energy).												#
#																									#
#	LeyboldTurbo is distributed subject to a Software License Agreement								#
#	found in file LICENSE that is included with this distribution.									#
#																									#
#####################################################################################################

import epics
import os
import sys

FirstPump='1'
if len(sys.argv) > 1:
	FirstPump=sys.argv[1]
	
LastPump='1'
if len(sys.argv) > 2:
	LastPump=sys.argv[2]
	
Run='1'
if len(sys.argv) > 3:
	Run=sys.argv[3]

ChannelDefaultRoot = os.getenv('ASYNPORT', 'LEYBOLDTURBO')
for Pump in range(int(FirstPump), int(LastPump)+1):
	print("Setting pump", Pump, Run)
	ChannelRoot = os.getenv('ASYNPORT'+str(Pump), ChannelDefaultRoot+':'+str(Pump))
	epics.caput(ChannelRoot + ':Running', Run)
	ASYNVERSION = epics.caget(ChannelRoot + ':AsynVersion')
	if ASYNVERSION < "4-26":
		epics.caput(ChannelRoot + ':Running.PROC', 1)
