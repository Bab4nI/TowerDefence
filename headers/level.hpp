#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#define step 0.01
#define model_len 10

namespace Level {
    extern std::vector<sf::Vector2f> level1_points_Bezier;

    class Level {
    public:
        Level(std::vector<sf::Vector2f> points_Bezier, unsigned num);
        sf::Vector2f get_cords(int cur_step);
        int get_route_length();
    private:
        std::vector<sf::Vector2f> route;
        unsigned number;

        std::vector<sf::Vector2f> computeBezierCurve(const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&);
        std::vector<sf::Vector2f> route_calculation(std::vector<sf::Vector2f>);
    };

}

