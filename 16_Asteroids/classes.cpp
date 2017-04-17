#ifndef _CLASSES_CPP
#define _CLASSES_CPP

#include <SFML/Graphics.hpp>
#include "global.h"
#include "classes.h"


Animation::Animation(){};
Animation::Animation(sf::Texture &t, int x, int y, int w, int h, int count, float speed)
{
  frameNumber = 0;
  this->speed = speed;

  for (int i = 0; i < count; i++)
    frames.push_back(sf::IntRect(x + i * w, y, w, h));

  sprite.setTexture(t);
  sprite.setOrigin(w / 2, h / 2);
  sprite.setTextureRect(frames[0]);
}

void Animation::update()
{
  frameNumber += speed;
  int n = frames.size();
  if (frameNumber >= n)
    frameNumber -= n;
  sprite.setTextureRect(frames[(int)frameNumber]);
}

bool Animation::isEnd()
{
  return frameNumber + speed >= frames.size();
}

Entity::Entity()
{
  life = 1;
}

void Entity::settings(Animation &a, int X, int Y, float Angle = 0, int radius = 1)
{
  anim = a;
  x = X;
  y = Y;
  angle = Angle;
  R = radius;
}

void Entity::update(){};

void Entity::draw(sf::RenderWindow &app)
{
  anim.sprite.setPosition(x, y);
  anim.sprite.setRotation(angle + 90);
  app.draw(anim.sprite);

  sf::CircleShape circle(R);
  circle.setFillColor(sf::Color(255, 0, 0, 170));
  circle.setPosition(x, y);
  circle.setOrigin(R, R);
  //app.draw(circle);
}

Entity::~Entity(){};

asteroid::asteroid()
{
  //do {
  dx = rand() % 8 - 4;
  dy = rand() % 8 - 4;
  //} while (dx == 0 && dy == 0);

  name = "asteroid";
}

void asteroid::update()
{
  x += dx;
  y += dy;

  if (x > W)
    x = 0;
  if (x < 0)
    x = W;
  if (y > H)
    y = 0;
  if (y < 0)
    y = H;
}

bullet::bullet()
{
  name = "bullet";
}

void bullet::update()
{
  dx = cos(angle * DEGTORAD) * 6;
  dy = sin(angle * DEGTORAD) * 6;
  x += dx;
  y += dy;

  if (x > W || x < 0 || y > H || y < 0)
    life = 0;
}

bomb::bomb()
{
  name = "bomb";
}

void bomb::update()
{
  R += 10;
}

player::player()
{
  name = "player";
  tilting = "nope";
  bombCoolDown = 0;
}

void player::update()
{
  if (bombCoolDown > 0)
    bombCoolDown -= 1;

  if (thrust)
  {
    dx += cos(angle * DEGTORAD) * 0.2;
    dy += sin(angle * DEGTORAD) * 0.2;
  }
  else if (brake)
  {
    dx *= 0.95;
    dy *= 0.95;
  }
  else
  {
    dx *= 0.99;
    dy *= 0.99;
  }

  //sprite.setTextureRect(frames[(int)frameNumber]);

  int maxSpeed = 15;
  float speed = sqrt(dx * dx + dy * dy);
  if (speed > maxSpeed)
  {
    dx *= maxSpeed / speed;
    dy *= maxSpeed / speed;
  }

  x += dx;
  y += dy;

  if (x > W)
    x = 0;
  if (x < 0)
    x = W;
  if (y > H)
    y = 0;
  if (y < 0)
    y = H;
}

class Game
{
public:
  Game()
  {
    sf::RenderWindow app(sf::VideoMode(1200, 800), "Asteroids!");
  }
  int getWidth()
  {
    return W;
  }

private:
  sf::RenderWindow app;
  static const int W = 10;
  const int H = 10;
};

#endif