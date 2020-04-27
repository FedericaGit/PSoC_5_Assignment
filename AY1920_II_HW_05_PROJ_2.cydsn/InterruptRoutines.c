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

#define LIS3DH_OUT_X_L 0x28 
#define LIS3DH_DEVICE_ADDRESS 0x18
#define LIS3DH_STATUS_REG 0x27

CY_ISR(Custom_isr_MultiRead)
{
    
    uint8_t status_register;
    
    Timer_MultiRead_ReadStatusRegister();
    //read the status register
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_STATUS_REG,
                                                  &status_register);
    if (error == NO_ERROR)
    {
        if (status_register & 0x08)     //if (ZYXDA==1). To check if new data are available 
        {   
            //read the x, y, z axis output
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                     LIS3DH_OUT_X_L,
                                                     6,
                                                     &AxisData[0]);
            if(error == NO_ERROR)
            {
                 for (int i=0; i < OUT_REG_NUMBER; i += 2)
                {
                    //trasforming the 3 axial outputs in 3 right-justified 16-bit integers
                    OutAxis = (int16)((AxisData[i] | (AxisData[i+1]<<8)))>>6; 
                    //converting the output values in mg (4 mg/digit)
                    OutAxis *= 4;
                    //storing data in bytes to send them through UART
                    DataBuffer[i+1] = (uint8_t)(OutAxis >> 8);
                    DataBuffer[i+2] = (uint8_t)(OutAxis & 0xFF);
                }
            UART_Debug_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);
            }
        } 
    }
}

/* [] END OF FILE */
