/**
* \brief Main source file for the I2C-Master project.
*
* In this project we test the LIS3DH 3-Axis Accelerometer
* output capabilities.
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
*   \brief Address of the output lower register of the x axis. 
*   
*   the higher register of x axis and the higher and lower registers 
*   of the y and z axis are adiacent to this one
*/
#define LIS3DH_OUT_X_L 0x28 

//total number of output registers for the 3 axis
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
*   \brief Hex value to set high resolution mode at 100Hz to the accelerometer 
*/
#define LIS3DH_HIGH_RES_MODE_CTRL_REG1 0x57  

/**
*   \brief Hex value to set the +/-4.0g FSR
*/
#define LIS3DH_CTRL_REG4_HIGH_RES_MODE 0x18   

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
    
           
    if (ctrl_reg1 != LIS3DH_HIGH_RES_MODE_CTRL_REG1)
    {
        UART_Debug_PutString("Updating the register..\r\n");
        
        ctrl_reg1 = LIS3DH_HIGH_RES_MODE_CTRL_REG1;
    
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
    
    if (ctrl_reg4 != LIS3DH_CTRL_REG4_HIGH_RES_MODE)
    {
        UART_Debug_PutString("Updating the register..\r\n");
        
        ctrl_reg4 = LIS3DH_CTRL_REG4_HIGH_RES_MODE; // must be changed to the appropriate value

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
    
        
    //number of registers we want to read (2 registers per each axis)
    #define OUT_REG_NUMBER 6         
    //total number of bytes we want to sent through the UART (header + axis values bytes*2 (=4) + footer)
    #define TRANSMIT_BUFFER_SIZE 1+ OUT_REG_NUMBER*2 +1

    
    uint8_t AxisData[OUT_REG_NUMBER];            //axis acceleration output data
    uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE];    //buffer containing all the 3 axis outputs + header and footer
    int16_t OutAxis;                             //right justified output casted at 16 bit

    DataBuffer[0] = 0xA0;                        //header
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0;   //footer
    
    Timer_MultiRead_Start();                     //start the Timer which triggers the interrupt on TC
    isr_MultiRead_StartEx(Custom_isr_MultiRead); //start the Timer isr
    ReadPacketFlag = 0; 
    
    
    float Acc_ms2;                             //axis output in m/s^2
    int32 Acc_mms2;
        
    for(;;)
    {
        if (ReadPacketFlag==1) 
        {   
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
                                                         OUT_REG_NUMBER,
                                                         &AxisData[0]);
                if(error == NO_ERROR)
                {
                     for (int i=0; i < OUT_REG_NUMBER; i += 2)
                    {
                        //trasforming the 3 axial outputs in 3 right-justified 16-bit integers
                        OutAxis = (int16)((AxisData[i] | (AxisData[i+1]<<8)))>>4; 
                        //converting the output values in m/s^2 (for high resol. @ +/-4.0g we have 2 mg/digit)
                        Acc_ms2 = (float) OutAxis * 2 * 9.806 * 0.001;
                        //to keep 3 decimals of the acceleration value (Acc_ms2)
                        Acc_mms2 = Acc_ms2 * 1000;
                        //storing each axis data in 4 bytes to send them through UART 
                      
                        DataBuffer[i*2+1] = (uint8_t)(Acc_mms2 >> 24);
                        DataBuffer[i*2+2] = (uint8_t)(Acc_mms2 >> 16);
                        DataBuffer[i*2+3] = (uint8_t)(Acc_mms2 >> 8);
                        DataBuffer[i*2+4] = (uint8_t)(Acc_mms2 & 0xFF);
                      
                    }
                    //the samples are sent through UART communication
                    UART_Debug_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);
                    ReadPacketFlag=0;
                    }
                }
            }        
        }
    }
}

/* [] END OF FILE */
