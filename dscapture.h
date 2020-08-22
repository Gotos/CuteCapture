#pragma once
#include <stdint.h>
#include <stdbool.h>

bool capture_init();
void capture_deinit();
bool capture_grabFrame(uint16_t *frameBuf);

enum {
    FRAMEWIDTH   =256,
    FRAMEHEIGHT  =192,
    FRAMESIZE    =FRAMEWIDTH*FRAMEHEIGHT*3,
    FRAMEBUFSIZE =FRAMESIZE + 0x200,    //extra space for USB maxPacketSize
};

void toRGBA(uint8_t *out, uint16_t *in);
