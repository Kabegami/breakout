#include "gameObjects.hpp"
#include "shape.hpp"
#include "utils.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <vector>
#include <memory>

void bounce(Ball &ball, const Segment &segment){
  Line line = Line(segment);
  sf::Vector2f ball_position = ball.shape->get_position();
  sf::Vector2f normale = line.get_normale(ball_position);
  sf::Vector2f new_direction = compute_bounce(ball.direction_vector, normale);
  ball.direction_vector = new_direction;
}


GameObject::GameObject(){
  deleted = false;
  shape = NULL;
}


GameObject::~GameObject(){delete shape;}

bool GameObject::check_ball_collision(const Circle &circle) const {
  return shape->check_collision(circle);
}

sf::IntRect GameObject::get_textureRect(){return sf::IntRect(0, 0, 0, 0);}


// ####################

Ball::Ball(sf::Vector2f position, float radius, float p_speed, sf::Vector2f p_direction_vector){
  shape = new Circle(position, radius);
  direction_vector = p_direction_vector;
  speed = p_speed;
}

Ball::Ball(const Ball& other){
  /* The copy constructor is needed to make deep copy. Therefore each gameObject have its own shape */
  Circle* ptn = (Circle *) other.shape;
  shape = new Circle(ptn->position, ptn->radius);
  direction_vector = other.direction_vector;
  speed = other.speed;
}


sf::Vector2f Ball::next_position() const {
  return shape->get_position() + (direction_vector * speed);
}

void Ball::move() { shape->move(next_position()); }

sf::IntRect Ball::get_textureRect() { return Ball::textureRect; }

void Ball::handle_collision(Ball &ball) {}

// ####################

Wall::Wall(sf::Vector2f point1, sf::Vector2f point2){
  shape = new Segment(point1, point2);
}

Wall::Wall(const Wall& other){
  Segment* ptn = (Segment*)(other.shape);
  shape = new Segment(ptn->point1, ptn->point2);
}


void Wall::handle_collision(Ball &ball) {
  Circle* circle_pnt = (Circle *)(ball.shape);
  
  if (!(shape->check_collision(*circle_pnt))){
      return;
    }
  
      
  Segment* segment_pnt = (Segment *)(shape);
  bounce(ball, *segment_pnt);
}

void Wall::handle_collision(Paddle &paddle){
  RectSegment* paddle_ptn = (RectSegment*)(paddle.shape);
  RectSegment paddle_shape = *paddle_ptn;

  Segment* wall_ptn = (Segment*)(shape);
  Segment wall_segment = *wall_ptn;
  
  if (!paddle_shape.check_collision(wall_segment)){
    return;
  }

  paddle.direction_vector = sf::Vector2f(-paddle.direction_vector.x, paddle.direction_vector.y);
  
}

// ####################

Pit::Pit(sf::Vector2f point1, sf::Vector2f point2) {
  shape = new Segment(point1, point2);
  isGameOver = false;
}

Pit::Pit(const Pit& other){
  Segment* ptn = (Segment*)(other.shape);
  shape = new Segment(ptn->point1, ptn->point2);
  isGameOver = other.isGameOver;
}


void Pit::handle_collision(Ball &ball) {
  Circle* circle_pnt = (Circle *)(ball.shape);
  isGameOver = shape->check_collision(*circle_pnt);
}

// ####################

Brick::Brick(){
  shape = new RectSegment();
}


Brick::Brick(sf::Vector2f left_corner, float height, float width){
  shape = new RectSegment(left_corner, height, width);
}

Brick::Brick(const Brick& other){
  RectSegment* ptn = (RectSegment*)(other.shape);
  shape = new RectSegment(ptn->left_corner, ptn->height, ptn->width);
}

void Brick::handle_collision(Ball &ball) {
  RectSegment* pnt = (RectSegment *) (shape);
  Circle circle = *(Circle *)(ball.shape);
  auto handle_segment = [&ball, &circle](const Segment& segment) -> void {
			  if (segment.check_collision(circle)){bounce(ball, segment);}};

  if (!pnt->check_collision(circle)){
    return;
  }

  handle_segment(pnt->bottom);
  handle_segment(pnt->top);
  handle_segment(pnt->left);
  handle_segment(pnt->right);

  deleted = true;

  
}


sf::IntRect Brick::get_textureRect(){return Brick::textureRect;}

// ####################

Paddle::Paddle(sf::Vector2f left_corner, float height, float width, sf::Vector2f p_direction_vector, float p_speed){
  shape = new RectSegment(left_corner, height, width);
  direction_vector = p_direction_vector;
  speed = p_speed;
  bounce_zones = rangex(-0.5, 1, 0.5, -1);
}

Paddle::Paddle(const Paddle &other){
  auto ptn = (RectSegment*)(other.shape);
  shape = new RectSegment(ptn->left_corner, ptn->height, ptn->width);
  direction_vector = other.direction_vector;
  speed = other.speed;
  bounce_zones = other.bounce_zones;
  
}

void Paddle::handle_collision(Ball &ball) {
  RectSegment* pnt = (RectSegment *) (shape);
  Circle circle = *(Circle *)(ball.shape);
  if (!pnt->check_collision(circle)){
    return;
  }
  
  float local_distance = circle.get_position().x - pnt->left_corner.x;
  float block_size = pnt->width / bounce_zones.size();
  int zone_number = (int) (local_distance) / (int) (block_size);
  ball.direction_vector = bounce_zones[zone_number];
}

sf::Vector2f Paddle::next_position() const {
  return shape->get_position() + (direction_vector * speed);
}


void Paddle::move() { shape->move(next_position());}



sf::IntRect Paddle::get_textureRect() { return Paddle::textureRect; }

// ####################

