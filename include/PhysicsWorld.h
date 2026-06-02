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

        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j)
                resolveCircleCollision(*entities[i], *entities[j]);
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

    size_t getNumEntities() const {
        return entities.size();
    }

    void clear() {
        entities.clear();
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

    void resolveCircleCollision(Entity& a, Entity& b) {
        // Pythagorean theorem for distance
        Vec2  delta = b.getPosition() - a.getPosition();
        float distSq = delta.lengthSq();
        float minDist = a.getRadius() + b.getRadius();

        if (distSq >= minDist * minDist || distSq < 1e-6f)
            return;

        float dist = delta.length();
        Vec2  normal = delta / dist;          // unit normal a→b

        // Relative velocity along normal
        Vec2 relVel = b.getVelocity() - a.getVelocity();
        float velAlongNormal = relVel.dot(normal);

        // Already separating - skip
        if (velAlongNormal > 0.f)
            return;

        float invMassA = 1.f / a.getMass();
        float invMassB = 1.f / b.getMass();
        float invMassSum = invMassA + invMassB;
        if (invMassSum < 1e-6f)
            return;

        float e = std::min(a.getRestitution(), b.getRestitution());

        // Impulse scalar
        float j = -(1.f + e) * velAlongNormal / invMassSum;

        Vec2 impulse = normal * j;
        a.setVelocity(a.getVelocity() - impulse * invMassA);
        b.setVelocity(b.getVelocity() + impulse * invMassB);

        // Positional correction (Baumgarte, 30% slop)
        const float percent = 0.3f;
        const float slop    = 0.5f;
        float penetration   = minDist - dist;
        if (penetration > slop) {
            Vec2 correction = normal * ((penetration - slop) / invMassSum * percent);
            a.setPosition(a.getPosition() - correction * invMassA);
            b.setPosition(b.getPosition() + correction * invMassB);
        }
    }
};
