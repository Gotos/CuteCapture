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
#include <unistd.h>
extern "C" {
#ifdef DS
    #include "dscapture.h"
#else
    #include "3dscapture.h"
#endif
}
#include "favicon.h"
#include "helpers.h"
#include <SFML/Graphics.hpp>
#include <map>


static sf::Clock m_time;
static uint frames;
static bool split;
#ifndef DS
static bool ds_crop_mode;
#endif
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

int main()
{
    init = capture_init();
    split = false;
#ifndef DS
    ds_crop_mode = false;
#endif
    frames = 0;
    m_time = sf::Clock();
#ifdef DS
    auto window_title = "Cute DS Capture";
#else
    auto window_title = "Cute 3DS Capture";
#endif
    char bottom_screen_title[40];
    sprintf(bottom_screen_title, "Bottom Screen %s", window_title);

#ifdef DS
    sf::RenderWindow window(sf::VideoMode(FRAMEWIDTH, FRAMEHEIGHT*2), window_title);
#else
    sf::RenderWindow window(sf::VideoMode(400, 480), window_title);
#endif
    window.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);
    sf::RenderWindow bottom_window;

#ifdef DS
    sf::RectangleShape top_screen(sf::Vector2f(FRAMEWIDTH, FRAMEHEIGHT));
    sf::RectangleShape bottom_screen(sf::Vector2f(FRAMEWIDTH, FRAMEHEIGHT));
    bottom_screen.setPosition(0,FRAMEHEIGHT);

    sf::View total = sf::View(sf::FloatRect(0, 0, FRAMEWIDTH, FRAMEHEIGHT*2));
    sf::View top = sf::View(sf::FloatRect(0, 0, FRAMEWIDTH, FRAMEHEIGHT));
    sf::View bottom = sf::View(sf::FloatRect(0, FRAMEHEIGHT, FRAMEWIDTH, FRAMEHEIGHT));
#else
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
#endif


    sf::Texture texture;
    uint8_t tex[] = {0,0,0,255};
#ifdef DS
    texture.create(int(FRAMEWIDTH),int(FRAMEHEIGHT) * 2);
#else
    texture.create(int(FRAMEWIDTH),int(FRAMEHEIGHT));
#endif
    texture.update(tex,1,1,0,0);

#ifdef DS
    top_screen.setTexture(&texture);
    top_screen.setTextureRect(sf::IntRect(0, 0, FRAMEWIDTH, FRAMEHEIGHT));
    bottom_screen.setTexture(&texture);
    bottom_screen.setTextureRect(sf::IntRect(0, FRAMEHEIGHT, FRAMEWIDTH, FRAMEHEIGHT));
#else
    top_screen.setTexture(&texture);
    top_screen.setTextureRect(sf::IntRect(0,0,240,400));
    bottom_screen.setTexture(&texture);
    bottom_screen.setTextureRect(sf::IntRect(0,400,240,320));
#endif

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
#ifndef DS
                        if (!ds_crop_mode) {
                            window.setSize(sf::Vector2u(
                                (200 * keycode_zoom_map[event.key.code] + 200),
                                (3 * (200 * keycode_zoom_map[event.key.code] + 200) / 5) * (2-int(split))
                            ));
                        } else {
#endif
                            window.setSize(sf::Vector2u(
                                (128 * keycode_zoom_map[event.key.code] + 128),
                                (3 * (128 * keycode_zoom_map[event.key.code] + 128) / 4) * (2-int(split))
                            ));
#ifndef DS
                        }
#endif
                    
                        break;
                case sf::Keyboard::C:
                // Switch to/from crop mode
#ifndef DS
                if (!ds_crop_mode) {
                    if (!split) {
                        window.setView(ds_crop_combined);
                        window.setSize(sf::Vector2u(256, 384));
                    } else {
                        window.setView(ds_crop_top);
                        window.setSize(sf::Vector2u(256, 192));
                        bottom_window.create(sf::VideoMode(256, 192), bottom_screen_title);
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
                        bottom_window.create(sf::VideoMode(320, 240), bottom_screen_title);
                        bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                        bottom_window.setView(bottom);
                    }
                    ds_crop_mode = false;
                }
#endif
                    break;
                 case sf::Keyboard::Space:
                    if (!split) {
#ifndef DS
                        if (!ds_crop_mode) {
                            window.setView(top);
                            window.setSize(sf::Vector2u(400, 240));
                            bottom_window.create(sf::VideoMode(320, 240), bottom_screen_title);
                            bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                            bottom_window.setView(bottom);
                        } else {
                            window.setView(ds_crop_top);
                            window.setSize(sf::Vector2u(256, 192));
                            bottom_window.create(sf::VideoMode(256, 192), bottom_screen_title);
                            bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                            bottom_window.setView(ds_crop_bottom);
                        }
#else
                        window.setView(top);
                        window.setSize(sf::Vector2u(FRAMEWIDTH, FRAMEHEIGHT));
                        bottom_window.create(sf::VideoMode(FRAMEWIDTH, FRAMEHEIGHT), "Bottom Screen Cute DS Capture");
                        bottom_window.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);
                        bottom_window.setView(bottom);
#endif
                        split = true;
                    } else {
#ifndef DS
                        if (!ds_crop_mode) {
                            window.setView(total);
                            window.setSize(sf::Vector2u(400, 480));
                        } else {
                            window.setView(ds_crop_combined);
                            window.setSize(sf::Vector2u(256, 384));
                        }
#else
                        window.setView(total);
                        window.setSize(sf::Vector2u(FRAMEWIDTH, FRAMEHEIGHT * 2));
#endif
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
#ifndef DS
                        if (!ds_crop_mode) {
                            bottom_window.setSize(sf::Vector2u(
                                (160 * keycode_zoom_map[event.key.code] + 160),
                                (3 * (160 * keycode_zoom_map[event.key.code] + 160) / 4)
                            ));
                        } else {
#endif
                            bottom_window.setSize(sf::Vector2u(
                                (128 * keycode_zoom_map[event.key.code] + 128),
                                (3 * (128 * keycode_zoom_map[event.key.code] + 128) / 4) * (2-int(split))
                            ));
#ifndef DS
                        }
#endif
                    
                        break;
#ifndef DS
                    case sf::Keyboard::C:
                        if (!ds_crop_mode) {
                            if (!split) {
                                window.setView(ds_crop_combined);
                                window.setSize(sf::Vector2u(256, 384));
                            } else {
                                window.setView(ds_crop_top);
                                window.setSize(sf::Vector2u(256, 192));
                                bottom_window.create(sf::VideoMode(256, 192), bottom_screen_title);
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
                                bottom_window.create(sf::VideoMode(320, 240), bottom_screen_title);
                                bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                                bottom_window.setView(bottom);
                            }
                            ds_crop_mode = false;
                        }
                        break;
#endif
                    case sf::Keyboard::Space:
                        if (!split) {
#ifndef DS
                            if (!ds_crop_mode) {
                                window.setView(top);
                                window.setSize(sf::Vector2u(400, 240));
                                bottom_window.create(sf::VideoMode(320, 240), bottom_screen_title);
                                bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                                bottom_window.setView(bottom);
                            } else {
                                window.setView(ds_crop_top);
                                window.setSize(sf::Vector2u(256, 192));
                                bottom_window.create(sf::VideoMode(256, 192), bottom_screen_title);
                                bottom_window.setIcon(sfml_icon.width, sfml_icon.height, sfml_icon.pixel_data);
                                bottom_window.setView(ds_crop_bottom);
                            }
#else
                            window.setView(top);
                            window.setSize(sf::Vector2u(FRAMEWIDTH, FRAMEHEIGHT));
                            bottom_window.create(sf::VideoMode(FRAMEWIDTH, FRAMEHEIGHT), "Bottom Screen Cute DS Capture");
                            bottom_window.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);
                            bottom_window.setView(bottom);
#endif
                            split = true;
                        } else {
#ifndef DS
                            if (!ds_crop_mode) {
                                window.setView(total);
                                window.setSize(sf::Vector2u(400, 480));
                            } else {
                                window.setView(ds_crop_combined);
                                window.setSize(sf::Vector2u(256, 384));
                            }
#else
                            window.setView(total);
                            window.setSize(sf::Vector2u(FRAMEWIDTH, FRAMEHEIGHT * 2));
#endif
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
        if (m_time.getElapsedTime().asMilliseconds() > 500) {
            char title[50];
            if (!split) {
                sprintf(title, "%s (%.2f FPS)", window_title, float(frames)/(m_time.getElapsedTime().asSeconds()));
            } else {
                sprintf(title,"Top Screen %s (%.2f FPS)", window_title, float(frames)/(m_time.getElapsedTime().asSeconds()));
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
            usleep(1000*20); //20 milliseconds
            init = capture_init();
        }

#ifdef DS
        uint16_t frameBuf[FRAMEHEIGHT*FRAMEWIDTH*2];
        uint8_t rgbaBuf[FRAMEHEIGHT*FRAMEWIDTH*2*4];
#else
        uint8_t frameBuf[FRAMEBUFSIZE];
        uint8_t rgbaBuf[FRAMESIZE*4/3];
#endif

        if(init){
#ifdef DS
            if(capture_grabFrame(frameBuf)) {
                toRGBA(rgbaBuf,frameBuf);
                texture.update(rgbaBuf,int(FRAMEWIDTH),int(FRAMEHEIGHT*2),0,0);
            } else {
                capture_deinit();
                init = false;
            }
#else
            switch(capture_grabFrame(frameBuf)) {
            case CAPTURE_OK:
                toRGBA(frameBuf,rgbaBuf);
                texture.update(rgbaBuf,int(FRAMEWIDTH),int(FRAMEHEIGHT),0,0);
                break;
            case CAPTURE_ERROR:
                capture_deinit();
                init = false;
                break;
            default:
                break;
            }
#endif
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
