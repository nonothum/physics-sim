#include "PhysicsWorld.h"
#include <SFML/Graphics.hpp>
#include <optional>

static constexpr unsigned  WIN_W = 900;
static constexpr unsigned  WIN_H = 600;
static constexpr float     FIXED_DT = 1.f / 60.f;   // 60 Hz physics
static constexpr int       MAX_STEPS = 5;           // spiral-of-death guard

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WIN_W, WIN_H)),
        "2D Physics Simulator"
    );
    window.setFramerateLimit(120);

    sf::RectangleShape bg(sf::Vector2f(WIN_W, WIN_H));
    bg.setFillColor({128, 128, 128});

    // ---- Physics world ------------------------------------------------
    PhysicsWorld world(WIN_W, WIN_H);

    world.addEntity({100, 50}, 50, 42);

    // ---- Fixed-timestep bookkeeping -----------------------------------
    sf::Clock clock;
    float accumulator = 0.f;

    while (window.isOpen()) {
        // ---- SFML 3 event loop (type-safe) ----------------------------
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // ---- Physics update (fixed timestep) --------------------------
        float frameTime = clock.restart().asSeconds();
        if (frameTime > 0.25f)
            frameTime = 0.25f;   // clamp huge deltas
        accumulator += frameTime;

        int steps = 0;
        while (accumulator >= FIXED_DT && steps < MAX_STEPS) {
            world.step(FIXED_DT);
            accumulator -= FIXED_DT;
            ++steps;
        }

        // ---- Render ---------------------------------------------------
        window.clear();
        window.draw(bg);

        world.drawAll(window);

        window.display();
    }
    return 0;
}
