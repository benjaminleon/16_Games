#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <SFML/Graphics.hpp>

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;

std::list<class Entity *> entities;

sf::Texture t1, t2, t3, t4, t5, t6, t7;

sf::RenderWindow app(sf::VideoMode(W, H), "Asteroids!");

#endif