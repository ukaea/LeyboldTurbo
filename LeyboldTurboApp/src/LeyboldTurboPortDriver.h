//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		LeyboldTurboPortDriver.h																	//
//																									//
//	Description:																					//
//		Declares the CLeyboldTurboPortDriver class.													//
//		This uses AsynPortDriver and asynOctetSyncIO to connect with either:						//
//			a. One or more serial-connected Leybold turbo pump controllers.							//
//			b. One or more TCP-connected simulatied controllers.									//
//		The class communicates by means of the Universal Serial Interface (USS)						//
//		protocol (http://www.automation.siemens.com/WW/forum/guests/PostShow.aspx?PostID=104133).	//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 05 Jan 2015.									//
//	Written for CCFE (Culham Centre for Fusion Energy).												//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LEYBOLD_TURBO_PORT_DRIVER_H
#define LEYBOLD_TURBO_PORT_DRIVER_H

#include "LeyboldBase.h"
#include "USSPacket.h"

#include <shareLib.h>

#include <map>
#include <string>
#include <vector>

class epicsShareClass CLeyboldTurboPortDriver : public CLeyboldBase {
public:
	// NB, an MBBI string is limited to 40 charachters in EPICS.
	static const size_t MaxEPICSMBBIStrLen = 16;
	static const size_t BitsPerUInt16 = 16;

    CLeyboldTurboPortDriver(const char *AsynPortName, int NumPumps, int NoOfPZD);
    ~CLeyboldTurboPortDriver();
	void disconnect();
    virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars,
                                        size_t *nActual, int *eomReason);
	void addIOPort(const char* IOPortName);
	static CLeyboldTurboPortDriver* Instance() {
		return m_Instance;
	}
	asynStatus ErrorHandler(int TableIndex, CException const& E);
	size_t NrInstalled() {
		return m_IOUsers.size()-1;
	}

                 
protected:
	template<size_t NoOfPZD> bool writeRead(int TableIndex, USSPacket<NoOfPZD> USSWritePacket, USSPacket<NoOfPZD>& USSReadPacket);
	template<size_t NoOfPZD> void processRead(int TableIndex, asynUser *pasynUser, USSPacket<NoOfPZD> const& USSReadPacket);
	template<size_t NoOfPZD> void processWrite(int TableIndex, asynUser *pasynUser, epicsInt32 value);
	static int UsedParams();

private:
	// Each of these is associated with an octet I/O connection (i.e. serial or TCP port).
	std::vector<asynUser*> m_IOUsers;
    std::vector<epicsMutex*> m_Mutexes;
    std::vector<bool> m_Disconnected;
	static CLeyboldTurboPortDriver* m_Instance;
};

#endif // LEYBOLD_TURBO_PORT_DRIVER_H
