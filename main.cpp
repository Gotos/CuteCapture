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

#include <stdio.h>
extern "C" {
#ifdef DS
    #include "dscapture.h"
#else
    #include "3dscapture.h"
#endif
}
#include <SFML/Graphics.hpp>
#include <map>


static sf::Clock m_time;
static uint frames;
static bool split;
static bool ds_crop_mode;
static bool init;
static std::map<int, int> keycode_zoom_map {
    { sf::Keyboard::Num0, 10},
    { sf::Keyboard::Num1, 1},
    { sf::Keyboard::Num2, 2},
    { sf::Keyboard::Num3, 3},
    { sf::Keyboard::Num4, 4},
    { sf::Keyboard::Num5, 5},
    { sf::Keyboard::Num6, 6},
    { sf::Keyboard::Num7, 7},
    { sf::Keyboard::Num8, 8},
    { sf::Keyboard::Num9, 9},
    { sf::Keyboard::Numpad0, 10},
    { sf::Keyboard::Numpad1, 1},
    { sf::Keyboard::Numpad2, 2},
    { sf::Keyboard::Numpad3, 3},
    { sf::Keyboard::Numpad4, 4},
    { sf::Keyboard::Numpad5, 5},
    { sf::Keyboard::Numpad6, 6},
    { sf::Keyboard::Numpad7, 7},
    { sf::Keyboard::Numpad8, 8},
    { sf::Keyboard::Numpad9, 9}
};

void toRGBA(const uint8_t* rgb, uint8_t* rgba, const size_t count) {
    size_t i;
    for (i=0; i<count; i++) {
        rgba[4*i] = rgb[3*i];
        rgba[4*i+1] = rgb[3*i+1];
        rgba[4*i+2] = rgb[3*i+2];
        rgba[4*i+3] = 255;
    }
}


/* GIMP RGBA C-Source image dump (favicon.c) */

static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[16 * 16 * 4 + 1];
} sfml_icon = {
  16, 16, 4,
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\374\2\4\377\374\2\4\377\374\2\4\377\374"
  "\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\374\2\4\377\374\2\4\377\374"
  "\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\374"
  "\2\4\377\374\2\4\377\374\2\4\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\374\2\4\377\374\2\4\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\374\2\4\377\374\2\4\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\374\2\4\377\374\2\4\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\374\2\4\377\374\2\4\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\374\2\4\377\374\2\4\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\374\2\4\377\374\2\4\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377"
  "\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\374\2\4\377\374\2\4\377\374\2\4\377"
  "\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377\374\2\4\377"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\4\2\374\377\4\2\374\377\4\2\374"
  "\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374"
  "\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\4\2\374\377\4\2\374"
  "\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374"
  "\377\4\2\374\377\4\2\374\377\4\2\374\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\4\2\374\377\4\2\374\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\4\2\374\377\4\2\374\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\4\2\374\377\4\2\374\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\4\2\374\377\4\2\374\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\4\2\374\377\4\2\374\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\4\2\374\377\4\2\374\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2"
  "\374\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374\377\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\4\2\374\377\4\2\374\377\4\2"
  "\374\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2\374\377\4\2"
  "\374\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0",
};



int main()
{
    init = capture_init();
    split = false;
    ds_crop_mode = false;
    frames = 0;
    m_time = sf::Clock();
    sf::RenderWindow window(sf::VideoMode(400, 480), "Cute 3DS Capture");
    window.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);
    window.setFramerateLimit(60);
    sf::RenderWindow bottom_window;
    sf::RectangleShape top_screen(sf::Vector2f(240,400));
    top_screen.rotate(-90);
    top_screen.setPosition(0,240);
    sf::RectangleShape bottom_screen(sf::Vector2f(240,320));
    bottom_screen.rotate(-90);
    bottom_screen.setPosition(40,480);

    sf::View total = sf::View(sf::FloatRect(0, 0, 400, 480));
    sf::View top = sf::View(sf::FloatRect(0, 0, 400, 240));
    sf::View bottom = sf::View(sf::FloatRect(40, 240, 320, 240));
    sf::View ds_crop_combined = sf::View(sf::FloatRect(72, 48, 256, 384));
    sf::View ds_crop_top = sf::View(sf::FloatRect(72, 48, 256, 192));
    sf::View ds_crop_bottom = sf::View(sf::FloatRect(72, 240, 256, 192));


    sf::Texture texture;
    uint8_t tex[] = {0,0,0,255};
    texture.create(int(FRAMEWIDTH),int(FRAMEHEIGHT));
    texture.update(tex,1,1,0,0);

    top_screen.setTexture(&texture);
    top_screen.setTextureRect(sf::IntRect(0,0,240,400));
    bottom_screen.setTexture(&texture);
    bottom_screen.setTextureRect(sf::IntRect(0,400,240,320));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type) {
            case sf::Event::Closed:
                if (bottom_window.isOpen()) {
                    bottom_window.close();
                }
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch(event.key.code) {    
                    case sf::Keyboard::Num1:
                    case sf::Keyboard::Num2:
                    case sf::Keyboard::Num3:
                    case sf::Keyboard::Num4:
                    case sf::Keyboard::Num5:
                    case sf::Keyboard::Num6:
                    case sf::Keyboard::Num7:
                    case sf::Keyboard::Num8:
                    case sf::Keyboard::Num9:
                    case sf::Keyboard::Num0:
                    case sf::Keyboard::Numpad1:
                    case sf::Keyboard::Numpad2:
                    case sf::Keyboard::Numpad3:
                    case sf::Keyboard::Numpad4:
                    case sf::Keyboard::Numpad5:
                    case sf::Keyboard::Numpad6:
                    case sf::Keyboard::Numpad7:
                    case sf::Keyboard::Numpad8:
                    case sf::Keyboard::Numpad9:
                    case sf::Keyboard::Numpad0:
                    // Zoom adjustment when top or main window is focused
                        if (!ds_crop_mode) {
                            window.setSize(sf::Vector2u(
                                (200 * keycode_zoom_map[event.key.code] + 200),
                                (3 * (200 * keycode_zoom_map[event.key.code] + 200) / 5) * (2-int(split))
                            ));
                        } else {
                            window.setSize(sf::Vector2u(
                                (128 * keycode_zoom_map[event.key.code] + 128),
                                (3 * (128 * keycode_zoom_map[event.key.code] + 128) / 4) * (2-int(split))
                            ));
                        }
                    
                        break;
                case sf::Keyboard::C:
                // Switch to/from crop mode
                if (!ds_crop_mode) {
                    if (!split) {
                        window.setView(ds_crop_combined);
                        window.setSize(sf::Vector2u(256, 384));
                    } else {
                        window.setView(ds_crop_top);
                        window.setSize(sf::Vector2u(256, 192));
                        bottom_window.create(sf::VideoMode(256, 192), "Bottom Screen Cute 3DS Capture");
                        bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                        bottom_window.setView(ds_crop_bottom);
                    }
                    ds_crop_mode = true;
                } else {
                    if (!split) {
                        window.setView(total);
                        window.setSize(sf::Vector2u(400, 480));
                    } else {
                        window.setView(top);
                        window.setSize(sf::Vector2u(400, 240));
                        bottom_window.create(sf::VideoMode(320, 240), "Bottom Screen Cute 3DS Capture");
                        bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                        bottom_window.setView(bottom);
                    }
                    ds_crop_mode = false;
                }
                    break;
                 case sf::Keyboard::Space:
                    if (!split) {
                        if (!ds_crop_mode) {
                            window.setView(top);
                            window.setSize(sf::Vector2u(400, 240));
                            bottom_window.create(sf::VideoMode(320, 240), "Bottom Screen Cute 3DS Capture");
                            bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                            bottom_window.setView(bottom);
                        } else {
                            window.setView(ds_crop_top);
                            window.setSize(sf::Vector2u(256, 192));
                            bottom_window.create(sf::VideoMode(256, 192), "Bottom Screen Cute 3DS Capture");
                            bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                            bottom_window.setView(ds_crop_bottom);
                        }
                        split = true;
                    } else {
                        if (!ds_crop_mode) {
                            window.setView(total);
                            window.setSize(sf::Vector2u(400, 480));
                        } else {
                            window.setView(ds_crop_combined);
                            window.setSize(sf::Vector2u(256, 384));
                        }
                        bottom_window.close();
                        split = false;
                    }
                    break;
                default:
                    break;
                }

            default:
                break;
            }

        }

        if (bottom_window.isOpen()) {
            while (bottom_window.pollEvent(event))
            {
                switch(event.type) {
                case sf::Event::Closed:
                    bottom_window.close();
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch(event.key.code) {
                    case sf::Keyboard::Num1:
                    case sf::Keyboard::Num2:
                    case sf::Keyboard::Num3:
                    case sf::Keyboard::Num4:
                    case sf::Keyboard::Num5:
                    case sf::Keyboard::Num6:
                    case sf::Keyboard::Num7:
                    case sf::Keyboard::Num8:
                    case sf::Keyboard::Num9:
                    case sf::Keyboard::Num0:
                    case sf::Keyboard::Numpad1:
                    case sf::Keyboard::Numpad2:
                    case sf::Keyboard::Numpad3:
                    case sf::Keyboard::Numpad4:
                    case sf::Keyboard::Numpad5:
                    case sf::Keyboard::Numpad6:
                    case sf::Keyboard::Numpad7:
                    case sf::Keyboard::Numpad8:
                    case sf::Keyboard::Numpad9:
                    case sf::Keyboard::Numpad0:
                    // Zoom adjustment when bottom window is focused
                        if (!ds_crop_mode) {
                            bottom_window.setSize(sf::Vector2u(
                                (160 * keycode_zoom_map[event.key.code] + 160),
                                (3 * (160 * keycode_zoom_map[event.key.code] + 160) / 4)
                            ));
                        } else {
                            bottom_window.setSize(sf::Vector2u(
                                (128 * keycode_zoom_map[event.key.code] + 128),
                                (3 * (128 * keycode_zoom_map[event.key.code] + 128) / 4) * (2-int(split))
                            ));
                        }
                    
                        break;
                    case sf::Keyboard::C:
                        if (!ds_crop_mode) {
                            if (!split) {
                                window.setView(ds_crop_combined);
                                window.setSize(sf::Vector2u(256, 384));
                            } else {
                                window.setView(ds_crop_top);
                                window.setSize(sf::Vector2u(256, 192));
                                bottom_window.create(sf::VideoMode(256, 192), "Bottom Screen Cute 3DS Capture");
                                bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                                bottom_window.setView(ds_crop_bottom);
                            }
                            ds_crop_mode = true;
                        } else {
                            if (!split) {
                                window.setView(total);
                                window.setSize(sf::Vector2u(400, 480));
                            } else {
                                window.setView(top);
                                window.setSize(sf::Vector2u(400, 240));
                                bottom_window.create(sf::VideoMode(320, 240), "Bottom Screen Cute 3DS Capture");
                                bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                                bottom_window.setView(bottom);
                            }
                            ds_crop_mode = false;
                        }
                        break;
                    case sf::Keyboard::Space:
                        if (!split) {
                            if (!ds_crop_mode) {
                                window.setView(top);
                                window.setSize(sf::Vector2u(400, 240));
                                bottom_window.create(sf::VideoMode(320, 240), "Bottom Screen Cute 3DS Capture");
                                bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                                bottom_window.setView(bottom);
                            } else {
                                window.setView(ds_crop_top);
                                window.setSize(sf::Vector2u(256, 192));
                                bottom_window.create(sf::VideoMode(256, 192), "Bottom Screen Cute 3DS Capture");
                                bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                                bottom_window.setView(ds_crop_bottom);
                            }
                            split = true;
                        } else {
                            if (!ds_crop_mode) {
                                window.setView(total);
                                window.setSize(sf::Vector2u(400, 480));
                            } else {
                                window.setView(ds_crop_combined);
                                window.setSize(sf::Vector2u(256, 384));
                            }
                            bottom_window.close();
                            split = false;
                        }
                        break;
                    default:
                        break;
                    }

                default:
                    break;
                }

            }
        }

        frames++;
        if (m_time.getElapsedTime().asMilliseconds() > 250) {
            char title[50];
            if (!split) {
                sprintf(title,"Cute 3DS Capture (%.2f FPS)", float(frames)/(m_time.getElapsedTime().asSeconds()));
            } else {
                sprintf(title,"Top Screen Cute 3DS Capture (%4.2f FPS)", float(frames)/(m_time.getElapsedTime().asSeconds()));
            }
            m_time.restart();
            frames = 0;
            window.setTitle(title);
        }

        window.clear();
        if(split) {
            bottom_window.clear();
        }

        if(!init) {
            init = capture_init();
        }

#ifdef DS
        uint16_t frameBuf[FRAMEBUFSIZE];
        uint16_t rgbaBuf[FRAMESIZE*4/3];
#else
        uint8_t frameBuf[FRAMEBUFSIZE];
        uint8_t rgbaBuf[FRAMESIZE*4/3];
#endif
        if(init){
            switch(capture_grabFrame(frameBuf)) {
            case CAPTURE_OK:
                toRGBA(frameBuf,rgbaBuf,FRAMESIZE/3);
                texture.update(rgbaBuf,int(FRAMEWIDTH),int(FRAMEHEIGHT),0,0);
                break;
            case CAPTURE_ERROR:
                capture_deinit();
                init = false;
                break;
            /*case CAPTURE_SKIP:
                break;*/
            default:
                break;
            }
        }

        window.draw(top_screen);
        if (!split) {
            window.draw(bottom_screen);
        } else {
            bottom_window.draw(bottom_screen);
            bottom_window.display();
        }


        window.display();
    }
    capture_deinit();
    return 0;
}
