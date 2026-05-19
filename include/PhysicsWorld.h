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

    PhysicsWorld(float w, float h)
    : worldW(w)
    , worldH(h)
    {}

    void addEntity(Vec2 pos, float radius, float mass, sf::Color color = sf::Color::White) {
        entities.push_back(std::make_unique<Entity>(pos, radius, mass, color));
    }

    // Advance simulation by one fixed timestep dt (seconds)
    void step(float dt) {
        for (auto& e: entities) {
            e->applyForce(gravity * e->getMass());
            e->integrate(dt);
        }
    }

    void drawAll(sf::RenderWindow& window) const {
        for (const auto& e : entities) {
            assert(e && "PhysicsWorld contains a null Entity");
            e->draw(window);
        }
    }

private:
    std::vector<std::unique_ptr<Entity>> entities;

    Vec2 gravity{0.f, 600.f};   // pixels/s² downward

    // World boundaries
    float worldW = 0.f;
    float worldH = 0.f;
};
