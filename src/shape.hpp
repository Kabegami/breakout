#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <utility>
#include <ostream>

sf::Vector2f compute_bounce(const sf::Vector2f& direction, const sf::Vector2f& normal);

enum class Direction { Left, Right, Top, Bottom, None };
std::ostream &operator<<(std::ostream &flux, Direction const &dir);

class Circle;
class Segment;

class Shape{
public:
  // by setting the virtual function = 0 we say this is a pure virtual function
  // threrefore decedents must implement it and we can't create a Shape member !
  sf::Vector2f virtual get_position() const;
  void virtual move(sf::Vector2f next_position);
  
  bool virtual check_collision(const Circle& circle) const = 0;
  void virtual set_vertex_array(sf::VertexArray &array, int index, sf::IntRect textureRect);
  int virtual get_vertex_size();

  virtual ~Shape();
};

class Segment : public Shape
{
public:
  sf::Vector2f point1;
  sf::Vector2f point2;
  
  Segment();
  Segment(sf::Vector2f p1, sf::Vector2f p2);

  bool check_collision(const Segment& other) const;
  bool check_line_collision (sf::Vector2f p1, sf::Vector2f p2) const;
  
  bool check_collision(const Circle& circle) const;
};


class Line : public Shape
{
public:
  sf::Vector2f origin;
  sf::Vector2f direction;
  
  Line(sf::Vector2f origin, sf::Vector2f direction);
  Line(Segment segment);
  Line();

  sf::Vector2f get_impact_point(const sf::Vector2f &C) const;
  sf::Vector2f get_normale(const sf::Vector2f &C) const;
  
  bool check_collision(const Circle& circle) const;
};




class Circle : public Shape
{
  public:
  float radius;
  float diameter;
  sf::Vector2f position;
  sf::Vector2f left_corner;

  Circle();
  Circle(sf::Vector2f p_position,float p_radius);
  // virtual ~Circle();
  void move(sf::Vector2f new_position);
  sf::Vector2f get_position() const;
  bool check_collision(const sf::Vector2f& point) const ;
  bool check_collision(const Line& line) const ;
  bool check_collision(const Segment& segment) const ;
  bool check_collision(const Circle& circle) const;
  void set_vertex_array(sf::VertexArray &array, int index, sf::IntRect textureRect);
  int get_vertex_size();

};

class RectSegment : public Shape
{
  public:
  sf::Vector2f left_corner;
  float height;
  float width;
  Segment left;
  Segment right;
  Segment top;
  Segment bottom;

  RectSegment();
  RectSegment(sf::Vector2f p_left_corner, float p_height, float p_width);
  void move(sf::Vector2f next_position);
  sf::Vector2f get_position() const;
  
  bool check_collision(const Circle& circle) const;
  bool check_collision(const Segment& segment) const;
  void set_vertex_array(sf::VertexArray &array, int index, sf::IntRect textureRect);
  int get_vertex_size();
};



