/*
 * ssd1315.h
 *
 *  Created on: Jun 3, 2022
 *      Author: jenkins
 */

#ifndef SSD1315_H_
#define SSD1315_H_

#include <msp430.h>
#include <stdint.h>

/*
 * Customizable Values
 */
#define SSD1315_I2C_ADDRESS                 0x3C

#define SSD1315_OLED_WIDTH                  128
#define SSD1315_OLED_HEIGHT                 64

/*
 * Commands For Memory Addressing Modes
 */

// Set Memory Addressing Mode
#define SSD1315_SET_MEM_ADDR_MODE           0X20

// Addressing Mode Values
#define SSD1315_MEM_ADDR_MODE_HORIZONTAL    0X0
#define SSD1315_MEM_ADDR_MODE_VERTICAL      0X1
#define SSD1315_MEM_ADDR_MODE_PAGE          0X10


/*
 * Commands For Horizontal and Vertical Modes Only
 */

// Set Column Address
#define SSD1315_SET_COL_ADDR                0x21

// Set Page Address
#define SSD1315_SET_PAGE_ADDR               0x22

/*
 * Commands For Page Addressing Mode Only
 */

// Mask for Set Lower Column Start Address
#define SSD1315_COL_START_ADDR_L            0x00

// Mask for Set Higher Column Start Address
#define SSD1315_COL_START_ADDR_H            0x10

// Mask for Set Page Start Address
#define SSD1315_PAGE_START_ADDR             0xB0

/*
 *  Commands For All Addressing Modes
 */

// Mask for Set Display Start Line
#define SSD1315_SET_DISPLAY_START_LINE      0x40

// Select Contrast Control
#define SSD1315_SET_CONTRAST                0x81
#define SSD1315_DEFAULT_CONTRAST            0x7F

// Set Segment Re-map
#define SSD1315_SEGMENT_MAP_NORMAL          0xA0
#define SSD1315_SEGMENT_MAP_INVERSE         0xA1

// Entire Diaply On
#define SSD1315_ENTIRE_DISPLAY_NORM         0xA4
#define SSD1315_ENTIRE_DISPLAY_ON           0xA5

// Set Normal/Inverse Display
#define SD1315_SET_NORMAL_DISP              0xA6
#define SD1315_SET_INVERSE_DISP             0xA7

// Set Multiplex Ratio
#define SSD1315_SET_MUX_RATIO               0xA8

// Mux Ratio Values
#define SSD1315_MUX_RATIO_16                0xF
#define SSD1315_MUX_RATIO_64                0x3F

// Internal Iref Setting
#define SSD1315_SET_IREF                    0xAD

// Iref Values
#define SSD1315_IREF_EXTERNAL               0x0
#define SSD1315_IREF_INTERNAL_19ua          0x20
#define SSD1315_IREF_INTERNAL_30ua          0x30

// Set Display On/Off
#define SSD1315_SET_DISPLAY_OFF             0xAE
#define SSD1315_SET_DISPLAY_ON              0xAF

// Set COM Output Scan Direction
#define SSD1315_SET_COM_OUTPUT_NORM         0xC0
#define SSD1315_SET_COM_OUTPUT_REV          0xC8

// Set Display Offset
#define SSD1315_SET_DISPLAY_OFFSET          0xD3

// Set Display Clock Divide
#define SSD1315_SET_CLOCK_DIV               0xD5

// Set Pre-charge Period
#define SSD1315_SET_PRECHARGE_PERIOD        0xD9

// Set COM Pins Hardware Configuration
#define SSD1315_SET_COM_PINS_CONF           0xDA
// TODO: Values?

// Set Vcomh Select Level
#define SSD1315_SET_VCOM_LEVEL              0xDB

// Vcomh Valus
#define SSD1315_VCOM_65                     0x0
#define SSD1315_VCOM_71                     0x10
#define SSD1315_VCOM_77                     0x20
#define SSD1315_VCOM_83                     0x30

// NOP
#define SSD1315_NOP                         0xE3

// Charge Pump Setting
#define SSD1315_SET_CHARGE_PUMP             0x8D

// Charge Pump Values
#define SSD1315_CPUMP_OFF_7_5               0x10
#define SSD1315_CPUMP_ON_7_5                0x14
#define SSD1315_CPUMP_OFF_8_5               0x90
#define SSD1315_CPUMP_ON_8_5                0x94
#define SSD1315_CPUMP_OFF_9_0               0x91
#define SSD1315_CPUMP_ON_9_0                0x95

/*
 * Commands For Scrolling
 */

#define SSD1315_DUMMY_BYTE_L                0x0
#define SSD1315_DUMMY_BYTE_H                0xFF

// Continuous Horizontal Scroll Setup
#define SSD1315_SET_HORZ_SCROLL_RIGHT       0x26
#define SSD1315_SET_HORZ_SCROLL_LEFT        0x27

// Continuous Vertical & Horizontal Scroll
#define SSD1315_SET_VH_SCROLL_URIGHT        0x29
#define SSD1315_SET_VH_SCROLL_ULEFT         0x2A

// VH Column Scroll
#define SSD1315_VH_COL_SCR_NONE             0x0
#define SSD1315_VH_COL_SCR_ONE              0x1

// Continuous Vertical Scroll
#define SSD1315_SET_V_SCROLL                0xA3

// Stop/Start Scrolling
#define SSD1315_STOP_SCROLL                 0x2E
#define SSD1315_START_SCROLL                0x2F

// Content Scroll Setup
#define SSD1315_RSCROLL_NOW                 0x2C
#define SSD1315_LSCROLL_NOW                 0x2D

// Scroll Step Time Intervals (In terms of frame frequency)
#define SSD1315_2_FRAME_SCROLL              0x7
#define SSD1315_3_FRAME_SCROLL              0x4
#define SSD1315_4_FRAME_SCROLL              0x5
#define SSD1315_5_FRAME_SCROLL              0x6
#define SSD1315_6_FRAME_SCROLL              0x0
#define SSD1315_32_FRAME_SCROLL             0x1
#define SSD1315_64_FRAME_SCROLL             0x2
#define SSD1315_128_FRAME_SCROLL            0x3

/*
 * Commands for Fade / Blink
 */

// Set Fade Out and Blinking
#define SSD1315_SET_FADE_BLINK              0x23

// Mask for Fade / Blink
#define SSD1315_FB_DISABLE                  0x0
#define SSD1315_FB_FADE                     0x20
#define SSD1315_FB_BLINK                    0x30

// Fade / Blink Interval (Probably in terms of frame frequency)
#define SSD1315_8_FRAME_FB                  0x0
#define SSD1315_16_FRAME_FB                 0x1
#define SSD1315_24_FRAME_FB                 0x2
#define SSD1315_32_FRAME_FB                 0x3
#define SSD1315_40_FRAME_FB                 0x4
#define SSD1315_48_FRAME_FB                 0x5
#define SSD1315_56_FRAME_FB                 0x6
#define SSD1315_64_FRAME_FB                 0x7
#define SSD1315_72_FRAME_FB                 0x8
#define SSD1315_80_FRAME_FB                 0x9
#define SSD1315_88_FRAME_FB                 0xA
#define SSD1315_96_FRAME_FB                 0xB
#define SSD1315_104_FRAME_FB                0xC
#define SSD1315_112_FRAME_FB                0xD
#define SSD1315_120_FRAME_FB                0xE
#define SSD1315_128_FRAME_FB                0xF

/*
 * Commands for Zoom
 */

// Set Zoom In
#define SSD1315_SET_ZOOM                    0xD6
#define SSD1315_ZOOM_DISABLE                0x0
#define SSD1315_ZOOM_ENABLE                 0x1

/*
 * Functions
 */

void ssd1315_init(void);
void ssd1315_command(unsigned char);
void ssd1315_clear(void);
void ssd1315_position(uint8_t, uint8_t);
void ssd1315_print(uint8_t, uint8_t, char*);
void ssd1315_rprint(uint8_t, uint8_t, char*);
void ssd1315_uint32(uint8_t, uint8_t, uint32_t);

void ssd1315_draw(unsigned char col, unsigned char page, const unsigned char *, unsigned int size);

#endif /* SSD1315_H_ */

