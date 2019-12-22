#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <utility>
#include <ostream>
#include "shape.hpp"
#include "utils.hpp"

//definition of virtual functions
void Shape::move(sf::Vector2f next_position){}
void Shape::set_vertex_array(sf::VertexArray &array, int index,
                             sf::IntRect textureRect) {}
int Shape::get_vertex_size() { return 0; }

sf::Vector2f Shape::get_position() const { return sf::Vector2f(0, 0); }

Shape::~Shape(){}

sf::Vector2f compute_bounce(const sf::Vector2f& direction, const sf::Vector2f& normal){
  float projection_length = dot_product(direction, normal);
  return direction - 2 * projection_length * normal;
}


Line::Line(){
  origin = sf::Vector2f(0, 0);
  direction = sf::Vector2f(0, 0);
}

Line::Line(sf::Vector2f p_origin, sf::Vector2f p_direction){
  origin = p_origin;
  direction = p_direction;
}

Line::Line(Segment segment){
  origin = segment.point1;
  direction = dist_vect(segment.point1, segment.point2);
}

bool Line::check_collision(const Circle &circle) const {
  return circle.check_collision(*this);
}

// ========================================


Segment::Segment(){
  point1 = sf::Vector2f(0, 0);
  point2 = sf::Vector2f(0, 0);
}

Segment::Segment(sf::Vector2f p1, sf::Vector2f p2){
  point1 = p1;
  point2 = p2;
}

bool Segment::check_line_collision(sf::Vector2f p1, sf::Vector2f p2) const{
  sf::Vector2f line = dist_vect(p1, p2);
  sf::Vector2f side1 = dist_vect(p1, point1);
  sf::Vector2f side2 = dist_vect(p1, point2);
  //check if the determinants of each side have the same sign
  return determinant(line, side1) * determinant(line, side2) < 0;  
}

bool Segment::check_collision(const Segment& other) const {
  return check_line_collision(other.point1, other.point2) && other.check_line_collision(point1, point2);
}

bool Segment::check_collision(const Circle& circle) const {
  return circle.check_collision(*this);
}

// ========================================

RectSegment::RectSegment(){
  left_corner = sf::Vector2f(0, 0);
  height = 0;
  width = 0;
  left = Segment();
  right = Segment();
  top = Segment();
  bottom = Segment();  
}


RectSegment::RectSegment(sf::Vector2f p_left_corner, float p_height, float p_width){
  left_corner = p_left_corner;
  height = p_height;
  width = p_width;
  float x = p_left_corner.x;
  float y = p_left_corner.y;
   
  left = Segment(left_corner, sf::Vector2f(x, y + height));
  right = Segment(sf::Vector2f(x+ width, y), sf::Vector2f(x+ width, y+ height));
  top = Segment(left_corner, sf::Vector2f(x+ width, y));
  bottom = Segment(sf::Vector2f(x, y + height), sf::Vector2f(x + width, y + height));
}

bool RectSegment::check_collision(const Circle &circle) const {
  return circle.check_collision(left) || circle.check_collision(right) || circle.check_collision(top) || circle.check_collision(bottom);
}

bool RectSegment::check_collision(const Segment& segment) const {
  return left.check_collision(segment) || right.check_collision(segment) || bottom.check_collision(segment) || top.check_collision(segment);
  
}

void RectSegment::set_vertex_array(sf::VertexArray &array, int index, sf::IntRect textureRect){
  std::vector<sf::Vector2f> posPoints = get_rect_points(left_corner.x, left_corner.y, height, width);
  std::vector<sf::Vector2f> texturePoints = get_rect_points(textureRect.left, textureRect.top, textureRect.height, textureRect.width);

  for (int i=0; i < 4; i++){
    array[index+i].position = posPoints[i];
    array[index+i].texCoords = texturePoints[i];
  }
  
}

int RectSegment::get_vertex_size() { return 4; }

sf::Vector2f RectSegment::get_position() const { return left_corner; }


void RectSegment::move(sf::Vector2f next_position){
  left_corner = next_position;
  float x = left_corner.x;
  float y = left_corner.y;
  
  left = Segment(left_corner, sf::Vector2f(x, y + height));
  right = Segment(sf::Vector2f(x+ width, y), sf::Vector2f(x+ width, y+ height));
  top = Segment(left_corner, sf::Vector2f(x+ width, y));
  bottom = Segment(sf::Vector2f(x, y + height), sf::Vector2f(x + width, y + height));
}

// ========================================

Circle::Circle(){
  position = sf::Vector2f(0, 0);
  radius = 0;
  diameter = 0;
  left_corner = sf::Vector2f(0, 0);
}

Circle::Circle(sf::Vector2f p_position, float p_radius){
  position = p_position;
  radius = p_radius;
  diameter = 2 * radius;
  left_corner = sf::Vector2f(position.x - radius, position.y - radius);
}

void Circle::move(sf::Vector2f new_position){
  position = new_position;
  left_corner = sf::Vector2f(position.x - radius, position.y - radius);
}

sf::Vector2f Circle::get_position() const { return position; }

bool Circle::check_collision(const sf::Vector2f& point) const {
  sf::Vector2f dif = (point - position);
  float distance = dot_product(dif, dif);
  return distance < (radius * radius);
}


bool Circle::check_collision(const Line& line) const {
  sf::Vector2f OC = dist_vect(line.origin, position);
  float numerator = determinant(line.direction, OC);
  if (numerator < 0) numerator = -numerator;
  float denominator = norme(line.direction);
  float CI = numerator / denominator;
  return CI < radius;
}

bool Circle::check_collision(const Segment& segment) const {
  sf::Vector2f AB = dist_vect(segment.point1, segment.point2);
  sf::Vector2f AC = dist_vect(segment.point1, position);
  sf::Vector2f BC = dist_vect(segment.point2, position);
  Line line = Line(segment);
  if (!check_collision(line)){
    return false;
  }
  if (dot_product(AB, AC) >=0 && dot_product(-AB, BC) >=0){
    return true;
  }

  return (check_collision(segment.point1) || check_collision(segment.point2));
}

bool Circle::check_collision(const Circle& circle) const {
  float distance = norme(dist_vect(position, circle.position));
  return distance < radius + circle.radius;
}

void Circle::set_vertex_array(sf::VertexArray &array, int index, sf::IntRect textureRect){
  std::vector<sf::Vector2f> posPoints = get_rect_points(left_corner.x, left_corner.y, diameter, diameter);
  std::vector<sf::Vector2f> texturePoints = get_rect_points(textureRect.left, textureRect.top, textureRect.height, textureRect.width);

  for (int i=0; i < 4; i++){
    array[index+i].position = posPoints[i];
    array[index+i].texCoords = texturePoints[i];
  }
}

int Circle::get_vertex_size(){return 4;}



sf::Vector2f Line::get_impact_point(const sf::Vector2f &C) const {
  sf::Vector2f AC = dist_vect(origin, C);
  float ti = dot_product(direction, AC) / dot_product(direction, direction);
  sf::Vector2f I = origin + ti * direction;
  return I;
}

sf::Vector2f Line::get_normale(const sf::Vector2f &P) const {
  sf::Vector2f AP = dist_vect(origin, P);
  float det = determinant(direction, AP);
  sf::Vector2f N = sf::Vector2f(-direction.y * det, direction.x * det);
  return N / norme(N);
}






std::ostream &operator<<(std::ostream &flux, Direction const &dir){
  switch (dir) {
  case Direction::Right:
    return flux << "Right";
  case Direction::Left:
    return flux << "Left";
  case Direction::Top:
    return flux << "Top";
  case Direction::Bottom:
    return flux << "Bottom";
  case Direction::None:
    return flux << "None";
      }
  return flux;
}
