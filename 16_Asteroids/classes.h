#ifndef _CLASSES_H
#define _CLASSES_H

#include <memory>
#include <list>
#include <SFML/Graphics.hpp>

class Animation
{
public:
  float frameNumber, speed;
  sf::Sprite sprite;
  std::vector<sf::IntRect> frames;

  Animation();
  Animation(sf::Texture &t, int x, int y, int w, int h, int count, float speed);
  virtual void update();
  bool isEnd();
};

class Entity
{
public:
  float x, y, dx, dy, R, angle;
  bool life;
  std::string name;
  Animation anim;

  Entity();
  void set_state(int X, int Y, float Angle = 0, int radius = 1);
  void set_animation(Animation &a);
  virtual void update();
  void draw(sf::RenderWindow &app);
  virtual ~Entity();
};

class asteroid : public Entity
{
public:
  asteroid();
  void update();
};

class bullet : public Entity
{
public:
  bullet();
  void update();
};

class bomb : public Entity
{
public:
  bomb();
  void update();
};

class player : public Entity
{
public:
  bool thrust;
  bool brake;
  std::string tilting;
  int bombCoolDown;

  player();
  void update();
};

class coolDownAnimation
{
public:
  float x, y;
  Animation anim;

  coolDownAnimation(float, float);
  void set_animation(Animation &a);
  void update();
  void draw(sf::RenderWindow &app);
};

class Game
{
public:
  static Game *getInstance();
  int getWidth();
  int getHeight();
  sf::RenderWindow *getApp();
  std::list<class Entity *> *getEntities();

private:
  //static std::unique_ptr<Game> instance_;
  static Game *instance_;
  Game();
  const int W;
  const int H;
  sf::RenderWindow *app;
  std::list<class Entity *> *entities;
};

#endif
