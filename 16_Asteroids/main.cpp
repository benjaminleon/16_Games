#include <time.h>
#include <list>
#include "helping_functions.cpp"

using namespace sf;

int main()
{
  srand(time(0));

  app.setFramerateLimit(60);

  t1.loadFromFile("images/spaceship.png");
  t2.loadFromFile("images/background.jpg");
  t3.loadFromFile("images/explosions/type_C.png");
  t4.loadFromFile("images/rock.png");
  t5.loadFromFile("images/fire_blue.png");
  t6.loadFromFile("images/rock_small.png");
  t7.loadFromFile("images/explosions/type_B.png");
  t8.loadFromFile("images/explosions/type_D.png");

  t1.setSmooth(true);
  t2.setSmooth(true);

  sf::Sprite background(t2);

  Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
  Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
  Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
  Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
  Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
  Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
  Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);
  Animation sPlayer_tilt_right(t1, 80, 0, 40, 40, 1, 0);
  Animation sPlayer_tilt_left(t1, 0, 0, 40, 40, 1, 0);
  Animation sPlayer_tilt_right_go(t1, 80, 40, 40, 40, 1, 0);
  Animation sPlayer_tilt_left_go(t1, 0, 40, 40, 40, 1, 0);
  Animation sBomb(t8, 0, 0, 256, 256, 19, 1);

  spawnAsteroids(0, sRock);

  player *p = new player();
  p->settings(sPlayer, 200, 200, 0, 20);
  entities.push_back(p);

  /////main loop/////
  while (app.isOpen())
  {
    Event event;
    while (app.pollEvent(event))
    {
      if (event.type == Event::Closed)
        app.close();

      if (event.type == Event::KeyPressed)
        if (event.key.code == Keyboard::Space)
        {
          bullet * b = new bullet();
          b->settings(sBullet, p->x, p->y, p->angle, 10);
          entities.push_back(b);
        }
        else if (event.key.code == Keyboard::LControl)
        {
          bomb * b = new bomb();
          b->settings(sBomb, p->x, p->y);
          entities.push_back(b);
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
      p->angle += 3;
      p->tilting = "right";
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left))
    {
      p->angle -= 3;
      p->tilting = "left";
    }
    else
      p->tilting = "nope";

    if (Keyboard::isKeyPressed(Keyboard::Up))
    {
      p->thrust = true;
    }
    else
    {
      p->thrust = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::Down))
      p->brake = true;
    else
      p->brake = false;

    for (auto a : entities)
      for (auto b : entities)
      {
        if (a->name == "asteroid" && ( (b->name == "bullet") || (b->name == "bomb")) )
          if (isCollide(a, b))
          {
            a->life = false;
            b->life = false;

            Entity *e = new Entity();
            e->settings(sExplosion, a->x, a->y);
            e->name = "explosion";
            entities.push_back(e);

            for (int i = 0; i < 2; i++) // why two times?
            {
              if (a->R == 15)
                continue;
              Entity *e = new asteroid();
              e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
              entities.push_back(e);
            }
          }

        if (a->name == "player" && b->name == "asteroid")
          if (isCollide(a, b))
          {
            b->life = false;

            Entity *e = new Entity();
            e->settings(sExplosion_ship, a->x, a->y);
            e->name = "explosion";
            entities.push_back(e);

            p->settings(sPlayer, W / 2, H / 2, 0, 20);
            p->dx = 0;
            p->dy = 0;
          }
      }

    // Set picture for space ship
    if (p->thrust)
    {
      if (p->tilting == "right")
      {
        p->anim = sPlayer_tilt_right_go;
      }
      else if (p->tilting == "left")
      {
        p->anim = sPlayer_tilt_left_go;
      }
      else
        p->anim = sPlayer_go;
    }
    else
    {
      if (p->tilting == "right")
      {
        p->anim = sPlayer_tilt_right;
      }
      else if (p->tilting == "left")
      {
        p->anim = sPlayer_tilt_left;
      }
      else
        p->anim = sPlayer;
    }

    removeFinishedExplosions();

    randomlySpawnAsteroid(sRock, sRock_small);

    updateEntitiesAndDeleteTheDead();

    drawEverythingOn(background);
  }

  return 0;
}
