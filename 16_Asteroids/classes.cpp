#ifndef _CLASSES_CPP
#define _CLASSES_CPP

#include <SFML/Graphics.hpp>

class Animation
{
public:
  float frameNumber, speed;
  sf::Sprite sprite;
  std::vector<sf::IntRect> frames;

  Animation() {}
  Animation(sf::Texture &t, int x, int y, int w, int h, int count, float speed)
  {
    frameNumber = 0;
    this->speed = speed;

    for (int i = 0; i < count; i++)
      frames.push_back(sf::IntRect(x + i * w, y, w, h));

    sprite.setTexture(t);
    sprite.setOrigin(w / 2, h / 2);
    sprite.setTextureRect(frames[0]);
  }

  void update()
  {
    frameNumber += speed;
    int n = frames.size();
    if (frameNumber >= n)
      frameNumber -= n;
    sprite.setTextureRect(frames[(int)frameNumber]);
  }

  bool isEnd()
  {
    return frameNumber + speed >= frames.size();
  }
};

class Entity
{
public:
  float x, y, dx, dy, R, angle;
  bool life;
  std::string name;
  Animation anim;

  Entity()
  {
    life = 1;
  }

  void settings(Animation &a, int X, int Y, float Angle = 0, int radius = 1)
  {
    anim = a;
    x = X;
    y = Y;
    angle = Angle;
    R = radius;
  }

  virtual void update(){};

  void draw(sf::RenderWindow &app)
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

  virtual ~Entity(){};
};

class asteroid : public Entity
{
public:
  asteroid()
  {
    //do {
      dx = rand() % 8 - 4;
      dy = rand() % 8 - 4;
    //} while (dx == 0 && dy == 0);

    name = "asteroid";
  }

  void update() {
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
};

class bullet : public Entity
{
public:
  bullet()
  {
    name = "bullet";
  }

  void update()
  {
    dx = cos(angle * DEGTORAD) * 6;
    dy = sin(angle * DEGTORAD) * 6;
    x += dx;
    y += dy;

    if (x > W || x < 0 || y > H || y < 0)
      life = 0;
  }
};

class player : public Entity
{
public:
  bool thrust;
  bool brake;
  std::string tilting;

  player() {
    name = "player";
    tilting = "nope";
  }

  void update()
  {
    if (thrust) {
      dx += cos(angle * DEGTORAD) * 0.2;
      dy += sin(angle * DEGTORAD) * 0.2;
    }
    else if (brake) {
      dx *= 0.95;
      dy *= 0.95;
    }
    else {
      dx *= 0.99;
      dy *= 0.99;
    }

    //sprite.setTextureRect(frames[(int)frameNumber]);

    int maxSpeed = 15;
    float speed = sqrt(dx * dx + dy * dy);
    if (speed > maxSpeed) {
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
};

#endif