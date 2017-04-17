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

void Entity::set_state(int X, int Y, float Angle, int radius)
{
  x = X;
  y = Y;
  angle = Angle;
  R = radius;
}

void Entity::set_animation(Animation &a)
{
  anim = a;
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

  int W = Game::getInstance()->getWidth();
  int H = Game::getInstance()->getHeight();

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

  if (x > Game::getInstance()->getWidth() || x < 0 ||
      y > Game::getInstance()->getHeight() || y < 0)
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

  int W = Game::getInstance()->getWidth();
  int H = Game::getInstance()->getHeight();

  if (x > W)
    x = 0;
  if (x < 0)
    x = W;
  if (y > H)
    y = 0;
  if (y < 0)
    y = H;
}

Game *Game::getInstance()
{
  if (!instance_)
  {
    instance_ = (std::unique_ptr<Game>)new Game();
    //instance_ = (Game *) new Game();
  }
  return instance_.get();
  //return instance_;
}

Game::Game() : W(1800), H(1200)
{
  sf::RenderWindow app(sf::VideoMode(1200, 800), "Asteroids!");
}

std::list<Entity *> Game::getEntities()
{
    return entities;
}

sf::RenderWindow * Game::getApp(){
  return &app;
}

int Game::getWidth()
{
  return W;
}

int Game::getHeight()
{
  return H;
}

#endif
