#include <time.h>
#include <list>
#include "classes.h"
#include "helping_functions.h"
#include "global.h"

#include <iostream>

using namespace sf;

int main()
{
  srand(time(0));

  Game::getInstance()->getApp()->setFramerateLimit(60);

  sf::Texture t1, t2, t3, t4, t5, t6, t7, t8, t9;
  t1.loadFromFile("images/spaceship.png");
  t2.loadFromFile("images/background.jpg");
  t3.loadFromFile("images/explosions/type_C.png");
  t4.loadFromFile("images/rock.png");
  t5.loadFromFile("images/fire_blue.png");
  t6.loadFromFile("images/rock_small.png");
  t7.loadFromFile("images/explosions/type_B.png");
  t8.loadFromFile("images/explosions/type_D.png");
  t9.loadFromFile("images/bombCoolDown.png");

  t1.setSmooth(true);
  t2.setSmooth(true);

  sf::Sprite background(t2);

  Animation sExplosion(t3, 0, 0, 256, 256, 32, 0.5);
  Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
  Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
  Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
  Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
  Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
  Animation sExplosion_ship(t7, 0, 0, 192, 192, 42, 0.5);
  Animation sPlayer_tilt_right(t1, 80, 0, 40, 40, 1, 0);
  Animation sPlayer_tilt_left(t1, 0, 0, 40, 40, 1, 0);
  Animation sPlayer_tilt_right_go(t1, 80, 40, 40, 40, 1, 0);
  Animation sPlayer_tilt_left_go(t1, 0, 40, 40, 40, 1, 0);
  Animation sBomb(t8, 0, 0, 256, 256, 19, 1);
  Animation sCoolDown(t9, 0, 0, 256, 256, NR_OF_CD_FRAMES, 0);

  spawnAsteroids(0, sRock);

  player *p = new player();
  p->set_state(200, 200, 0, 20);
  p->set_animation(sPlayer);
  Game::getInstance()->getEntities()->push_back(p);

  coolDownAnimation *c = new coolDownAnimation(100, 100);
  c->set_animation(sCoolDown);

  /////main loop/////
  while (Game::getInstance()->getApp()->isOpen())
  {
    Event event;
    while (Game::getInstance()->getApp()->pollEvent(event))
    {
      if (event.type == Event::Closed)
        Game::getInstance()->getApp()->close();

      if (event.type == Event::KeyPressed)
        if (event.key.code == Keyboard::Space)
        {
          bullet *b = new bullet();
          b->set_state(p->x, p->y, p->angle, 10);
          b->set_animation(sBullet);
          Game::getInstance()->getEntities()->push_back(b);
        }
        else if (event.key.code == Keyboard::LControl)
        {
          if (p->bombCoolDown == 0)
          {
            p->bombCoolDown = FULL_COOLDOWN;
            bomb *b = new bomb();
            b->set_state(p->x, p->y, 0, 1);
            b->set_animation(sBomb);
            Game::getInstance()->getEntities()->push_back(b);
          }
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

    for (auto a : *(Game::getInstance()->getEntities()))
      for (auto b : *(Game::getInstance()->getEntities()))
      {
        if (a->name == "asteroid" && ((b->name == "bullet") || (b->name == "bomb")))
          if (isCollide(a, b))
          {
            a->life = false;
            if (b->name == "bullet")
              b->life = false;

            Entity *e = new Entity();
            e->set_state((int)a->x, (int)a->y);
            e->set_animation(sExplosion);
            e->name = "explosion";
            Game::getInstance()->getEntities()->push_back(e);

            // A large rock explodes into 2 small ones
            if (a->R != 15)
              for (int i = 0; i < 2; i++)
              {
                Entity *e = new asteroid();
                e->set_state(a->x, a->y, rand() % 360, 15);
                e->set_animation(sRock_small);
                Game::getInstance()->getEntities()->push_back(e);
              }
          }

        if (a->name == "player" && b->name == "asteroid")
          if (isCollide(a, b))
          {
            b->life = false;

            Entity *e = new Entity();
            e->set_state(a->x, a->y);
            e->set_animation(sExplosion_ship);
            e->name = "explosion";
            Game::getInstance()->getEntities()->push_back(e);

            int W = Game::getInstance()->getWidth();
            int H = Game::getInstance()->getHeight();
            p->set_state(W / 2, H / 2, 0, 20);
            p->set_animation(sPlayer);
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

    c->update();

    drawEverythingOn(background, c);
  }
  return 0;
}
