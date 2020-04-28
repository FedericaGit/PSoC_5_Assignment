/**
* \brief Main source file 
*
* In this project we test the LIS3DH 3-Axis Accelerometer
* output capabilities. 
*
* The multiread of the axial output registers is triggered
* by the timer ISR that occurs with a 100Hz frequency 
* (same freq. of the registers updating)
*
*/

// Include required header files
#include "I2C_Interface.h"
#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"

/**
*   \brief Address of the Status register
*/
#define LIS3DH_STATUS_REG 0x27

/**
*   \brief 7-bit I2C address of the slave device.
*/
#define LIS3DH_DEVICE_ADDRESS 0x18

/**
*   \brief Address of the WHO AM I register
*/
#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

/**
*   \brief Address of the Control register 1
*/
#define LIS3DH_CTRL_REG1 0x20   

/**
*   \brief Address of the Control register 4
*/
#define LIS3DH_CTRL_REG4 0x23  

/**
*   \brief Address of the output lower register of the x axis. 
*   
*   the higher register of x axis and the higher and lower registers 
*   of the y and z axis are adiacent to this one 
*   (6 output registers in total)
*/
#define LIS3DH_OUT_X_L 0x28 

/**
*   \brief Hex value to set normal mode at 100Hz to the accelerometer 
*/
#define LIS3DH_NORMAL_MODE_CTRL_REG1 0x57  

/**
*   \brief Hex value to set the +/-2.0g FSR (which is the default value). (BDU active)
*/
#define LIS3DH_CTRL_REG4_DEFAULT 0x80   

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Peripheral_Start();
    UART_Debug_Start();
    char message[50];  // String to print out messages on the UART
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."


    // Check which devices are present on the I2C bus
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "\r\nDevice 0x%02X is connected\r\n", i);
            UART_Debug_PutString(message); 
        }
        
    }
    
    /******************************************/
    /*            I2C Reading                 */
    /******************************************/
    
    /* Read WHO AM I register */
    
    uint8_t who_am_i_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_WHO_AM_I_REG_ADDR, 
                                                  &who_am_i_reg);
    if (error == NO_ERROR)
    {
        sprintf(message, "WHO AM I REG: 0x%02X [Expected: 0x33]\r\n", who_am_i_reg);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm\r\n");   
    }
    
    /* Read Status Register */
    
    uint8_t status_register; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &status_register);
    if (error == NO_ERROR)
    {
        sprintf(message, "STATUS REGISTER: 0x%02X\r\n", status_register);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read status register\r\n");   
    }
       
    /******************************************/
    /*        Read Control Register 1         */
    /******************************************/
    uint8_t ctrl_reg1; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
           
    if (ctrl_reg1 != LIS3DH_NORMAL_MODE_CTRL_REG1)
    {
        UART_Debug_PutString("Updating the register..\r\n");
        
        ctrl_reg1 = LIS3DH_NORMAL_MODE_CTRL_REG1;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1);
        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_CTRL_REG1,
                                            &ctrl_reg1);
        
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 after being updated: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
    
    /******************************************/
    /*        Read Control Register 4         */
    /******************************************/
        
    uint8_t ctrl_reg4;

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
    if (ctrl_reg4 != LIS3DH_CTRL_REG4_DEFAULT)
    {
        UART_Debug_PutString("Updating the register..\r\n");
        
        ctrl_reg4 = LIS3DH_CTRL_REG4_DEFAULT; // must be changed to the appropriate value

        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG4,
                                             ctrl_reg4);

        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_CTRL_REG4,
                                            &ctrl_reg4);   
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 after being updated: 0x%02X\r\n", ctrl_reg4);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
        }
    }
    
    //number of registers we want to read (2 registers (=2byte) per each axis)
    #define OUT_REG_NUMBER 6         
    //total number of byte we want to send through the UART (header + axis values bytes + footer)
    #define TRANSMIT_BUFFER_SIZE 1+ OUT_REG_NUMBER +1 
    
    //axis acceleration output data
    uint8_t AxisData[OUT_REG_NUMBER];   
    //buffer containing all the 3 axis outputs + header and footer
    uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE]; 
    //right justified output casted at 16 bit
    int16_t OutAxis;                             

    DataBuffer[0] = 0xA0;                        //header
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0;   //footer
    
    Timer_MultiRead_Start();                     //start the Timer which triggers the interrupt on TC
    isr_MultiRead_StartEx(Custom_isr_MultiRead); //start the Timer isr
    ReadPacketFlag = 0;                          //set to 0 the isr occurrence flag 
    
    for(;;)
    {
        if (ReadPacketFlag==1) //flag that goes high every time the isr occurs
        {   
            //read the status register
            ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_STATUS_REG,
                                                  &status_register);
            if (error == NO_ERROR)
            {
                //to check if new acceleration data are available (if (ZYXDA==1))  
                if (status_register & 0x08)     
                {   
                //read the x, y, z axis output and store them in AxisData
                error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                         LIS3DH_OUT_X_L,
                                                         OUT_REG_NUMBER,
                                                         &AxisData[0]);
                    if(error == NO_ERROR)
                    {
                        /*
                        *  in this cycle we read the 3 axis acceleration 
                        *  outputs and convert them into mg.
                        *
                        *  After that, in order to trasmit this int16-values through 
                        *  UART they are splitted into 2 bytes and stored in a buffer.
                        */
                        for (int i=0; i < OUT_REG_NUMBER; i += 2)
                        {
                            //trasforming the axis output in a right-justified 16-bit integer
                            OutAxis = (int16)((AxisData[i] | (AxisData[i+1]<<8)))>>6; 
                            //converting the output value in mg (4 mg/digit)
                            OutAxis *= 4;
                            //storing the 16-bit int in 2 bytes of the buffer to send them through UART
                            DataBuffer[i+1] = (uint8_t)(OutAxis >> 8);
                            DataBuffer[i+2] = (uint8_t)(OutAxis & 0xFF);
                        }
                    //the sample is sent through UART communication
                    UART_Debug_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);
                    ReadPacketFlag=0;
                    }
                }
            }        
        }
    }
}

/* [] END OF FILE */
