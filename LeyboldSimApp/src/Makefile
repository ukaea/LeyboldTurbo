TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

LIBRARY_IOC = LeyboldSimDev
PROD_IOC = LeyboldSimApp
# LeyboldSim.dbd will be created and installed
DBD += LeyboldSim.dbd

# LeyboldSim.dbd will be made up from these files:
LeyboldSimDev_DBD += base.dbd

# Include dbd files from all support applications:
#LeyboldSim_DBD += xxx.dbd
USR_INCLUDES += -I../../../LeyboldTurboApp/src

# Add all the support libraries needed by this IOC
LeyboldSimDev_LIBS += LeyboldTurboDev
LeyboldSimDev_LIBS += asyn
LeyboldSimDev_SYS_LIBS_WIN32 += ws2_32
LeyboldSimDev_SRCS += LeyboldSimPortDriver.cpp

# LeyboldSim_registerRecordDeviceDriver.cpp derives from LeyboldSim.dbd
LeyboldSimApp_LIBS += LeyboldSimDev
LeyboldSimApp_LIBS += asyn
LeyboldSimApp_SRCS += LeyboldSim_registerRecordDeviceDriver.cpp
LeyboldSimApp_SRCS += LeyboldSimMain.cpp

# Add support from base/src/vxWorks if needed
#LeyboldSim_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
LeyboldSimDev_LIBS += $(EPICS_BASE_IOC_LIBS)
LeyboldSimApp_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

