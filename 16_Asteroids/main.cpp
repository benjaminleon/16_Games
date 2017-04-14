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

  spawnAsteroids(15, sRock);
  
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
          bullet *b = new bullet();
          b->settings(sBullet, p->x, p->y, p->angle, 10);
          entities.push_back(b);
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Right))
      p->angle += 3;
    if (Keyboard::isKeyPressed(Keyboard::Left))
      p->angle -= 3;
    if (Keyboard::isKeyPressed(Keyboard::Up))
      p->thrust = true;
    else
      p->thrust = false;

    for (auto a : entities)
      for (auto b : entities)
      {
        if (a->name == "asteroid" && b->name == "bullet")
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

    if (p->thrust)
      p->anim = sPlayer_go;
    else
      p->anim = sPlayer;

    removeFinishedExplosions();

    randomlySpawnAsteroid(sRock, sRock_small);

    updateEntitiesAndDeleteTheDead();

    drawEverythingOn(background);
    
  }

  return 0;
}
