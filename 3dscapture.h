#pragma once
#include <stdint.h>
#include <stdbool.h>

enum {
    FRAMEWIDTH   =240,
    FRAMEHEIGHT  =720,
    FRAMESIZE    =FRAMEWIDTH*FRAMEHEIGHT*3,
    FRAMEBUFSIZE =FRAMESIZE + 0x200,    //extra space for USB maxPacketSize
    
    //capture_grabFrame() return codes
    CAPTURE_OK,
    CAPTURE_SKIP,       //transient error (lost frame / closed lid)
    CAPTURE_ERROR,      //connection problem
};

bool capture_init();
void capture_deinit();
int capture_grabFrame(uint8_t *frameBuf);
uint8_t capture_getVersion();

void toRGBA(const uint8_t* rgb, uint8_t* rgba);
