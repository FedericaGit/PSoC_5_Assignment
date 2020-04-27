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
    
    #define OUT_REG_NUMBER 6         
    //total number of bytes we want to sent through the UART (header + axis values bytes + footer)
    #define TRANSMIT_BUFFER_SIZE 1+ OUT_REG_NUMBER +1 
    
    #include "cytypes.h"
    #include "stdio.h"
    #include "project.h"

    CY_ISR_PROTO(Custom_isr_MultiRead);
    
    char message[50];                       // String to print out messages on the UART
    uint8_t AxisData[OUT_REG_NUMBER];       //axis acceleration output data
    uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE]; //buffer containing all the 3 axis outputs + header and footer
    int16_t OutAxis;                        //right justified output casted at 16 bit
    
#endif    

/* [] END OF FILE */
