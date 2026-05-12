#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>

// -----------------------------------------------------------------------
// Entity
//   Represents a single circular rigid body in the simulation.
// -----------------------------------------------------------------------
class Entity {
public:
    Entity(Vec2 pos, float radius, sf::Color color = sf::Color::White)
        : position(pos)
        , radius(radius)
        , color(color)
    {}
    
    void draw(sf::RenderWindow& window) const {
        sf::CircleShape shape(radius);
        shape.setOrigin(sf::Vector2f(radius, radius));
        shape.setPosition(sf::Vector2f(position.x, position.y));
        shape.setFillColor(color);
        shape.setOutlineThickness(1.f);
        shape.setOutlineColor(sf::Color(255, 255, 255, 60));
        window.draw(shape);
    }

private:
    Vec2 position;
    float radius;
    sf::Color color;

};
