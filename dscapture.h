#pragma once
#include <stdint.h>
#include <stdbool.h>

bool capture_init();
void capture_deinit();
bool capture_grabFrame(uint16_t *frameBuf);

enum {
    FRAMEWIDTH   =256,
    FRAMEHEIGHT  =192
};
