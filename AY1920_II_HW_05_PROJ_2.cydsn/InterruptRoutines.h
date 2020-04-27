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

#ifndef _INTERRUPT_ROUTINES_H
    #define _INTERRUPT_ROUTINES_H
    /**
    *   \brief Address of the Status register
    */
    #define LIS3DH_STATUS_REG 0x27
    
    /**
    *   \brief 7-bit I2C address of the slave device.
    */
    #define LIS3DH_DEVICE_ADDRESS 0x18
    
    /**
    *   \brief Address of the output lower register of the x axis. 
    *   
    *   the higher register of x axis and the higher and lower registers 
    *   of the y and z axis are adiacent to this one
    */
    #define LIS3DH_OUT_X_L 0x28 
    //total number of output registers for the 3 axis
    #define OUT_REG_NUMBER 6         
    //total number of bytes we want to sent through the UART (header + axis values bytes + footer)
    #define TRANSMIT_BUFFER_SIZE 1+ OUT_REG_NUMBER +1 
    
    #include "cytypes.h"
    #include "stdio.h"

    CY_ISR_PROTO(Custom_isr_MultiRead);

    char message[50];                       // String to print out messages on the UART
    uint8_t AxisData[OUT_REG_NUMBER];       //axis acceleration output data
    uint8 DataBuffer[TRANSMIT_BUFFER_SIZE]; //buffer containing all the 3 axis outputs + header and footer
    int16_t OutAxis;                        //right justified output casted at 16 bit
    
#endif    

/* [] END OF FILE */
