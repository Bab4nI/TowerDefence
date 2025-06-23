#include "level.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

/*
    All scores are recorded using a hardcode in the "points" vector, 
    and their number is guaranteed to be 3 * n + 1, where "n" is the number of segments.
*/
namespace Level {

    Castle::Castle(sf::Vector2f _cords, int hp){
        if(!texture.loadFromFile("assets\\images\\castle_200.png"))
            throw std::runtime_error("Failed to load castle texture.");
        else{
            sprite = sf::Sprite(texture);
            sprite.setOrigin(sf::Vector2f(100, 100));
            health = hp;
            cords = _cords;
            sprite.setPosition(_cords);
        }
    }
    
    bool Castle::deal_damage(int damage){
        health -= damage;
        return health <= 0;
    }

    int Castle::get_health(){
        return health;
    }
    
    sf::Sprite Castle::get_sprite(){
        return sprite;
    }

    sf::Vector2f level1_castle_cords = sf::Vector2f(860, 910);

    std::vector<sf::Vector2f> level1_points_Bezier = {
        sf::Vector2f(1080, 630),
        sf::Vector2f(430, 870),
        sf::Vector2f(490, 510),
        sf::Vector2f(670, 460),

        sf::Vector2f(870, 400),
        sf::Vector2f(800, 165),
        sf::Vector2f(480, 165),

        sf::Vector2f(240, 170),
        sf::Vector2f(130, 330),
        sf::Vector2f(295, 405),

        sf::Vector2f(450, 470),
        sf::Vector2f(375, 580),
        sf::Vector2f(260, 650),

        sf::Vector2f(140, 710),
        sf::Vector2f(130, 980),
        sf::Vector2f(780, 940)
    };

    /*
        Here the initialization list is specifically used, 
            because of the field needs to be initialized as a class object.
    */
    Level::Level(std::vector<sf::Vector2f> points_Bezier, std::vector<sf::Vector2f> _points, unsigned num, int castle_hp, int _road_width, sf::Vector2f castle_cords) :      
        road_width(_road_width),
        number(num),
        castle(castle_cords, castle_hp),
        points(_points)
    {
        build_ban_map.resize(1024, std::vector<bool>(1024, true));
        compute_bezier_curve();
    }
    
    bool Level::is_build_allowed(sf::Vector2f pos) const{
        int x = static_cast<int>(std::round(pos.x));
        int y = static_cast<int>(std::round(pos.y));
        if (x < 0 || x >= static_cast<int>(build_ban_map[0].size()) ||
            y < 0 || y >= static_cast<int>(build_ban_map.size()))
            return false;
        return build_ban_map[y][x];
    }

    bool Level::damage_the_castle(int damage){
        return castle.deal_damage(damage);
    }

    std::pair<sf::Vector2f, bool> Level::get_cords(int cur_step) {
        if (cur_step <static_cast<int>(route.size())) {
            return std::make_pair(route[cur_step], false);
        } 
        else {
            return std::make_pair(route.back(), true);
        }
    }


    int Level::get_route_length(){
        return route.size();
    }
    
    int Level::get_road_length(){
        return road.size();
    }
    
    void Level::compute_bezier_curve()
    {
        int length = points.size();

        for (int i = 0; i < length - 1; i += 3) {
            sf::Vector2f p0 = points[i];
            sf::Vector2f p1 = points[i + 1];
            sf::Vector2f p2 = points[i + 2];
            sf::Vector2f p3 = points[i + 3];

            std::vector<sf::Vector2f> curve;
            std::vector<std::pair<sf::Vector2f, sf::Vector2f>> road_part;

            sf::Vector2f prev_point = p0;
            curve.push_back(prev_point);

            float cur_len = 0;

            sf::Vector2f prev_left, prev_right;
            bool has_prev_normals = false;

            for (float t = 0.f; t <= 1.f; t += step) {
                float u = 1.f - t;
                float uu = u * u;
                float uuu = uu * u;

                float tt = t * t;
                float ttt = tt * t;

                sf::Vector2f point =
                    uuu * p0 +
                    3.f * uu * t * p1 +
                    3.f * u * tt * p2 +
                    ttt * p3;

                float dx = point.x - prev_point.x;
                float dy = point.y - prev_point.y;
                cur_len += std::sqrt(dx * dx + dy * dy);

                if (cur_len >= model_len) {
                    
                    curve.push_back(point);
                    cur_len = 0;
                    prev_point = point;

                    float u = 1.f - t;
                    sf::Vector2f tangent =
                        3.f * u * u * (p1 - p0) +
                        6.f * u * t * (p2 - p1) +
                        3.f * t * t * (p3 - p2);

                    float len = std::sqrt(tangent.x * tangent.x + tangent.y * tangent.y);
                    if (len != 0.f) tangent /= len;

                    sf::Vector2f left_normal = point + sf::Vector2f(-tangent.y * road_width, tangent.x * road_width);
                    sf::Vector2f right_normal = point + sf::Vector2f(tangent.y * road_width, -tangent.x * road_width);

                    road_part.emplace_back(left_normal, right_normal);

                    if (has_prev_normals) {
                        int samples = 20; 
                        for (int j = 0; j <= samples; ++j) {
                            float s = static_cast<float>(j) / samples;

                            sf::Vector2f left_interp = prev_left + s * (left_normal - prev_left);
                            sf::Vector2f right_interp = prev_right + s * (right_normal - prev_right);

                            int inner_steps = static_cast<int>(std::hypot(right_interp.x - left_interp.x, right_interp.y - left_interp.y));
                            for (int k = 0; k <= inner_steps; ++k) {
                                float t = static_cast<float>(k) / inner_steps;
                                sf::Vector2f p = left_interp + t * (right_interp - left_interp);

                                int x = static_cast<int>(std::round(p.x));
                                int y = static_cast<int>(std::round(p.y));
                                if (x >= 0 && x < 1024 && y >= 0 && y < 1024) {
                                    build_ban_map[y][x] = false;
                                }
                            }
                        }
                    }

                    prev_left = left_normal;
                    prev_right = right_normal;
                    has_prev_normals = true;
                }
            }

            if (curve.back() != p3)
                curve.push_back(p3);

            route.insert(route.end(), curve.begin(), curve.end());
            road.insert(road.end(), road_part.begin(), road_part.end());
        }
    }

}; // namespace Level



