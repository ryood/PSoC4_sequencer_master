#ifndef INCLUDED_ST7032_H_
#define INCLUDED_ST7032_H_

/*
    include元で
    I2C masterコンポーネントを「I2C_M」という名前で作成してください。
    LEDを接続したLED_RED、LED_GREENというPinを定義してください。
*/
    
#include <project.h>
    
/* I2C slave address to communicate with */
#define I2C_LCD_ADDR	(0b0111110)

/* Buffer and packet size */
#define BUFFER_SIZE     (2u)
#define PACKET_SIZE     (BUFFER_SIZE)

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

/* contrast: LCDのコントラストの設定 [0.. 63] */
void LCD_Init(uint8 contrast);
uint32 LCD_Write(uint8 *buffer);
void LCD_Cmd(uint8 cmd);
void LCD_Data(uint8 data);
void LCD_Clear();
void LCD_SetPos(uint32 x, uint32 y);
void LCD_Puts(char8 *s);

#endif // INCLUDED_ST7032_H_