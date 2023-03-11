#include "ssd1315_img.h"

void ssd1315_img_init(SSD1315_IMG *img) {
    img->buffer[0] = 0x40; // Data start byte
    ssd1315_img_clear(img);
}

void ssd1315_img_array_init(SSD1315_IMG *imgs, uint8_t *buffer, uint8_t size, uint8_t colsX, uint8_t pagesY) {
    uint16_t i = size;

    if (size == 0)
        return;

    do {
        --i;
        imgs[i].colsX = colsX;
        imgs[i].pagesY = pagesY;
        imgs[i].buffer = &buffer[i * (1 + colsX * pagesY)];
        ssd1315_img_init(&imgs[i]);
    } while (i != 0);
}

void ssd1315_img_clear(SSD1315_IMG *img) {
    // Each row is just a bit of the column bytes giving us 128 pixels.
    // Each column is an 8 pixel byte x 8 giving us 64 pixels.
    // The extra bit is for the control chart at [0]. The for loop will not modify this value.
    uint16_t i;
    for (i = (img->colsX * img->pagesY); i > 0; --i) {
        img->buffer[i] = 0;
    }
}

int8_t ssd1315_img_set(SSD1315_IMG *img, uint8_t pixelX, uint8_t pixelY, uint8_t isOn) {
    if (pixelX > img->colsX || pixelY > (img->pagesY << 4)) {
        return -1; // OUT OF RANGE
    }

    // Figure out the column count and leave pixelY ready to shift bitMask.
    uint16_t col = 0;
    while (pixelY > 7) {
        pixelY -= 8;
        ++col;
    }
    uint16_t offset = (col * 128) + pixelX + 1; // the +1 is to account for the control byte
    img->buffer[offset] |= (0x1 << pixelY);

    return 0;
}

int8_t ssd1315_img_get(SSD1315_IMG *img, uint8_t pixelX, uint8_t pixelY) {
    if (pixelX > img->colsX || pixelY > (img->pagesY << 4)) {
        return -1; // OUT OF RANGE
    }

    // Figure out the column count and leave pixelY ready to shift bitMask.
    uint8_t col = 0;
    while (pixelY > 7) {
        pixelY -= 8;
        ++col;
    }
    uint16_t offset = (col * 128) + pixelX + 1; // the +1 is to account for the control byte
    return (img->buffer[offset] &= (0x1 << pixelY)) != 0;
}

int8_t ssd1315_img_draw(SSD1315_IMG *img, uint8_t pixelX, uint8_t pageY) {
    if (pixelX > 128 || pageY > 7) {
        return -1;
    }

    ssd1315_draw(pixelX, pageY, img->buffer, img->colsX * img->pagesY + 1);

    return 0;
}
