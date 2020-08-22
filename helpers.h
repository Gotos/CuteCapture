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

#ifdef DS

#define SINGLE_SCREEN_HEIGHT 192
#define TOP_WIDTH 256
#define BOTTOM_WIDTH 256
#define TOTAL_RESOLUTION 192 * 256 * 2

void toRGBA(uint8_t *out, uint16_t *in);

#else

#define SINGLE_SCREEN_HEIGHT 240
#define TOP_WIDTH 400
#define BOTTOM_WIDTH 320
#define TOTAL_RESOLUTION 240 * 400 + 240 * 320

void toRGBA(const uint8_t* rgb, uint8_t* rgba);
#endif