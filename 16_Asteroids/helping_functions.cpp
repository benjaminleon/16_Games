#include "global.h"
#include "classes.cpp"
#include <iostream>

void removeFinishedExplosions()
{
  for (auto e : entities)
    if (e->name == "explosion" || e->name == "bomb")
      if (e->anim.isEnd())
        e->life = 0;
}

void randomlySpawnAsteroid(Animation sRock, Animation sRock_small)
{
  if (rand() % 150 == 0)
  {
    asteroid *a = new asteroid();

    if (rand() % 10 < 8)
      a->settings(sRock, 0, rand() % H, rand() % 360, 25);
    else
      a->settings(sRock_small, 0, rand() % H, rand() % 360, 25);
    entities.push_back(a);
  }
}

void updateEntitiesAndDeleteTheDead()
{
  for (auto i = entities.begin(); i != entities.end();)
  {
    Entity *e = *i;

    e->update();
    e->anim.update();

    if (e->life == false)
    {
      i = entities.erase(i);
      delete e;
    }
    else
      i++;
  }
}

void drawEverythingOn(sf::Sprite background)
{
  app.draw(background);

  for (auto i : entities)
    i->draw(app);

  app.display();
}

bool isCollide(Entity *a, Entity *b)
{
  return (b->x - a->x) * (b->x - a->x) +
             (b->y - a->y) * (b->y - a->y) <
         (a->R + b->R) * (a->R + b->R);
}

void spawnAsteroids(int number, Animation sRock)
{
  for (int i = 0; i < number; i++)
  {
    asteroid *a = new asteroid();
    a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
    entities.push_back(a);
  }
}
