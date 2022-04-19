/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_phy.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the timeout macro. */
#define PHY_TIMEOUT_COUNT 0x3FFFFFFU
//#define PHY_TIMEOUT_COUNT 64000

//#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
	unsigned char g_inx_enable_network_debug = 0;
//#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the ENET instance from peripheral base address.
 *
 * @param base ENET peripheral base address.
 * @return ENET instance.
 */
extern uint32_t ENET_GetInstance(ENET_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to enet clocks for each instance. */
extern clock_ip_name_t s_enetClock[FSL_FEATURE_SOC_ENET_COUNT];
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
typedef enum {
	INX_PHY_STATE_INIT,
	INX_PHY_STATE_WAIT_AUTONEG
} inxPhyState_t;
static inxPhyState_t gPhyState=INX_PHY_STATE_INIT;
static uint16_t gPhyAutonegTicks=0;
static bool first=true;
status_t PHY_Init(ENET_Type *base, uint32_t phyAddr, uint32_t srcClock_Hz)
{
    uint32_t bssReg;
    uint32_t idReg = 0;
    //uint32_t idReg2 = 0;
    status_t result = kStatus_Success;
    uint32_t instance = ENET_GetInstance(base);
    uint32_t timeDelay;

//for HRD board the TLK needs to be set up to provide 50Mhz clock at the moment, once that is a separate physical line we can do away with this
    if(first){
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    	/* Set SMI first. */
    	CLOCK_EnableClock(s_enetClock[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
#if CONFIG_PHY_ENABLE_CLOCK_OUTPUT == 1
    	ENET_SetSMI(base, srcClock_Hz, false);
    	first=false;
#else
    	ENET_SetSMI(base, srcClock_Hz, false);
    	first=false;
#endif
#if CONFIG_TLK106

    	//wait for a valid read from the id reg before going any further
    	do{
			//wait for chip to settle
			result=PHY_Read(base,phyAddr,0x2,&idReg);
			//LWIP_PLATFORM_DIAG(("phyAddr=%d idReg=%d",phyAddr,idReg));
		}while(idReg!=8192U);
    	//LWIP_PLATFORM_DIAG(("72 idReg=0x%X",idReg));
//#if CONFIG_PHY_ENABLE_CLOCK_OUTPUT
    	//enable RX_CLOCK 50Mhz
    	do{
    		result=PHY_Read(base,phyAddr,PHY_CONTROL2_REG,&idReg);
    	}while(idReg==65535);
    	//LWIP_PLATFORM_DIAG(("83 Control Register 2 = 0x%X",idReg));
//#endif
		//wait for chip to settle
		for (timeDelay = 0; timeDelay < 1000; timeDelay ++)
		{
			__ASM("nop");
		}

#if CONFIG_PHY_ENABLE_CLOCK_OUTPUT
		//loop round until we confirm that the 50Mhz clock has been started
		//we won't need this once the separate clock line is installed
		do{
			//write the value
			idReg=idReg | 0x1U; // Start emitting the 50MHz clock on the RMII RX_CLK PIN to the MCU
			if (g_inx_enable_network_debug)  HW_DIAG_printf(("enabling RMII 50Mhz clock idReg=0x%X\n",idReg));
			result=PHY_Write(base,phyAddr,PHY_CONTROL2_REG,idReg);
			if(result!=kStatus_Success){
				if (g_inx_enable_network_debug) HW_DIAG_printf(("Error writing PHY_CONTROL2_REG\n"));
				return result;
			}

			//read it back to confirm
			do{
				PHY_Read(base,phyAddr,PHY_CONTROL2_REG,&idReg);
			}while(idReg==65535);
		}while(!(idReg & 0x1U)); //until we are confident we are getting the 50Mhz clock
#endif //CONFIG_PHY_ENABLE_CLOCK_OUTPUT
#endif
    }else{

		/* Initialization after PHY stars to work. */

    	switch(gPhyState){
    		case INX_PHY_STATE_INIT:
    			//LWIP_PLATFORM_DIAG(("STATE=INX_PHY_STATE_INIT"));
    			//we need to make sure that our SMI bus is configured correctly for our clock speed
    			#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
					//Set SMI first.
					CLOCK_EnableClock(s_enetClock[instance]);
    			#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
				ENET_SetSMI(base, srcClock_Hz, false);
#if CONFIG_TLK106

				//wait for a valid read from the id reg before going any further
				do{
					result=PHY_Read(base,phyAddr,0x2,&idReg);
				}while(idReg!=8192U);
				//LWIP_PLATFORM_DIAG(("135 idReg 0x02 = %x",idReg));
				/* So we should be good to go now */
				/* Try a reset first? */
//#define CONFIG_PHY_DO_INITIAL_SOFTWARE_RESET 1
#if CONFIG_PHY_DO_INITIAL_SOFTWARE_RESET == 1
				uint32_t counter = PHY_TIMEOUT_COUNT;
				/* Reset PHY. */
				result = PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, PHY_BCTL_RESET_MASK);
				if (result != kStatus_Success){
					printf("failed to reset phy\r\n");
					return kStatus_PHY_AutoNegotiateFail;
				}
#endif
#define CONFIG_PHY_DISABLE_HARDWARE_RESET_PIN 1
#if CONFIG_PHY_DISABLE_HARDWARE_RESET_PIN == 1
				//set power down pin to an interrupt output (otherwise this is a reset input).
				PHY_Read(base,phyAddr,0x11U,&idReg);
				//LWIP_PLATFORM_DIAG(("130 physcr=0X%X",idReg));
				idReg = idReg | 1; //allow everything
				//LWIP_PLATFORM_DIAG(("132 physcr=0X%X",idReg));
				PHY_Write(base,phyAddr,0x11U,idReg);
#endif

				//check the rmii receive clock
				//PHY_Read(base,phyAddr,0xAU,&idReg);
				//LWIP_PLATFORM_DIAG(("156 rmii receive clock is %d",idReg & 1));
				/*idReg = idReg | 1;
				PHY_Write(base,phyAddr,0xAU,idReg);*/

				//check whether rmii mode is enabled
				//PHY_Read(base,phyAddr,0x17U,&idReg);
				//LWIP_PLATFORM_DIAG(("162 rmii status register is 0x%X",idReg));

#if CONFIG_PHY_FORCE_POLARITY == 1
				do{
					//get current polarity
					//this should always be set, the default from the TLK is inverted which breaks old hubs
					do{
						PHY_Read(base,phyAddr,0xBU,&idReg);
					}while(idReg==65535);

					if(idReg & 64){
						idReg = idReg & (~64);
						//LWIP_PLATFORM_DIAG(("setting normal polarity 0x%X=0x%X",0xBU,idReg));
						PHY_Write(base,phyAddr,0xBU,idReg);
						do{
							PHY_Read(base,phyAddr,0xBU,&idReg);
						}while(idReg==65535);
					}else{
						//LWIP_PLATFORM_DIAG(("no need to disable polarity inversion"));
					}
				}while(idReg & 64);
#endif //CONFIG_PHY_FORCE_POLARITY

				//enable automdix (auto crossover negotiation
				PHY_Read(base,phyAddr,0x19U,&idReg);
				idReg |= 32768; //enable bit 15, crossover negotiation
				PHY_Write(base,phyAddr,0x19U,idReg);

				//change our autonegotiation advertisements (shouldn't this be a write?)
				do{
					PHY_Read(base,phyAddr,PHY_AUTONEG_ADVERTISE_REG,&idReg);
				}while(idReg==65535);
				while(idReg!=0x0021){
					//LWIP_PLATFORM_DIAG(("131  ANAR _ Before =0x%X",idReg));
					idReg = 0x0021; //enable only 10BT - no pauses for full duplex and 802.3 coding
					result=PHY_Write(base,phyAddr,PHY_AUTONEG_ADVERTISE_REG,idReg);
					do{
						PHY_Read(base,phyAddr,PHY_AUTONEG_ADVERTISE_REG,&idReg);
					}while(idReg==65535);
					//LWIP_PLATFORM_DIAG(("132  ANAR AFTER =0x%X",idReg));
					//idReg=0x0021;
				}

#if CONFIG_PHY_DO_LOOPBACK
#else
				//trigger auto negotiation
				do{
					PHY_Read(base,phyAddr,PHY_BASICCONTROL_REG,&idReg);
				}while(idReg==65535);
				//LWIP_PLATFORM_DIAG(("253 idReg=0x%X",idReg));
				idReg=idReg | PHY_BCTL_RESTART_AUTONEG_MASK;
				idReg |= PHY_BCTL_AUTONEG_MASK;
				//idReg |= 0x1200; // this does restart and negotiation at the same time ...
				//LWIP_PLATFORM_DIAG(("168 Writing PHY_BASICCONTROL_REG (with auto negotiation) = 0x%X",idReg));
				PHY_Write(base,phyAddr,PHY_BASICCONTROL_REG,idReg);

				//read back basic control till the auto neg restart bit goes low
				do{
					PHY_Read(base,phyAddr,PHY_BASICCONTROL_REG,&idReg);
					//LWIP_PLATFORM_DIAG(("198 basic control bit 9=%d",idReg & 512U));
				}while(idReg & 512U);
#endif //CONFIG_PHY_DO_LOOPBACK
#else //if CONFIG_TLK106 - the followingisd for the FRDMK64 board.

				uint32_t counter = PHY_TIMEOUT_COUNT;
				/* Reset PHY. */
				result = PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, PHY_BCTL_RESET_MASK);
				if (result != kStatus_Success){
					printf("failed to reset phy\r\n");
					return kStatus_PHY_AutoNegotiateFail;
				}

	#if defined(FSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE)
				uint32_t data = 0;
				result = PHY_Read(base, phyAddr, PHY_CONTROL2_REG, &data);
				if ( result != kStatus_Success)
				{
					return result;
				}
				result = PHY_Write(base, phyAddr, PHY_CONTROL2_REG, (data | PHY_CTL2_REFCLK_SELECT_MASK));
				if (result != kStatus_Success)
				{
					return result;
				}
	#endif  /* FSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE */

				/* Set the negotiation. */
				result = PHY_Write(base, phyAddr, PHY_AUTONEG_ADVERTISE_REG,
								   (PHY_100BASETX_FULLDUPLEX_MASK | PHY_100BASETX_HALFDUPLEX_MASK |
									PHY_10BASETX_FULLDUPLEX_MASK | PHY_10BASETX_HALFDUPLEX_MASK | 0x1U));
				if (result != kStatus_Success){
					printf("failed to write autonegotiation\r\n");
					return kStatus_PHY_AutoNegotiateFail;
				}
				result = PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG,
									   (PHY_BCTL_AUTONEG_MASK | PHY_BCTL_RESTART_AUTONEG_MASK));
				if (result != kStatus_Success){
					printf("failed to write basic control reg\r\n");
					return kStatus_PHY_AutoNegotiateFail;
				}
#endif //else CONFIG_TLK106
				gPhyState=INX_PHY_STATE_WAIT_AUTONEG;
				gPhyAutonegTicks=0;
#if CONFIG_PHY_DO_INITIAL_SOFTWARE_RESET == 1
				return kStatus_PHY_SMIVisitTimeout;
				break;
#endif // else fall through and try to do an auto negotiation right away
    		case INX_PHY_STATE_WAIT_AUTONEG:
    			//LWIP_PLATFORM_DIAG(("STATE=INX_PHY_STATE_WAIT_AUTONEG gPhyAutonegTicks=%d",gPhyAutonegTicks));
    			gPhyAutonegTicks++;
    			if (gPhyAutonegTicks>CONFIG_PHY_MAX_AUTONEG_TICKS)
				{
    				gPhyAutonegTicks=0;
    				gPhyState=INX_PHY_STATE_INIT;
    				//LWIP_PLATFORM_DIAG(("Timing out autonego"));
					return kStatus_PHY_AutoNegotiateFail;
				}
#if CONFIG_TLK106
    			PHY_Read(base, phyAddr, PHY_BASICSTATUS_REG, &bssReg);
    			if(bssReg == 65535){
    				if (g_inx_enable_network_debug) HW_DIAG_printf(("Network Phy bssReg read failed - check PHY hardware!\n"));
					gPhyState=INX_PHY_STATE_INIT;
					return kStatus_PHY_AutoNegotiateFail;
				}

				uint32_t phyStatusReg=0;
				PHY_Read(base,phyAddr,0x10,&phyStatusReg);
				//LWIP_PLATFORM_DIAG(("229 phyStatusReg=0X%X",phyStatusReg));
				if(phyStatusReg & 16384){
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
					if (g_inx_enable_network_debug) HW_DIAG_printf("Network Crossover cable detected!\n");
#endif
				}

#if CONFIG_PHY_DO_LOOPBACK
				//enable loopback
				PHY_Read(base,phyAddr,0x16,&idReg);
				//printf("283 bist=0x%X",idReg);
				idReg = idReg | 4;
				PHY_Write(base,phyAddr,0x16,idReg);


				gPhyState=INX_PHY_STATE_INIT;
				gPhyAutonegTicks=0;
				return kStatus_Success;
#endif

				uint32_t autoNegDone=bssReg & PHY_BCTL_AUTONEG_COMPLETE_MASK;
				uint32_t linkReady=bssReg & PHY_LINK_READY_MASK;
				if (autoNegDone!=0 && linkReady!=0)
				{
					gPhyAutonegTicks=0;
					gPhyState=INX_PHY_STATE_INIT; //assume if we are called again it is to restart the process

					//do some cable diagnostics
					/*PHY_Read(base,phyAddr,0x1E,&idReg);
					printf("290 cdcr reg=0x%X\r\n",idReg);

					//trigger diagnostic start
					PHY_Write(base,phyAddr,0x1E,8000);
					do{
						PHY_Read(base,phyAddr,0x1E,&idReg);
						printf("297 cdcr reg=0x%X\r\n",idReg);
					}while(!(idReg & 2));*/

					return kStatus_Success;
				}else{
					return kStatus_PHY_SMIVisitTimeout;
				}
#else //if CONFIG_TLK106
				uint32_t ctlReg;
				PHY_Read(base, phyAddr, PHY_BASICSTATUS_REG, &bssReg);
				PHY_Read(base, phyAddr, PHY_CONTROL1_REG, &ctlReg);
				if (((bssReg & PHY_BSTATUS_AUTONEGCOMP_MASK) != 0) && (ctlReg & PHY_LINK_READY_MASK))
				{
					gPhyState=INX_PHY_STATE_INIT; //assume if we are called again it is to restart the process
					return kStatus_Success;
				}else{
					return kStatus_PHY_SMIVisitTimeout;
				}
#endif //else CONFIG_TLK106
    		break;
    	} //switch(gPhyState)
	} //else (first)


    return result;
}

status_t PHY_Write(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t data)
{
    uint32_t counter;

    /* Clear the SMI interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI write command. */
    ENET_StartSMIWrite(base, phyAddr, phyReg, kENET_MiiWriteValidFrame, data);

    /* Wait for SMI complete. */
    for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}

status_t PHY_Read(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t *dataPtr)
{
    assert(dataPtr);

    uint32_t counter;

    /* Clear the MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI read command operation. */
    ENET_StartSMIRead(base, phyAddr, phyReg, kENET_MiiReadValidFrame);

    /* Wait for MII complete. */
    for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Get data from MII register. */
    *dataPtr = ENET_ReadSMIData(base);

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}

status_t PHY_EnableLoopback(ENET_Type *base, uint32_t phyAddr, phy_loop_t mode, phy_speed_t speed, bool enable)
{
    status_t result;
    uint32_t data = 0;

    /* Set the loop mode. */
    if (enable)
    {
        if (mode == kPHY_LocalLoop)
        {
            if (speed == kPHY_Speed100M)
            {
                data = PHY_BCTL_SPEED_100M_MASK | PHY_BCTL_DUPLEX_MASK | PHY_BCTL_LOOP_MASK;
            }
            else
            {
                data = PHY_BCTL_DUPLEX_MASK | PHY_BCTL_LOOP_MASK;                
            }
           return PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, data);
        }
        else
        {
            /* First read the current status in control register. */
            result = PHY_Read(base, phyAddr, PHY_CONTROL2_REG, &data);
            if (result == kStatus_Success)
            {
                return PHY_Write(base, phyAddr, PHY_CONTROL2_REG, (data | PHY_CTL2_REMOTELOOP_MASK));
            }
        }
    }
    else
    {
        /* Disable the loop mode. */
        if (mode == kPHY_LocalLoop)
        {
            /* First read the current status in control register. */
            result = PHY_Read(base, phyAddr, PHY_BASICCONTROL_REG, &data);
            if (result == kStatus_Success)
            {
                data &= ~PHY_BCTL_LOOP_MASK;
                return PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, (data | PHY_BCTL_RESTART_AUTONEG_MASK));
            }
        }
        else
        {
            /* First read the current status in control one register. */
            result = PHY_Read(base, phyAddr, PHY_CONTROL2_REG, &data);
            if (result == kStatus_Success)
            {
                return PHY_Write(base, phyAddr, PHY_CONTROL2_REG, (data & ~PHY_CTL2_REMOTELOOP_MASK));
            }
        }
    }
    return result;
}

status_t PHY_GetLinkStatus(ENET_Type *base, uint32_t phyAddr, bool *status)
{
    assert(status);

    status_t result = kStatus_Success;
    uint32_t data;

    /* Read the basic status register. */
    result = PHY_Read(base, phyAddr, PHY_BASICSTATUS_REG, &data);
    if (result == kStatus_Success)
    {
        if (!(PHY_BSTATUS_LINKSTATUS_MASK & data))
        {
            /* link down. */
            *status = false;
        }
        else
        {
            /* link up. */
            *status = true;
        }
    }
    return result;
}

status_t PHY_GetLinkSpeedDuplex(ENET_Type *base, uint32_t phyAddr, phy_speed_t *speed, phy_duplex_t *duplex)
{
    assert(duplex);

    status_t result = kStatus_Success;
    uint32_t data;

#if CONFIG_TLK106
    uint32_t timeDelay;
    //now read the phy status register
	do{
		for (timeDelay = 0; timeDelay < 8388607; timeDelay ++)
		{
			__ASM("nop");
		}
		result=PHY_Read(base,phyAddr,0x10U,&data);
		//LWIP_PLATFORM_DIAG(("498 data=%d",data));
	}while(data & 32768);

	if(data & 4U){
		if (g_inx_enable_network_debug) HW_DIAG_printf(("PHY negotiated full duplex mode\n"));
		*duplex = kPHY_FullDuplex;
	}else{
		if (g_inx_enable_network_debug) HW_DIAG_printf(("PHY negotiated half duplex mode"));
		*duplex = kPHY_HalfDuplex;
	}

	if(data & 2U){
		if (g_inx_enable_network_debug) HW_DIAG_printf(("Network is 10Mb connection\n"));
		*speed = kPHY_Speed10M;
	}else{
		if (g_inx_enable_network_debug) HW_DIAG_printf(("Network is 100Mb connection\n"));
		*speed = kPHY_Speed100M;
	}
#else
	uint32_t ctlReg;
    /* Read the control two register. */
	result = PHY_Read(base, phyAddr, PHY_CONTROL1_REG, &ctlReg);
	if (result == kStatus_Success)
	{
		data = ctlReg & PHY_CTL1_SPEEDUPLX_MASK;
		if ((PHY_CTL1_10FULLDUPLEX_MASK == data) || (PHY_CTL1_100FULLDUPLEX_MASK == data))
		{
		//	LWIP_PLATFORM_DIAG(("full duplex"));
			/* Full duplex. */
			*duplex = kPHY_FullDuplex;
		}
		else
		{
		//	LWIP_PLATFORM_DIAG(("half duplex"));
			/* Half duplex. */
			*duplex = kPHY_HalfDuplex;
		}

		data = ctlReg & PHY_CTL1_SPEEDUPLX_MASK;
		if ((PHY_CTL1_100HALFDUPLEX_MASK == data) || (PHY_CTL1_100FULLDUPLEX_MASK == data))
		{
		//	LWIP_PLATFORM_DIAG(("100M"));
			/* 100M speed. */
			*speed = kPHY_Speed100M;
		}
		else
		{ /* 10M speed. */
		//	LWIP_PLATFORM_DIAG(("10M"));
			*speed = kPHY_Speed10M;
		}
	}
#endif

    return result;
}
