#include <SFML/System/Vector2.hpp>
#include <math.h>
#include <vector>
#include <iostream>

#define PI 3.14159265

sf::Vector2f rotate (const sf::Vector2f &vector, float angle){
  // sin and cos functions work with radian. 1 radian = 180 / PI defgree
  float radian_angle = angle * PI / 180;
  float angle_cos = cos(radian_angle);
  float angle_sin = sin(radian_angle);
  float new_x = angle_cos*vector.x - angle_sin*vector.y;
  float new_y = angle_sin*vector.x + angle_cos*vector.y;
  return sf::Vector2f(new_x, new_y);
}


float determinant(const sf::Vector2f &v1, const sf::Vector2f & v2){
  return v1.x*v2.y - v1.y * v2.x;
}

float dot_product(const sf::Vector2f &v1, const sf::Vector2f &v2){
  return (v1.x * v2.x) + (v1.y * v2.y);
}

sf::Vector2f dist_vect(const sf::Vector2f &point1, const sf::Vector2f &point2){
  return sf::Vector2f(point2.x - point1.x, point2.y - point1.y);
}

float norme(const sf::Vector2f &v){
  return sqrt(v.x * v.x + v.y * v.y);
}


sf::Vector2f xdeviate(const sf::Vector2f &vector){
  return sf::Vector2f(-1 * vector.x, vector.y);
}

sf::Vector2f ydeviate(const sf::Vector2f &vector){
  return sf::Vector2f(vector.x, -1 * vector.y);
}

std::vector<sf::Vector2f> get_rect_points(int left, int top, float height, float width){
  sf::Vector2f p1 = sf::Vector2f(left, top);
  sf::Vector2f p2 = sf::Vector2f(left + width, top);
  sf::Vector2f p3 = sf::Vector2f(left + width, top + height);
  sf::Vector2f p4 = sf::Vector2f(left, top + height);

  std::vector<sf::Vector2f> v = {p1, p2, p3, p4};
  return v;
}

std::ostream &operator<<(std::ostream &flux, sf::Vector2f const &vector){
  return flux << "[ " << vector.x << ", " << vector.y << " ]";
}

std::vector<sf::Vector2f> rangex(float start, float end, float step, float y_value){
  auto vector = std::vector<sf::Vector2f>();
  for (float i = start; i < end; i+=step){
    vector.push_back(sf::Vector2f(i, y_value));
  }
  return vector;
}
