#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#define step 0.0005f
#define model_len 0.0005f

namespace Level {

class Castle{
public:
    Castle(sf::Vector2f cords, int hp);

    int get_health();
    sf::Sprite get_sprite();
    sf::RectangleShape get_health_bar();
    void update_health_bar();
    bool deal_damage(int damage);

private:
    int health;
    int max_health;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RectangleShape health_bar;
    sf::Vector2f cords;
};
    
    extern sf::Vector2f level1_castle_cords;
    extern std::vector<sf::Vector2f> level1_points_Bezier;

    class Level {
    public:
        Level(std::vector<sf::Vector2f> points_Bezier, std::vector<sf::Vector2f> _points, unsigned num, int castle_hp, int _road_width, sf::Vector2f castle_cords);

        // The second value indicates whether an enemy 
        //  has reached the end of the route
        std::pair<sf::Vector2f, bool> get_cords(int cur_step);
        int get_route_length();
        int get_road_length();
        bool damage_the_castle(int damage);
        bool is_build_allowed(sf::Vector2f pos) const;
        
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

