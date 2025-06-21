#include "level.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

/*
    All scores are recorded using a hardcode in the "points" vector, 
    and their number is guaranteed to be 3 * n + 1, where "n" is the number of segments.
*/
namespace Level {

    std::vector<sf::Vector2f> level1_points_Bezier = {
        sf::Vector2f(1685, 1145),
        sf::Vector2f(1360, 1095),
        sf::Vector2f(1150, 1190),
        sf::Vector2f(930, 1050),
        sf::Vector2f(720, 900),
        sf::Vector2f(890, 720),
        sf::Vector2f(1030, 730)
    };

    Level::Level(std::vector<sf::Vector2f> points_Bezier, unsigned num){
        route = route_calculation(points_Bezier),
        number = num;
    }
        

    sf::Vector2f Level::get_cords(int cur_step){
        return route[cur_step];
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

            curve.push_back(point);
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



