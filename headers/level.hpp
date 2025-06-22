#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#define step 0.001f
#define model_len 3

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
        Level(std::vector<sf::Vector2f> points_Bezier, std::vector<sf::Vector2f> points, unsigned num, int castle_hp, int road_width);

        // The second value indicates whether an enemy 
        //  has reached the end of the route
        std::pair<sf::Vector2f, bool> get_cords(int cur_step);
        int get_route_length();
        int get_road_length();
        bool damage_the_castle(int damage);

        Castle castle;

        std::vector<sf::Vector2f> route;
        std::vector<std::pair<sf::Vector2f, sf::Vector2f>> road; 
        unsigned number;

    private:
        std::vector<std::vector<bool>> build_ban_map;
        int road_width;
        std::vector<sf::Vector2f> points;         
        void compute_bezier_curve();
       // void route_calculation(std::vector<sf::Vector2f>);
    };
}

