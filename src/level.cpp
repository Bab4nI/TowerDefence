#include "level.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

/*
    All scores are recorded using a hardcode in the "points" vector, 
    and their number is guaranteed to be 3 * n + 1, where "n" is the number of segments.
*/
namespace Level {

    Castle::Castle(int hp){
        health = hp;
    }
    
    bool Castle::deal_damage(int damage){
        health -= damage;
        return health < 0;
    }

    int Castle::get_health(){
        return health;
    }
    
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
    Level::Level(std::vector<sf::Vector2f> points_Bezier, unsigned num, int castle_hp) :
        route(route_calculation(points_Bezier)),
        number(num),
        castle(castle_hp)
    {}
    
    bool Level::damage_the_castle(int damage){
        return castle.deal_damage(damage);
    }

    std::pair<sf::Vector2f, bool> Level::get_cords(int cur_step) {
    if (cur_step < static_cast<int>(route.size())) {
        return std::make_pair(route[cur_step], false);
    } else {
        return std::make_pair(route.back(), true);
    }
}


    int Level::get_route_length(){
        return route.size();
    }
    
    std::vector<sf::Vector2f> Level::computeBezierCurve(
        const sf::Vector2f& p0,
        const sf::Vector2f& p1,
        const sf::Vector2f& p2,
        const sf::Vector2f& p3
        )
    {
        std::vector<sf::Vector2f> curve;

        sf::Vector2f prev_point = p0;
        curve.push_back(prev_point);

        float cur_len = 0;
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

            if (cur_len >= model_len){
                curve.push_back(point);
                cur_len = 0;
                prev_point = point;
            }

            
        }

        if (curve.back() != p3)
            curve.push_back(p3);

        return curve;
    }

    std::vector<sf::Vector2f> Level::route_calculation(std::vector<sf::Vector2f> points){
        std::vector<sf::Vector2f> route;
        int length = points.size();

        for(int i = 0; i < length - 1; i+=3){
            sf::Vector2f p1 = points[i];
            sf::Vector2f p2 = points[i + 1];
            sf::Vector2f p3 = points[i + 2];
            sf::Vector2f p4 = points[i + 3];

            std::vector<sf::Vector2f> seg = computeBezierCurve(p1, p2, p3, p4);
            route.insert(route.end(), seg.begin(), seg.end());
        }

        return route;
    }
}; // namespace Level



