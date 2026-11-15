#include <iostream>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

// Box2D -> SFML
constexpr float SCALE = 40.0f;

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Workshop");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Render
        window.clear(sf::Color::White);
        window.display();
    }

    return 0;
}
