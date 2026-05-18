#include "PhysicsWorld.h"
#include <SFML/Graphics.hpp>
#include <optional>

static constexpr unsigned  WIN_W = 900;
static constexpr unsigned  WIN_H = 600;

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WIN_W, WIN_H)),
        "2D Physics Simulator"
    );
    window.setFramerateLimit(120);

    sf::RectangleShape bg(sf::Vector2f(WIN_W, WIN_H));
    bg.setFillColor({128, 128, 128});

    // ---- Physics world ------------------------------------------------
    PhysicsWorld world;

    world.addEntity({100, 50}, 50, 42);

    while (window.isOpen()) {
        // ---- SFML 3 event loop (type-safe) ----------------------------
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // ---- Render ---------------------------------------------------
        window.clear();
        window.draw(bg);

        world.drawAll(window);

        window.display();
    }
    return 0;
}
