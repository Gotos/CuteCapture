#include <stdio.h>
extern "C" {
    #include "3dscapture.h"
}
#include <SFML/Graphics.hpp>

static sf::Clock m_time;
static uint frames;
static bool split;
static bool init;

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
    frames = 0;
    m_time = sf::Clock();
    sf::RenderWindow window(sf::VideoMode(400, 480), "Cute 3DS Capture");
	window.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);
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
                    window.setSize(sf::Vector2u(400, 240*(2-int(split))));
                    break;
                case sf::Keyboard::Num2:
                    window.setSize(sf::Vector2u(600, 360*(2-int(split))));
                    break;
                case sf::Keyboard::Num3:
                    window.setSize(sf::Vector2u(800, 480*(2-int(split))));
                    break;
                case sf::Keyboard::Num4:
                    window.setSize(sf::Vector2u(1000, 600*(2-int(split))));
                    break;
                case sf::Keyboard::Num5:
                    window.setSize(sf::Vector2u(1200, 720*(2-int(split))));
                    break;
                case sf::Keyboard::Num6:
                    window.setSize(sf::Vector2u(1400, 840*(2-int(split))));
                    break;
                case sf::Keyboard::Num7:
                    window.setSize(sf::Vector2u(1600, 960*(2-int(split))));
                    break;
                case sf::Keyboard::Num8:
                    window.setSize(sf::Vector2u(1800, 1080*(2-int(split))));
                    break;
                case sf::Keyboard::Num9:
                    window.setSize(sf::Vector2u(2000, 1200*(2-int(split))));
                    break;
                case sf::Keyboard::Num0:
                    window.setSize(sf::Vector2u(2200, 1320*(2-int(split))));
                    break;
                case sf::Keyboard::Space:
                    if (!split) {
                        window.setView(top);
                        window.setSize(sf::Vector2u(400, 240));
                        bottom_window.create(sf::VideoMode(320, 240), "Bottom Screen Cute 3DS Capture");
						bottom_window.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);
                        bottom_window.setView(bottom);
                        split = true;
                    } else {
                        window.setView(total);
                        window.setSize(sf::Vector2u(400, 480));
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
                        bottom_window.setSize(sf::Vector2u(320, 240));
                        break;
                    case sf::Keyboard::Num2:
                        bottom_window.setSize(sf::Vector2u(480, 360));
                        break;
                    case sf::Keyboard::Num3:
                        bottom_window.setSize(sf::Vector2u(640, 480));
                        break;
                    case sf::Keyboard::Num4:
                        bottom_window.setSize(sf::Vector2u(800, 600));
                        break;
                    case sf::Keyboard::Num5:
                        bottom_window.setSize(sf::Vector2u(960, 720));
                        break;
                    case sf::Keyboard::Num6:
                        bottom_window.setSize(sf::Vector2u(1120, 840));
                        break;
                    case sf::Keyboard::Num7:
                        bottom_window.setSize(sf::Vector2u(1280, 960));
                        break;
                    case sf::Keyboard::Num8:
                        bottom_window.setSize(sf::Vector2u(1440, 1080));
                        break;
                    case sf::Keyboard::Num9:
                        bottom_window.setSize(sf::Vector2u(1600, 1200));
                        break;
                    case sf::Keyboard::Num0:
                        bottom_window.setSize(sf::Vector2u(1760, 1320));
                        break;
                    case sf::Keyboard::Space:
                        if (!split) {
                            window.setView(top);
                            window.setSize(sf::Vector2u(400, 240));
                            bottom_window.create(sf::VideoMode(320, 240), "Bottom Screen Cute 3DS Capture");
                            bottom_window.setView(bottom);
                            split = true;
                        } else {
                            window.setView(total);
                            window.setSize(sf::Vector2u(400, 480));
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

        uint8_t frameBuf[FRAMEBUFSIZE];
        uint8_t rgbaBuf[FRAMESIZE*4/3];
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


