/*
 * ssd1315.c
 *
 *  Created on: Jun 6, 2022
 *      Author: jenkins
 */

#include <string.h>
#include "font_5x7.h"
#include "i2c.h"
#include "ssd1315.h"


unsigned char SSD1315_BUFFER[17];

void ssd1315_init(void) {
    ssd1315_command(SSD1315_SET_DISPLAY_OFF);                           // 0xAE
    ssd1315_command(SSD1315_SET_CLOCK_DIV);                             // 0xD5
    ssd1315_command(0x80);                                              // the suggested ratio 0x80

    ssd1315_command(SSD1315_SET_MUX_RATIO);                             // 0xA8
    ssd1315_command(SSD1315_OLED_HEIGHT - 1);

    ssd1315_command(SSD1315_SET_DISPLAY_OFFSET);                        // 0xD3
    ssd1315_command(0x0);                                               // no offset
    ssd1315_command(SSD1315_SET_DISPLAY_START_LINE | 0x0);              // line #0
    ssd1315_command(SSD1315_SET_CHARGE_PUMP);                           // 0x8D
    ssd1315_command(SSD1315_CPUMP_ON_7_5);                              // generate high voltage from 3.3v line internally
    ssd1315_command(SSD1315_SET_MEM_ADDR_MODE);                         // 0x20
    ssd1315_command(SSD1315_MEM_ADDR_MODE_HORIZONTAL);                  // Horizontal Address Mode
    ssd1315_command(SSD1315_SEGMENT_MAP_INVERSE);
    ssd1315_command(SSD1315_SET_COM_OUTPUT_REV);

    ssd1315_command(SSD1315_SET_COM_PINS_CONF);                         // 0xDA
    ssd1315_command(0x12);
    ssd1315_command(SSD1315_SET_CONTRAST);                              // 0x81
    ssd1315_command(SSD1315_DEFAULT_CONTRAST);

    ssd1315_command(SSD1315_SET_PRECHARGE_PERIOD);                      // 0xd9
    ssd1315_command(0xF1);
    ssd1315_command(SSD1315_SET_VCOM_LEVEL);                            // 0xDB
    ssd1315_command(0x40);

    ssd1315_command(SSD1315_ENTIRE_DISPLAY_NORM);                       // 0xA4

    ssd1315_command(SD1315_SET_NORMAL_DISP);                            // 0xA6

    ssd1315_command(SSD1315_STOP_SCROLL);

    ssd1315_command(SSD1315_SET_FADE_BLINK);
    ssd1315_command(SSD1315_FB_DISABLE);

    ssd1315_command(SSD1315_SET_ZOOM);
    ssd1315_command(SSD1315_ZOOM_DISABLE);

    ssd1315_command(SSD1315_SET_DISPLAY_ON);                            //--turn on OLED panel
}

void ssd1315_command(unsigned char command) {
    SSD1315_BUFFER[0] = 0x80;
    SSD1315_BUFFER[1] = command;

    i2c_write(SSD1315_I2C_ADDRESS, SSD1315_BUFFER, 2);
}

void ssd1315_clear(void) {
    ssd1315_position(0, 0);
    uint8_t i;
    for (i = 64; i > 0; i--) {                                          // count down for loops when possible for ULP
        uint8_t x;
        for(x = 16; x > 0; x--) {
            if (x == 1) {
                SSD1315_BUFFER[x-1] = 0x40;
            } else {
                SSD1315_BUFFER[x-1] = 0x0;
            }
        }

        i2c_write(SSD1315_I2C_ADDRESS, SSD1315_BUFFER, 17);
    }
}

void ssd1315_position(uint8_t column, uint8_t page) {
    if (column > 127) {
        column = 0;                                                     // Wrap around column upper limit
    }

    if (page > 7) {
        page = 0;                                                       // Wrap around page upper limit
    }

    ssd1315_command(SSD1315_SET_COL_ADDR);
    ssd1315_command(column);                                            // Column start address (0 = reset)
    ssd1315_command(SSD1315_OLED_WIDTH - 1);                            // Column end address (127 = reset)

    ssd1315_command(SSD1315_SET_PAGE_ADDR);
    ssd1315_command(page);                                              // Page start address (0 = reset)
    ssd1315_command(7);                                                 // Page end address ((64 / 8) - 1)
}

void ssd1315_print(uint8_t column, uint8_t page, char* str) {
    uint8_t i;

    ssd1315_position(column, page);

    while (*str != '\0') {
        SSD1315_BUFFER[0] = 0x40;                                       // Mark buffer as data (the command was already sent to move)

        for (i = 0; i < 5; ++i) {
            SSD1315_BUFFER[i+1] = font_5x7[*str - ' '][i];
        }

        SSD1315_BUFFER[6] = 0x0;

        i2c_write(SSD1315_I2C_ADDRESS, SSD1315_BUFFER, 7);
        ++str;
    }
}

void ssd1315_rprint(uint8_t column, uint8_t page, char* str) {
    uint8_t i;

    ssd1315_position(column, page);

    while (*str != '\0') {
        SSD1315_BUFFER[0] = 0x40;                                       // Mark buffer as data (the command was already sent to move)

        for (i = 0; i < 5; ++i) {
            SSD1315_BUFFER[5 - i] = font_5x7[*str - ' '][i];
        }

        SSD1315_BUFFER[6] = 0x0;

        i2c_write(SSD1315_I2C_ADDRESS, SSD1315_BUFFER, 7);
        ++str;
    }
}

void reverse(char *s)
{
    uint8_t i, j;
    uint8_t c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void ultoa(uint32_t val, char *string) {
    uint8_t i = 0;
    uint8_t j = 0;
                                                                        // use do loop to convert val to string
    do {
        if (j==3) {                                                     // we have reached a separator position
            string[i++] = ',';                                          // add a separator to the number string
            j=0;                                                        // reset separator indexer thingy
        }
            string[i++] = val%10 + '0';                                 // add the ith digit to the number string
            j++;                                                        // increment counter to keep track of separator placement
    } while ((val/=10) > 0);

    string[i++] = '\0';                                                 // add termination to string
    reverse(string);                                                    // string was built in reverse, fix that
}

void ssd1315_uint32(uint8_t column, uint8_t page, uint32_t val) {
    char text[14];

    ultoa(val, text);
    ssd1315_print(column, page, text);
}

void ssd1315_draw(unsigned char col, unsigned char page, const unsigned char * img, unsigned int size) {
    ssd1315_position(col, page);

    i2c_write(SSD1315_I2C_ADDRESS, img, size);
}
