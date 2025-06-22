#pragma once

#include "level.hpp"
#include <SFML/Graphics.hpp>

namespace Enemies{

    class Enemy{
    public:
        Enemy();

        bool tick(Level::Level& level);

        sf::Texture get_texture();
        
        int get_speed();

        int get_health();

        int get_step();

        int get_damage();

        sf::Vector2f get_cords();

        sf::Sprite get_sprite();

        virtual bool health_decrease(int hp) = 0;

        virtual ~Enemy();

    protected:
        float speed;
        int health;
        int cur_step;
        int damage;
        float damage_interval;
        sf::Clock damage_clock;
        sf::Vector2f cords;
        sf::Texture texture;
        sf::Sprite sprite;

        void set_cords(sf::Vector2f crds); 
        void move();
    };

    class Goblin: public Enemy{
    public:
        Goblin();
        bool health_decrease(int hp) override;
    };
} // namespace Enemies

