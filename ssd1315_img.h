/*
 * ssd1315_img.h
 *
 *  Created on: Dec 7, 2022
 *      Author: Michael Duncan
 */

#include "ssd1315.h"

#ifndef SSD1315_IMG_H_
#define SSD1315_IMG_H_

typedef struct SSD1315_IMG_t {
    uint8_t colsX;
    uint8_t pagesY;
    uint8_t *buffer;
} SSD1315_IMG;

#define SSD1315_IMG_STRUCT(var_, colsX_, pagesY_) uint8_t var_ ## Buf[colsX_ * pagesY_ + 1]; SSD1315_IMG var_ = { colsX_, pagesY_, var_##Buf}

void ssd1315_img_init(SSD1315_IMG *img);

void ssd1315_img_array_init(SSD1315_IMG *imgs, uint8_t *buffer, uint8_t size, uint8_t colsX, uint8_t pagesY);

#define SSD1315_IMG_ARRAY(var_, size_, colsX_, pagesY_) uint8_t var_ ## Buf[size_ * (colsX_ * pagesY_ + 1)]; SSD1315_IMG var_ ## [size_]; ssd1315_img_array_init(&var_, var_ ## Buf, size_, colsX_, pagesY_)

void ssd1315_img_clear(SSD1315_IMG *img);

int8_t ssd1315_img_set(SSD1315_IMG *img, uint8_t pixelX, uint8_t pixelY, uint8_t isOn);

int8_t ssd1315_img_get(SSD1315_IMG *img, uint8_t pixelX, uint8_t pixelY);

int8_t ssd1315_img_draw(SSD1315_IMG *img, uint8_t pixelX, uint8_t pageY);

#endif /* SSD1315_IMG_H_ */
