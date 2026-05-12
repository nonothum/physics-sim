#pragma once

#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include <vector>
#include <memory>

// -----------------------------------------------------------------------
// PhysicsWorld
//   Owns all entities
// -----------------------------------------------------------------------
class PhysicsWorld {
public:

    void addEntity(Vec2 pos, float radius, sf::Color color = sf::Color::White) {
        entities.push_back(std::make_unique<Entity>(pos, radius, color));
    }

    void drawAll(sf::RenderWindow& window) const {
        for (const auto& e : entities) {
            assert(e && "PhysicsWorld contains a null Entity");
            e->draw(window);
        }
    }

private:
    std::vector<std::unique_ptr<Entity>> entities;
};
