/* ========================================
 * This interrupt occurs every 10ms in 
 * order to trigger (through a flag) the read 
 * of the accelerometer output registers
 * at the same frequency they update their 
 * values (100Hz).
 * ========================================
 */

#include "InterruptRoutines.h"
#include "project.h"
#include "I2C_Interface.h"
#include "stdio.h"

uint8_t status_register;

CY_ISR(Custom_isr_MultiRead)
{
    //read Timer status register to lower the isr line
    Timer_MultiRead_ReadStatusRegister();   
    //flag that starts the multiread of the 3 axis values in the main.c 
    ReadPacketFlag=1;
 }

/* [] END OF FILE */
