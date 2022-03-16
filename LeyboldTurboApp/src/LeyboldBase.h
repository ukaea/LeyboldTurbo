//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Module:																							//
//		LeyboldBase.h																				//
//																									//
//	Description:																					//
//		Declares the CLeyboldBase class that forms an intermediate base class for both				//
//		CLeyboldTurboPortDriver and CLeyboldSimPortDriver.											//
//		Includes parameter management and error handling.											//
//																									//
//	Author:  Peter Heesterman (Tessella plc). Date: 04 Mar 2015.									//
//	Written for CCFE (Culham Centre for Fusion Energy).												//
//																									//
//	LeyboldTurbo is distributed subject to a Software License Agreement								//
//	found in file LICENSE that is included with this distribution.									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LEYBOLD_BASE_H
#define LEYBOLD_BASE_H

#include "ParameterDefns.h"

#ifdef epicsExportSharedSymbols
#define LeyboldBaseepicsExportSharedSymbols
#undef epicsExportSharedSymbols
#endif

#include <asynPortDriver.h>
#include <envDefs.h>

#include <stdexcept>
#include <string>
#include <map>

#ifdef LeyboldBaseepicsExportSharedSymbols
#undef LeyboldBaseepicsExportSharedSymbols
#define epicsExportSharedSymbols
#endif
#include <shareLib.h>

class epicsShareClass CLeyboldBase : public asynPortDriver
{
public:
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//																								//
	//	class CLeyboldTurboPortDriver::CException : public std::runtime_error						//
	//	Description:																				//
	//		If an error ocurrs, an object of this type is thrown.									//
	//																								//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	class CException : public std::runtime_error
	{
		asynStatus m_Status;
	public:
		CException(asynUser* AsynUser, asynStatus Status, const char* functionName, std::string const& what) : std::runtime_error(what) {
			std::string message = "%s:%s ERROR: " + what + "%s\n";
			asynPrint(AsynUser, ASYN_TRACE_ERROR, message.c_str(), __FILE__, functionName, AsynUser->errorMessage);
			m_Status = Status;
		}
		asynStatus Status() const {
			return m_Status;
		}
	};

	static const size_t NoOfPZD6 = 6;
	static const size_t NoOfPZD2 = 2;

	// NB, a string is limited to 40 charachters in EPICS.
	static const size_t MaxEPICSStrLen = 40;

    CLeyboldBase(const char *portName, int maxAddr, int paramTableSize, int NoOfPZD);
    void createParam(size_t list, size_t ParamIndex);
    void createParam(size_t ParamIndex);
	void checkParamStatus(size_t list, const char* ParamName) {
#ifdef _DEBUG
		// NB Asyn 4-27 requires the parameter status to be clear before the value can be set.
		// Let's be sure that is the case, because it is a silent fail otherwise.
		asynStatus Status = getParamStatus(list, ParamName);
		if (Status != asynSuccess)
			throw CException(pasynUserSelf, Status, __FUNCTION__, ParamName);
#endif
	}
    void setIntegerParam(size_t list, const char* ParamName, int value, bool ThrowIt = true) {
		checkParamStatus(list, ParamName);
		asynStatus Status = asynPortDriver::setIntegerParam(int(list), Parameters(ParamName), value);
		if (Status != asynSuccess)
			throw CException(pasynUserSelf, Status, __FUNCTION__, ParamName);
	}
    void setIntegerParam(const char* ParamName, int value) {
		ThrowException(pasynUserSelf, asynPortDriver::setIntegerParam(Parameters(ParamName), value), __FUNCTION__, ParamName);
	}
    void setDoubleParam(size_t list, const char* ParamName, double value) {
		checkParamStatus(list, ParamName);
		ThrowException(pasynUserSelf, asynPortDriver::setDoubleParam(int(list), Parameters(ParamName), value), __FUNCTION__, ParamName);
	}
    void setDoubleParam(const char* ParamName, double value) {
		ThrowException(pasynUserSelf, asynPortDriver::setDoubleParam(Parameters(ParamName), value), __FUNCTION__, ParamName);
	}
	void setStringParam(size_t list, const char* ParamName, std::string const& value, bool ThrowIt = true) {
		checkParamStatus(list, ParamName);
		ThrowException(pasynUserSelf, asynPortDriver::setStringParam (int(list), Parameters(ParamName), value.substr(0, MaxEPICSStrLen).c_str()), __FUNCTION__, ParamName, ThrowIt);
	}
	void setStringParam(const char* ParamName, std::string const& value) {
		ThrowException(pasynUserSelf, asynPortDriver::setStringParam (Parameters(ParamName), value.substr(0, MaxEPICSStrLen).c_str()), __FUNCTION__, ParamName);
	}
    void setParamStatus(size_t list, const char* ParamName, asynStatus ParamStatus, bool ThrowIt = true) {
		ThrowException(pasynUserSelf, asynPortDriver::setParamStatus(int(list), Parameters(ParamName), ParamStatus), __FUNCTION__, ParamName);
	}
    asynStatus getParamStatus(size_t list, const char* ParamName) {
		asynStatus ParamStatus;
		ThrowException(pasynUserSelf, asynPortDriver::getParamStatus(int(list), Parameters(ParamName), &ParamStatus), __FUNCTION__, ParamName);
		return ParamStatus;
	}
    int getIntegerParam(size_t list, const char* ParamName) {
		int value = 0;
		ThrowException(pasynUserSelf, asynPortDriver::getIntegerParam(int(list), Parameters(ParamName), &value), __FUNCTION__, ParamName);
		return value;
	}
    double getDoubleParam(size_t list, const char* ParamName) {
		double value = 0;
		ThrowException(pasynUserSelf, asynPortDriver::getDoubleParam(int(list), Parameters(ParamName), &value), __FUNCTION__, ParamName);
		return value;
	}
    void getStringParam(size_t list, const char* ParamName, int maxChars, char *value) {
		ThrowException(pasynUserSelf, asynPortDriver::getStringParam(int(list), Parameters(ParamName), maxChars, value), __FUNCTION__, ParamName);
	}
	int Parameters(std::string const& ParamName) const {
		std::map<std::string, int>::const_iterator Iter = m_Parameters.find(ParamName);
		if (Iter == m_Parameters.end())
			throw CException(pasynUserSelf, asynError, __FUNCTION__, ParamName);
		return Iter->second;
	}
	void callParamCallbacks(size_t list, bool ThrowIt = true) {
		asynStatus Status = asynPortDriver::callParamCallbacks(int(list));
		if (Status != asynSuccess)
			throw CException(pasynUserSelf, Status, __FUNCTION__, "callParamCallbacks");
	}
	size_t getNoOfPZD() const {
		return m_NoOfPZD;
	}
protected:
	void ThrowException(asynUser* pasynUser, asynStatus Status, const char* Function, std::string const& what, bool ThrowIt = true) const {
		if (Status == asynSuccess)
			return;
		if (ThrowIt)
			throw CException(pasynUser, Status, __FUNCTION__, "callParamCallbacks");
		else
			asynPrint(pasynUser, ASYN_TRACE_ERROR, what.c_str(), __FILE__, Function, pasynUser->errorMessage);
	}
	static int Mask();
	void ParamDefaultValue(size_t ParamIndex);
	void ParamDefaultValue(size_t list, size_t ParamIndex);

	// Each parameter is associated with an int handle.
	// This structure is used in order to address them by name, which is more convenient.
	std::map<std::string, int> m_Parameters;

private:
	size_t m_NoOfPZD;

};

#endif
