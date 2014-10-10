#include <stdio.h>
extern "C" {
    #include "3dscapture.h"
}
#include <SFML/Graphics.hpp>

static sf::Clock m_time;
static uint frames;

void toRGBA(const uint8_t* rgb, uint8_t* rgba, const size_t count) {
    //printf("Hallo");
    size_t i;
    for (i=0; i<count; i++) {
        rgba[4*i] = rgb[3*i];
        rgba[4*i+1] = rgb[3*i+1];
        rgba[4*i+2] = rgb[3*i+2];
        rgba[4*i+3] = 255;
    }
}

int main()
{
    if(!capture_init()) {
        printf("capture_init() error\n");
        capture_deinit();
        return -1;
    }
    frames = 0;
    m_time = sf::Clock();
    sf::RenderWindow window(sf::VideoMode(400, 480), "Cute 3DS Capture");//,sf::Style::Close);
    sf::RectangleShape top_screen(sf::Vector2f(240,400));
    top_screen.rotate(-90);
    top_screen.setPosition(0,240);
    sf::RectangleShape bottom_screen(sf::Vector2f(240,320));
    bottom_screen.rotate(-90);
    bottom_screen.setPosition(40,480);


    sf::Texture texture;

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
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch(event.key.code) {
                case sf::Keyboard::Num1:
                    window.setSize(sf::Vector2u(400, 480));
                    break;
                case sf::Keyboard::Num2:
                    window.setSize(sf::Vector2u(600, 720));
                    break;
                case sf::Keyboard::Num3:
                    window.setSize(sf::Vector2u(800, 960));
                    break;
                case sf::Keyboard::Num4:
                    window.setSize(sf::Vector2u(1000, 1200));
                    break;
                case sf::Keyboard::Num5:
                    window.setSize(sf::Vector2u(1200, 1680));
                    break;
                case sf::Keyboard::Space:
                    //toggel double window
                    break;
                default:
                    break;
                }

            default:
                break;
            }

        }

        frames++;
        if (m_time.getElapsedTime().asMilliseconds() > 250) {
            char title[40];
            sprintf(title,"Cute 3DS Capture (%.2f FPS)", float(frames)/(m_time.getElapsedTime().asSeconds()));
            m_time.restart();
            frames = 0;
            window.setTitle(title);
        }

        window.clear();

        uint8_t frameBuf[FRAMEBUFSIZE];
        uint8_t rgbaBuf[FRAMESIZE*4/3];
        int frameResult;

        frameResult = capture_grabFrame(frameBuf);
        if(frameResult != CAPTURE_OK) {
            //printf("capture_grapFrame() error\n");
            //return;
            goto error;
        }


        toRGBA(frameBuf,rgbaBuf,FRAMESIZE/3);

        texture.create(int(FRAMEWIDTH),int(FRAMEHEIGHT));

        texture.update(rgbaBuf,int(FRAMEWIDTH),int(FRAMEHEIGHT),0,0);

        //texture.loadFromFile("out.png");

        error:
        window.draw(top_screen);
        window.draw(bottom_screen);
        window.display();
    }
    capture_init();
    return 0;
}


