#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>
#include <vector>
#include "utils.hpp"
#include "shape.hpp"
#include "gameObjects.hpp"

void gameLoop(Ball& ball, std::vector<GameObject*>& gameObjectVector){
  auto update_fn = [&ball](GameObject* &pnt) -> void {
		     if (pnt != NULL){
		     pnt->handle_collision(ball);
		     if (pnt->deleted){
		       // we don't delete the object because the memory address are handled by the stack
		       pnt = NULL;

		       
		     }
		     }
		   };
  std::for_each(gameObjectVector.begin(), gameObjectVector.end(), update_fn);
}

sf::IntRect spriteIndex_to_rect(int index, int ncols, int square_size){
  int i = index / ncols;
  int j = index % ncols;
  return sf::IntRect(j*square_size, i*square_size, square_size, square_size);
}

void set_paddle_movement(Paddle& paddle, sf::Event& event){
  if (event.type != sf::Event::KeyPressed){
    return;
  }
  switch (event.key.code) {
  case sf::Keyboard::Left:
    paddle.direction_vector = sf::Vector2f(-1, 0);
    break;
  case sf::Keyboard::Right:
    paddle.direction_vector = sf::Vector2f(1, 0);
    break;
  case sf::Keyboard::Down:
    paddle.direction_vector = sf::Vector2f(0, 0);
    break;

  default:
    break;
  }
}

std::vector<Brick> create_bricks(sf::Vector2f grid_dim, int nrows, int ncols, int npadding, float brick_height){
  auto vector = std::vector<Brick>();
  float brick_width = grid_dim.x / ncols;
  float h = npadding * brick_height;
  sf::Vector2f left_corner;
  Brick brick;

  for (int i =0; i < nrows; i++){
    for (int j=0; j < ncols; j++){
      left_corner = sf::Vector2f(j * brick_width, h);
      vector.push_back(Brick(left_corner,  brick_height, brick_width));
    }
    h += brick_height;
  }
  return vector;
}


// global variables declaration
const int SQUARE_SIZE = 32;
const int NCOLS = 2;

//static attributes must be created/affected outside of a function
sf::IntRect Ball::textureRect = spriteIndex_to_rect(0, NCOLS, SQUARE_SIZE);
sf::IntRect Paddle::textureRect = spriteIndex_to_rect(2, NCOLS, SQUARE_SIZE);
sf::IntRect Brick::textureRect = spriteIndex_to_rect(3, NCOLS, SQUARE_SIZE);

int main(){
  sf::Vector2f grid_dim = sf::Vector2f(500, 500);
  float ball_speed = 2;
  float radius = 10;
  float time_step = 0.01;
  int vertex_number;
  bool gameOver = false;
  sf::VertexArray quadArray;
  sf::Vector2f direction_vector = sf::Vector2f(0.25, 1);
  sf::Vector2f ball_position = grid_dim * (float)(0.5);

  
  Ball ball = Ball(ball_position, radius, ball_speed, direction_vector);
  std::string texture_file {"../img/Breakout.png"};
  
  Wall left = Wall(sf::Vector2f(0, 0), sf::Vector2f(0, grid_dim.y));
  Wall right = Wall(sf::Vector2f(grid_dim.x, 0), sf::Vector2f(grid_dim.x, grid_dim.y));
  Wall top = Wall(sf::Vector2f(0, 0), sf::Vector2f(grid_dim.x, 0));

  Pit bottom = Pit(sf::Vector2f(0, grid_dim.y + 2 * radius), sf::Vector2f(grid_dim.x, grid_dim.y + 2 * radius));



  
  float paddle_height = 20;
  float paddle_width = 80;
  sf::Vector2f paddle_direction = sf::Vector2f(0, 0);
  Paddle paddle = Paddle(sf::Vector2f(grid_dim.x / 2, grid_dim.y - paddle_height), paddle_height, paddle_width, paddle_direction, 3);

  //create row of bricks  
  std::vector<Brick> bricks = create_bricks(grid_dim, 3, 7, 1, 40);
  
  
  sf::RenderWindow window(sf::VideoMode(grid_dim.y, grid_dim.x), "Breakout");
  sf::Clock clock;
  std::vector<GameObject*> gameObjectVector = {&ball, &left, &right, &bottom,& top, &paddle};
  //insert the bricks into gameObjectvector
  auto insert_bricks = [&gameObjectVector](Brick &brick){
  			 gameObjectVector.push_back(&brick);
  		       };

  std::for_each(bricks.begin(), bricks.end(), insert_bricks);

  sf::Texture texture;
  if (!texture.loadFromFile(texture_file)){
    std::cout << "An error have occured while loading the texture file : " <<  texture_file << std::endl;
    return -1;
  }


  auto accumulate_fn = [](int acc, GameObject* pnt) -> int {
			 if (pnt != NULL){
			   return acc + pnt->shape->get_vertex_size(); }
			 return acc;
		       };

  auto set_vertex_fn = [&quadArray](int acc, GameObject* pnt) -> int {
			 if (pnt != NULL){
			   pnt->shape->set_vertex_array(quadArray, acc, pnt->get_textureRect());
			   return acc + pnt->shape->get_vertex_size();
			 }
			 return acc;
		       };
  

  //game loop
  while (window.isOpen()){
    sf::Event event;
    if (gameOver){
      window.close();
      std::cout << "game over !" << std::endl;
      return 0;
    }
    
    while (window.pollEvent(event)){
      if (event.type == sf::Event::Closed){
	window.close();
      }

      set_paddle_movement(paddle, event);
      
    }

    if (clock.getElapsedTime().asSeconds() > time_step){
      clock.restart();
      vertex_number = std::accumulate(gameObjectVector.begin(), gameObjectVector.end(), 0, accumulate_fn);  
      quadArray = sf::VertexArray(sf::Quads, vertex_number);
      
      gameLoop(ball, gameObjectVector);
     (ball.shape)->move(ball.next_position());

     left.handle_collision(paddle);
     right.handle_collision(paddle);
     (paddle.shape)->move(paddle.next_position());
     std::accumulate(gameObjectVector.begin(), gameObjectVector.end(), 0, set_vertex_fn);

     gameOver = bottom.isGameOver;
 
      window.clear(sf::Color::White);
      window.draw(quadArray, &texture);
      window.display();
    }

    
  }
  return 0;
}
