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

    while (window.isOpen()) {
        // ---- SFML 3 event loop (type-safe) ----------------------------
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
    return 0;
}
