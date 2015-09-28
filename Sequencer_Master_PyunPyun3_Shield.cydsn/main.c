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
#include <project.h>
#include <stdio.h>
#include "ST7032.h"

#define I2C_SLAVE_ADDRESS   (0x7f)
#define I2C_RD_BUFFER_SIZE  (6u)
#define I2C_WR_BUFFER_SIZE  (1u)

/* Command valid status */
#define TRANSFER_CMPLT    (0x00u)
#define TRANSFER_ERROR    (0xFFu)

/***************************************
*               Macros
****************************************/
/* Set LED RED color */
#define RGB_LED_ON_RED  \
                do{     \
                    LED_RED_Write  (0u); \
                    LED_GREEN_Write(1u); \
                }while(0)

/* Set LED GREEN color */
#define RGB_LED_ON_GREEN \
                do{      \
                    LED_RED_Write  (1u); \
                    LED_GREEN_Write(0u); \
                }while(0) 

/***************************************
*               大域変数
****************************************/
uint8 rdBuffer[I2C_RD_BUFFER_SIZE];
uint8 wrBuffer[I2C_WR_BUFFER_SIZE] = {0};

/*======================================================
 * Sequencer Board 
 *
 *======================================================*/
uint32 readSequencerBoard(void)
{
    uint32 status = TRANSFER_ERROR; 
    
    // Read from sequencer board
    //
    I2CM_I2CMasterReadBuf(I2C_SLAVE_ADDRESS, rdBuffer, I2C_RD_BUFFER_SIZE, I2CM_I2C_MODE_COMPLETE_XFER);
    while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_RD_CMPLT))
    {
        /* Waits until master completes read transfer */
    }
    
    /* Displays transfer status */
    if (0u == (I2CM_I2C_MSTAT_ERR_XFER & I2CM_I2CMasterStatus()))
    {
        RGB_LED_ON_GREEN;

        /* Check if all bytes was written */
        if (I2CM_I2CMasterGetReadBufSize() == I2C_RD_BUFFER_SIZE)
        {
            status = TRANSFER_CMPLT;
        }
    }
    else
    {
        RGB_LED_ON_RED;
    }

    (void) I2CM_I2CMasterClearStatus();
    
    return status;
}

uint32 writeSequencerBoard(void)
{
    uint32 status = TRANSFER_ERROR; 
    
    I2CM_I2CMasterWriteBuf(I2C_SLAVE_ADDRESS, wrBuffer, I2C_WR_BUFFER_SIZE, I2CM_I2C_MODE_COMPLETE_XFER);
    while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_WR_CMPLT))
    {
        /* Waits until master completes read transfer */
    }
    
    /* Displays transfer status */
    if (0u == (I2CM_I2C_MSTAT_ERR_XFER & I2CM_I2CMasterStatus()))
    {
        RGB_LED_ON_GREEN;

        /* Check if all bytes was written */
        if (I2CM_I2CMasterGetWriteBufSize() == I2C_WR_BUFFER_SIZE)
        {
            status = TRANSFER_CMPLT;
        }
    }
    else
    {
        RGB_LED_ON_RED;
    }

    (void) I2CM_I2CMasterClearStatus();
    
    return status;   
}

/*======================================================
 * Main Routine 
 *
 *======================================================*/
uint8 inc_within_uint8(uint8 x, uint8 h, uint8 l)
{
    x++;
    if (x == h)
        x = l;
    return x;
}

int main()
{
    char uartBuffer[80];          
    
    UART_1_Start();    
    UART_1_UartPutString("Sequencer Board Test\r\n");
    
    // Sequence Boardをリセット
    Pin_I2C_Reset_Write(0u);
    CyDelay(1);
    Pin_I2C_Reset_Write(1u);
    
    /* Init I2C */
    I2CM_Start();   
    
    CyGlobalIntEnable;
    
    for(;;)
    {            
        if (readSequencerBoard() == TRANSFER_CMPLT) {
            sprintf(uartBuffer, "%d %d %d %d %d %d ",
                rdBuffer[0], rdBuffer[1], rdBuffer[2], rdBuffer[3], rdBuffer[4], rdBuffer[5]);
            UART_1_UartPutString(uartBuffer);
        }
        else {
            UART_1_UartPutString("I2C Master Read Error.\r\n");
        }
        
        if (writeSequencerBoard() == TRANSFER_CMPLT) {
            sprintf(uartBuffer, "%d\r\n", wrBuffer[0]);
            UART_1_UartPutString(uartBuffer);
        }
        else {
            UART_1_UartPutString("I2C Master Write Error.\r\n");
        }
        
        // Increment Note Posisiton 
        /*
        wrBuffer[0]++;
        if (wrBuffer[0] == 16)
           wrBuffer[0] = 0;
        */
        wrBuffer[0] = inc_within_uint8(wrBuffer[0], 16, 0);
        
        CyDelay(125);
    }
}

/* [] END OF FILE */
