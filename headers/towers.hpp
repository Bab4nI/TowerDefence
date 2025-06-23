#pragma once

#include "level.hpp"
#include "enemies.hpp"
#include <SFML/Graphics.hpp>

namespace Towers{
    class Tower{
    public:
        Tower(sf::Vector2f crds);
        
        void tick(std::vector<Enemies::Enemy*>& enemies);

        sf::Sprite get_sprite();

        virtual ~Tower();
    protected:
        float range;
        int damage;
        float damage_interval;
        sf::Clock damage_clock;
        sf::Vector2f cords;
        sf::Texture texture;
        sf::Sprite sprite;
    };

    class Cannon : public Tower{
    public:
        Cannon(sf::Vector2f crds);
    };

    class Archer : public Tower{
    public:
        Archer(sf::Vector2f crds);
    };

    class Wizzard : public Tower{
    public:
        Wizzard(sf::Vector2f crds);
    };
} // namespace Towers