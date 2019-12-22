#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <iostream>

float determinant(const sf::Vector2f &v1, const sf::Vector2f & v2);
float dot_product(const sf::Vector2f &v1, const sf::Vector2f &v2);
sf::Vector2f dist_vect(const sf::Vector2f &point1, const sf::Vector2f &point2);
float norme(const sf::Vector2f &v);

sf::Vector2f rotate(const sf::Vector2f& vector, float angle);
sf::Vector2f xdeviate(const sf::Vector2f& vector);
sf::Vector2f ydeviate(const sf::Vector2f& vector);
std::vector<sf::Vector2f> get_rect_points(int left, int top, float height, float width);
std::ostream &operator<<(std::ostream &flux, sf::Vector2f const &vector);

std::vector<sf::Vector2f> rangex(float start, float end, float step, float y_value);
