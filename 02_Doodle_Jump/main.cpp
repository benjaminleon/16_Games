#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

#include <iostream>

struct point
{
    int x, y;
    int mode;
};

enum platformType
{
    weak,
    strong,
    boost
};

enum xDirection
{
    left = -1,
    right = 1
};

int main()
{
    srand(time(0));

    const int SCREEN_WIDTH = 400;
    const int SCREEN_HEIGHT = 533;
    RenderWindow app(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Doodle Game!");
    app.setFramerateLimit(60);

    Texture t1, t2, t3, t4, t5;
    t1.loadFromFile("images/background.png");
    t2.loadFromFile("images/platform.png");
    t3.loadFromFile("images/doodle.png");
    t4.loadFromFile("images/platform_yellow.png");
    t5.loadFromFile("images/platform_boost.png");

    const int DIST_TO_RIGHT_FOOT = 50;
    const int DIST_TO_LEFT_FOOT = 20;

    Sprite sBackground(t1), sPlat(t2), sPers(t3), sPlatWeak(t4), sPlatBoost(t5);

    const int NR_OF_PLATFORMS = 8;
    point plat[NR_OF_PLATFORMS];

    // Initialize platforms
    for (int i = 0; i < NR_OF_PLATFORMS; i++)
    {
        plat[i].x = rand() % SCREEN_WIDTH;
        plat[i].y = rand() % SCREEN_HEIGHT;
        plat[i].mode = rand() % 3;  // Three different platforms
    }

    float x = 100;
    int y = SCREEN_HEIGHT, moveHigherThreshold = SCREEN_HEIGHT * 2 / 3;
    float ySpeed = 0;
    bool falling = false;
    int lastHorizontalDir = xDirection::right;
    int coolDown = 0;
    float dx = 0;

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            x += 3;
            lastHorizontalDir = xDirection::right;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            x -= 3;
            lastHorizontalDir = xDirection::left;
        }

        if (Keyboard::isKeyPressed(Keyboard::LControl))
        {
            if (coolDown <= 0)
            {
                coolDown = 100;
                dx = 20 * lastHorizontalDir;
            }
        }
        x += dx;
        dx *= 0.95;
        if (abs(dx) < 0.7)
            dx = 0;

        if (--coolDown < 0)
            coolDown = 0;

        if (x > SCREEN_WIDTH)
            x = -DIST_TO_RIGHT_FOOT;
        else if (x < -DIST_TO_RIGHT_FOOT)
            x = SCREEN_WIDTH - DIST_TO_LEFT_FOOT;

        ySpeed -= 0.2; // Fall faster and faster
        y += ySpeed;

        if (y < 0)
        {
            std::cout << "Game over!\n";
            app.close();
        }

        falling = ((ySpeed < 0) ? true : false);

        if (y > moveHigherThreshold)
            for (int i = 0; i < NR_OF_PLATFORMS; i++)
            {
                y = moveHigherThreshold;
                plat[i].y -= ySpeed;
                if (plat[i].y < 0)
                {
                    plat[i].y = SCREEN_HEIGHT;
                    plat[i].x = rand() % 400;
                }
            }

        int bouncingPlatform;
        for (int i = 0; i < NR_OF_PLATFORMS; i++)
            if ((x + DIST_TO_RIGHT_FOOT > plat[i].x) && (x + DIST_TO_LEFT_FOOT < plat[i].x + 68) && (y - 70 > plat[i].y) && (y - 70 < plat[i].y + 14)) // touching a platform
            {
                bouncingPlatform = i;
                if (falling)
                {
                    if (plat[i].mode != platformType::boost)
                    {
                        ySpeed = 10;

                        if (plat[i].mode == platformType::weak)
                        {
                            plat[i].x = rand() % SCREEN_WIDTH;
                            plat[i].x = SCREEN_HEIGHT;
                        }
                    }
                }
                else
                {
                    if (plat[i].mode == platformType::boost)
                        ySpeed = 10;
                }
            }

        sPers.setPosition(x, SCREEN_HEIGHT - y); // (0, 0) is at top left in sfml. We're using bottom left as (0, 0) in this game

        app.draw(sBackground);
        app.draw(sPers);
        for (int i = 0; i < NR_OF_PLATFORMS; i++)
        {
            int type = plat[i].mode;
            if (type == platformType::weak)
            {
                sPlatWeak.setPosition(plat[i].x, SCREEN_HEIGHT - plat[i].y);
                app.draw(sPlatWeak);
            }
            else if (type == platformType::strong)
            {
                sPlat.setPosition(plat[i].x, SCREEN_HEIGHT - plat[i].y);
                app.draw(sPlat);
            }
            else if (type == platformType::boost)
            {
                sPlatBoost.setPosition(plat[i].x, SCREEN_HEIGHT - plat[i].y);
                app.draw(sPlatBoost);
            }
        }

        app.display();
    }

    return 0;
}
