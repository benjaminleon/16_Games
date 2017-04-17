#ifndef HELPING_FUNCTIONS_CPP
#define HELPING_FUNCTIONS_CPP

#include "helping_functions.h"

#include <iostream>

void removeFinishedExplosions()
{
  auto entities = Game::getInstance()->getEntities();
  for (auto e : entities)
    if (e->name == "explosion" || e->name == "bomb")
      if (e->anim.isEnd())
        e->life = 0;
}

void randomlySpawnAsteroid(Animation sRock, Animation sRock_small)
{
  if (rand() % 150 == 0)
  {
    int H = Game::getInstance()->getHeight();

    asteroid *a = new asteroid();

    a->set_state(0, rand() % H, rand() % 360, 25);

    if (rand() % 10 < 8)
      a->set_animation(sRock);
    else
      a->set_animation(sRock_small);

    Game::getInstance()->getEntities().push_back(a);
  }
}

void updateEntitiesAndDeleteTheDead()
{
  auto entities = Game::getInstance()->getEntities();
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
  sf::RenderWindow * appPtr = Game::getInstance()->getApp();
  appPtr->draw(background);

  for (auto i : Game::getInstance()->getEntities())
    i->draw(*appPtr);

  appPtr->display();
}

bool isCollide(Entity *a, Entity *b)
{
  return (b->x - a->x) * (b->x - a->x) +
             (b->y - a->y) * (b->y - a->y) <
         (a->R + b->R) * (a->R + b->R);
}

void spawnAsteroids(int number, Animation sRock)
{
  int W = Game::getInstance()->getWidth();
  int H = Game::getInstance()->getHeight();
  for (int i = 0; i < number; i++)
  {
    asteroid *a = new asteroid();
    a->set_state(rand() % W, rand() % H, rand() % 360, 25);
    a->set_animation(sRock);
    Game::getInstance()->getEntities().push_back(a);
  }
}

//void updateCoolDownAnimation();

#endif
