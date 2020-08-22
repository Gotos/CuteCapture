/*
 * Copyright 2014-2020 Alina Saalfeld
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 *         limitations under the License.
 */

#include <stdint.h>
#include <cstddef>
#include "helpers.h"

#ifdef DS
#include "dscapture.h"

// This is magic. No, I can't remember how it works either. Should probably be cleaned up - or at least explained.
void toRGBA(uint8_t *out, uint16_t *in) {
    size_t i;
    for(i=0;i<TOTAL_RESOLUTION;i++) {
        unsigned char r,g,b;
        g=((*in)>>5)&0x3f;
        b=((*in<<1)&0x3e)|(g&1);
        r=(((*in)>>10)&0x3e)|(g&1);
        out[0]=(r<<2) | (r>>4);
        out[1]=(g<<2) | (g>>4);
        out[2]=(b<<2) | (b>>4);
        out[3]=255;
        out+=4;
        in++;
    }
}

#else
#include "3dscapture.h"

// This is magic. No, I can't remember how it works either. Should probably be cleaned up - or at least explained.
void toRGBA(const uint8_t* rgb, uint8_t* rgba) {
    size_t i;
    for (i = 0; i < TOTAL_RESOLUTION; i++) {
        rgba[4*i] = rgb[3*i];
        rgba[4*i+1] = rgb[3*i+1];
        rgba[4*i+2] = rgb[3*i+2];
        rgba[4*i+3] = 255;
    }
}

#endif
