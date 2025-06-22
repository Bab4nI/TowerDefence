#pragma once

#include <SFML/Graphics.hpp>

namespace Enemies{

    class Enemy{
    public:
        Enemy();
        sf::Texture get_texture();
        
        int get_speed();

        int get_health();

        int get_step();

        sf::Sprite get_sprite();

        void move();

        void set_cords(sf::Vector2f crds);  

        virtual bool health_decrease(int hp) = 0;

        virtual ~Enemy();

    protected:
        float speed;
        int health;
        int cur_step;
        int damage;
        sf::Vector2f cords;
        sf::Texture texture;
        sf::Sprite sprite;
    };

    class Goblin: public Enemy{
    public:
        Goblin();
        bool health_decrease(int hp) override;
    };
} // namespace Enemies

