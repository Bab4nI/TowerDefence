#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#define step 0.001f
#define model_len 10

namespace Level {

    class Castle{
    public:
        Castle(int hp);

        int get_health();
        bool deal_damage(int damage);

    private:
        int health;
    };

    extern std::vector<sf::Vector2f> level1_points_Bezier;

    class Level {
    public:
        Level(std::vector<sf::Vector2f> points_Bezier, unsigned num, int castle_hp);

        // The second value indicates whether an enemy 
        //  has reached the end of the route
        std::pair<sf::Vector2f, bool> get_cords(int cur_step);
        int get_route_length();
        bool damage_the_castle(int damage);

        Castle castle;

    private:
        std::vector<sf::Vector2f> route;
        unsigned number;

        std::vector<sf::Vector2f> computeBezierCurve(const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&);
        std::vector<sf::Vector2f> route_calculation(std::vector<sf::Vector2f>);
    };
}

