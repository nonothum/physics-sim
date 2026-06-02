#include "PhysicsWorld.h"
#include <SFML/Graphics.hpp>
#include <iterator>
#include <optional>

static constexpr unsigned  WIN_W = 900;
static constexpr unsigned  WIN_H = 600;
static constexpr float     FIXED_DT = 1.f / 60.f;   // 60 Hz physics
static constexpr int       MAX_STEPS = 5;           // spiral-of-death guard
static constexpr int       SPAWN_VEL = 350.f;       // initial horizontal kick

// -----------------------------------------------------------------------
//  Helpers
// -----------------------------------------------------------------------
static sf::Color randomColor() {
    // Bright varied RGB palette
    static const sf::Color palette[] = {
        {255, 99,  99},   // red
        {255,165,  50},   // orange
        {250,230,  60},   // yellow
        { 80,220, 120},   // green
        { 60,180,255},    // sky blue
        {180, 90,255},    // violet
        {255,110,200},    // pink
        { 80,240,230},    // teal
    };
    return palette[std::rand() % std::size(palette)];
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WIN_W, WIN_H)),
        "2D Physics Simulator"
    );
    window.setFramerateLimit(120);

    sf::RectangleShape bg(sf::Vector2f(WIN_W, WIN_H));
    bg.setFillColor({0, 0, 0});

    // ---- Physics world ------------------------------------------------
    PhysicsWorld world(WIN_W, WIN_H);

    auto spawnBall = [&world](float x, float y) {
        float rad = 10.f + std::rand() % 20;
        float mass = rad * 0.3f;

        float kickVariance = 0.4f + (std::rand() % 60 / 100.f);
        float vx = (std::rand() % 2 ? 1 : -1) * SPAWN_VEL * kickVariance;
        float vy = -200.f - std::rand() % 200;

        auto* e = world.addEntity({x, y}, rad, mass, randomColor());
        e->setVelocity({vx, vy});
    };

    // Seed a few balls
    for (int i = 0; i < 8; i++)
        spawnBall(std::rand() % WIN_W, std::rand() % WIN_H);

    // ---- Fixed-timestep bookkeeping -----------------------------------
    sf::Clock clock;
    float accumulator = 0.f;

    while (window.isOpen()) {
        // ---- SFML 3 event loop (type-safe) ----------------------------
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>())
                spawnBall(static_cast<float>(mb->position.x),
                          static_cast<float>(mb->position.y));
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
