#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <utility>
#include <vector>
#include <memory>
#include "shape.hpp"

class Ball;
class Paddle;

class GameObject
{
public:
  Shape* shape;
  bool deleted;

  GameObject();
  virtual ~GameObject();
  void virtual handle_collision(Ball &ball) = 0;
  bool check_ball_collision(const Circle& circle) const;
  void set_vertex_array(sf::VertexArray &array, int index);
  sf::IntRect virtual get_textureRect();
  
  
};


class Ball : public GameObject
{
public:
  static sf::IntRect textureRect;
  sf::Vector2f direction_vector;
  float speed;

  Ball(sf::Vector2f position, float radius, float speed,  sf::Vector2f p_direction_vector);
  Ball(const Ball& other);
  sf::Vector2f next_position() const;
  void handle_collision(Ball &ball);
  void move();
  sf::IntRect get_textureRect();
};

class Wall : public GameObject
{
public:
  Wall(sf::Vector2f point1, sf::Vector2f point2);
  Wall(const Wall& other);
  void virtual handle_collision(Ball& ball);
  void handle_collision(Paddle &paddle);
};

class Pit : public GameObject
{
  public:
  bool isGameOver;

  Pit(sf::Vector2f point1, sf::Vector2f point2);
  Pit(const Pit& other);
  void virtual handle_collision(Ball& ball);
};

class Brick : public GameObject
{
  public:
  static sf::IntRect textureRect;
  Brick();
  Brick(sf::Vector2f left_corner, float height, float width);
  Brick(const Brick& other);
  void virtual handle_collision(Ball& ball);
  sf::IntRect get_textureRect();
};

class Paddle : public GameObject
{
  public:
  static sf::IntRect textureRect;
  sf::Vector2f direction_vector;
  float speed;
  std::vector<sf::Vector2f> bounce_zones;
  sf::IntRect get_textureRect();
  
  
  Paddle(sf::Vector2f left_corner, float height, float width, sf::Vector2f p_direction_vector, float p_speed);
  Paddle(const Paddle& other);
  sf::Vector2f next_position() const;
  void move();

  void virtual handle_collision(Ball& ball);
};

void bounce(Ball &ball, const Segment &segment);



