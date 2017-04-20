#ifndef HELPING_FUNCTIONS_H
#define HELPING_FUNCTIONS_H

#include "classes.h"

void randomlySpawnAsteroid(Animation, Animation);
void removeFinishedExplosions();
void updateEntitiesAndDeleteTheDead();

void drawEverythingOn(sf::Sprite background, coolDownAnimation *c);
bool isCollide(Entity *a, Entity *b);
void spawnAsteroids(int number, Animation sRock);

#endif
