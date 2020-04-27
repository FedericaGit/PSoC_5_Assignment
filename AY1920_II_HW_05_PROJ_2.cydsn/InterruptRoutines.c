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

uint8_t status_register;

CY_ISR(Custom_isr_MultiRead)
{
    //read the status register
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_STATUS_REG,
                                                  &status_register);
    if (error == NO_ERROR)
    {
        if (status_register & 0x08)     //if (ZYXDA==1). To check if new data are available 
        {
            sprintf(message, "Data are available.\r\n");
            UART_Debug_PutString(message);
            
            //read the x, y, z axis output
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                     LIS3DH_OUT_X_L,
                                                     6,
                                                     &AxisData[0]);
            if(error == NO_ERROR)
            {
                 for (int i=0; i < OUT_REG_NUMBER; i += 2)
                {
                    OutAxis = (int16)((AxisData[i] | (AxisData[i+1]<<8)))>>6; 
                    DataBuffer[i+1] = (uint8_t)(OutAxis & 0xFF);
                    DataBuffer[i+2] = (uint8_t)(OutAxis >> 8);
                }
            UART_Debug_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);
            }
            else
            {
                UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
            }
        } 
    }
}


/* [] END OF FILE */
