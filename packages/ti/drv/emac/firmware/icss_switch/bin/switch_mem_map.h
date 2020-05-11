//***********************************************************************************
//**+-----------------------------------------------------------------------------+**
//**|                              ******                                         |**
//**|                              ******     o                                   |**
//**|                              *******__////__****                            |**
//**|                              ***** /_ //___/ ***                            |**
//**|                           ********* ////__ ******                           |**
//**|                             *******(_____/ ******                           |**
//**|                                 **********                                  |**
//**|                                   ******                                    |**
//**|                                      ***                                    |**
//**|                                                                             |**
//**|            Copyright (c) 2019 Texas Instruments Incorporated                |**
//**|                           ALL RIGHTS RESERVED                               |**
//**|                                                                             |**
//**|    Permission is hereby granted to licensees of Texas Instruments           |**
//**|    Incorporated (TI) products to use this computer program for the sole     |**
//**|    purpose of implementing a licensee product based on TI products.         |**
//**|    No other rights to reproduce, use, or disseminate this computer          |**
//**|    program, whether in part or in whole, are granted.                       |**
//**|                                                                             |**
//**|    TI makes no representation or warranties with respect to the             |**
//**|    performance of this computer program, and specifically disclaims         |**
//**|    any responsibility for any damages, special or consequential,            |**
//**|    connected with the use of this program.                                  |**
//**|                                                                             |**
//**+-----------------------------------------------------------------------------+**
//***********************************************************************************
// file:     switch_mem_map.h
//
// brief:    Contains memory map for Ethernet Switch.
//           This file is used by Ethernet Switch driver.

#ifndef ____switch_mem_map_h
#define ____switch_mem_map_h 1

#include "switch_mmap_defines.h"

//************************************************************************************
//
// Memory Usage of : SHARED_MEMORY
//
//************************************************************************************

#define HOST_PORT_DF_VLAN_OFFSET                           0x0018    //default VLAN tag for Host Port
#define EMAC_ICSSG_SWITCH_PORT0_DEFAULT_VLAN_OFFSET        HOST_PORT_DF_VLAN_OFFSET    //Same as HOST_PORT_DF_VLAN_OFFSET
#define P1_PORT_DF_VLAN_OFFSET                             0x001C    //default VLAN tag for P1 Port
#define EMAC_ICSSG_SWITCH_PORT1_DEFAULT_VLAN_OFFSET        P1_PORT_DF_VLAN_OFFSET    //Same as P1_PORT_DF_VLAN_OFFSET
#define P2_PORT_DF_VLAN_OFFSET                             0x0020    //default VLAN tag for P2 Port
#define EMAC_ICSSG_SWITCH_PORT2_DEFAULT_VLAN_OFFSET        P2_PORT_DF_VLAN_OFFSET    //Same as P2_PORT_DF_VLAN_OFFSET
#define VLAN_STATIC_REG_TABLE_OFFSET                       0x0100    //VLAN-FID Table offset. 4096 VIDs. 2B per VID = 8KB = 0x2000
#define EMAC_ICSSG_SWITCH_DEFAULT_VLAN_TABLE_OFFSET        VLAN_STATIC_REG_TABLE_OFFSET    //VLAN-FID Table offset for EMAC
#define PORT_DESC0_HI                                      0x2104    //packet descriptor Q reserved memory
#define PORT_DESC0_LO                                      0x2F6C    //packet descriptor Q reserved memory
#define PORT_DESC1_HI                                      0x3DD4    //packet descriptor Q reserved memory
#define PORT_DESC1_LO                                      0x4C3C    //packet descriptor Q reserved memory
#define HOST_DESC0_HI                                      0x5AA4    //packet descriptor Q reserved memory
#define HOST_DESC0_LO                                      0x5F0C    //packet descriptor Q reserved memory
#define HOST_DESC1_HI                                      0x6374    //packet descriptor Q reserved memory
#define HOST_DESC1_LO                                      0x67DC    //packet descriptor Q reserved memory
#define HOST_SPPD0                                         0x7AAC    //special packet descriptor Q reserved memory
#define HOST_SPPD1                                         0x7EAC    //special packet descriptor Q reserved memory

//************************************************************************************
//
// Memory Usage of : MSMC
//
//************************************************************************************


//************************************************************************************
//
// Memory Usage of : DMEM0
//
//************************************************************************************

#define TAS_CONFIG_CHANGE_TIME                             0x000C    //New list is copied at this time
#define TAS_CONFIG_CHANGE_ERROR_COUNTER                    0x0014    //config change error counter
#define TAS_CONFIG_PENDING                                 0x0018    //TAS List update pending flag
#define TAS_CONFIG_CHANGE                                  0x0019    //TAS list update trigger flag
#define TAS_ADMIN_LIST_LENGTH                              0x001A    //List length for new TAS schedule
#define TAS_ACTIVE_LIST_INDEX                              0x001B    //Currently active TAS list index
#define TAS_ADMIN_CYCLE_TIME                               0x001C    //Cycle time for the new TAS schedule
#define TAS_CONFIG_CHANGE_CYCLE_COUNT                      0x0020    //Cycle counts remaining till the TAS list update
#define PSI_L_REGULAR_FLOW_ID_BASE_OFFSET                  0x0024    //Base Flow ID for sending packets to Host for Slice0
#define EMAC_ICSSG_SWITCH_PSI_L_REGULAR_FLOW_ID_BASE_OFFSET PSI_L_REGULAR_FLOW_ID_BASE_OFFSET    //Same as PSI_L_REGULAR_FLOW_ID_BASE_OFFSET
#define PSI_L_MGMT_FLOW_ID_OFFSET                          0x0026    //Base Flow ID for sending mgmt and Tx TS to Host for Slice0
#define EMAC_ICSSG_SWITCH_PSI_L_MGMT_FLOW_ID_BASE_OFFSET   PSI_L_MGMT_FLOW_ID_OFFSET    //Same as PSI_L_MGMT_FLOW_ID_OFFSET
#define SPL_PKT_DEFAULT_PRIORITY                           0x0028    //Queue number for Special packets written here
#define EXPRESS_PRE_EMPTIVE_Q_MASK                         0x0029    //Express Preemptible Queue Mask
#define QUEUE_NUM_UNTAGGED                                 0x002A    //Port1/Port2 Default Queue number for untagged packets, only 1B is used
#define PORT_Q_PRIORITY_REGEN_OFFSET                       0x002C    //Stores the table used for priority regeneration. 1B per PCP/Queue
#define EXPRESS_PRE_EMPTIVE_Q_MAP                          0x0034    //For marking packet as priority/express (this feature is disabled) or cut-through/S&F. One per slice
#define PORT_Q_PRIORITY_MAPPING_OFFSET                     0x003C    //Stores the table used for priority mapping. 1B per PCP/Queue
#define TAS_GATE_MASK_LIST0                                0x0100    //TAS gate mask for windows list0
#define TAS_GATE_MASK_LIST1                                0x0350    //TAS gate mask for windows list1
#define PRE_EMPTION_ENABLE_TX                              0x05A0    //Memory to Enable/Disable Preemption on TX side
#define PRE_EMPTION_ACTIVE_TX                              0x05A1    //Active State of Preemption on TX side
#define PRE_EMPTION_ENABLE_VERIFY                          0x05A2    //Memory to Enable/Disable Verify State Machine Preemption
#define PRE_EMPTION_VERIFY_STATUS                          0x05A3    //Verify Status of State Machine
#define PRE_EMPTION_ADD_FRAG_SIZE_REMOTE                   0x05A4    //Non Final Fragment Size supported by Link Partner
#define PRE_EMPTION_ADD_FRAG_SIZE_LOCAL                    0x05A6    //Non Final Fragment Size supported by Firmware
#define PRE_EMPTION_VERIFY_TIME                            0x05A8    //Time in ms the State machine waits for respond packet
#define MGR_R30_CMD_OFFSET                                 0x05AC    //Memory used for R30 related management commands
#define BUFFER_POOL_0_ADDR_OFFSET                          0x05BC    //HW Buffer Pool0 base address
#define HOST_RX_Q_PRE_CONTEXT_OFFSET                       0x0684    //16B for Host Egress MSMC Q (Pre-emptible) context
#define FDB_CMD_BUFFER                                     0x0894    //Buffer for 8 FDB entries to be added by 'Add Multiple FDB entries IOCTL

//************************************************************************************
//
// Memory Usage of : DMEM1
//
//************************************************************************************

#define FDB_AGEING_TIMEOUT_OFFSET                          0x00A0    //Time after which FDB entries are checked for aged out values. Value in nanoseconds

//************************************************************************************
//
// Memory Usage of : PA_STAT
//
//************************************************************************************

#define PA_STAT_32b_START_OFFSET                           0x0080    //Start of 32 bits PA_STAT counters


#endif // ____switch_mem_map_h
