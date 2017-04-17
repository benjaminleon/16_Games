class Animation
{
  public:
    float frameNumber, speed;
    sf::Sprite sprite;
    std::vector<sf::IntRect> frames;

    Animation();
    Animation(sf::Texture &t, int x, int y, int w, int h, int count, float speed);
    void update();
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
    void settings(Animation &a, int X, int Y, float Angle, int radius);
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