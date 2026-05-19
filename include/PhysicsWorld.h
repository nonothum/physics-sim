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

    Entity* addEntity(Vec2 pos, float radius, float mass, sf::Color color = sf::Color::White) {
        entities.push_back(std::make_unique<Entity>(pos, radius, mass, color));
        return entities.back().get();
    }

    // Advance simulation by one fixed timestep dt (seconds)
    void step(float dt) {
        for (auto& e: entities) {
            e->applyForce(gravity * e->getMass());
            e->integrate(dt);
        }

        for (auto& e : entities) {
            assert(e && "PhysicsWorld contains a null Entity");
            constrainToBounds(*e);
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

    // ----------------------------------------------------------------
    // Boundary constraint (AABB of circular body vs window edges)
    // ----------------------------------------------------------------
    void constrainToBounds(Entity& e) {
        const float rad = e.getRadius();
        const float res = e.getRestitution();
        const Vec2 pos = e.getPosition();
        const Vec2 vel = e.getVelocity();

        // Floor
        if (pos.y + rad > worldH) {
            e.setPosition(Vec2(pos.x, worldH - rad));
            e.setVelocity(Vec2(vel.x * 0.98f, -std::abs(vel.y) * res)); // mild horizontal friction
        }

        // Right wall
        if (pos.x + rad > worldW) {
            e.setPosition(Vec2(worldW - rad, pos.y));
            e.setVelocity(Vec2(-std::abs(vel.x) * res, vel.y));
        }

        // Left wall
        if (pos.x - rad < 0) {
            e.setPosition(Vec2(rad, pos.y));
            e.setVelocity(Vec2(std::abs(vel.x) * res, vel.y));
        }
    }
};
