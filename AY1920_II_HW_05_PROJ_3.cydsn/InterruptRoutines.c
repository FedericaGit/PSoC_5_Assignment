/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "InterruptRoutines.h"
#include "project.h"
#include "I2C_Interface.h"
#include "stdio.h"

CY_ISR(Custom_isr_MultiRead)
{
    //read Timer status register to lower the isr line
    Timer_MultiRead_ReadStatusRegister();   
    //flag that starts the multiread of the 3 axis values in the main 
    ReadPacketFlag=1;
 }

/* [] END OF FILE */
