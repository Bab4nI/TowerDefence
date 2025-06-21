#include <SFML/Graphics.hpp>
#include "level.hpp"

int main()
{
    //-----------------------------------------------------------------------------
    // Example route
    //-----------------------------------------------------------------------------

    Level::Level level1(Level::level1_points_Bezier, 1);

    // Convert the route to a vertex array so that it can be drawn using SFML.
    sf::VertexArray routeLine(sf::LineStrip, level1.get_route_length());
    for (std::size_t i = 0; i < level1.get_route_length(); ++i)
    {
        routeLine[i].position = level1.get_cords(i);
        routeLine[i].color = sf::Color::Black;
    }

    sf::RenderWindow window(sf::VideoMode({1080, 1080}), "Tower Defense"); 

    sf::Texture BackgroundImage; 
    if(!BackgroundImage.loadFromFile("assets\\images\\white_square.png"))
        return EXIT_FAILURE;
    float coef = static_cast<float>(BackgroundImage.getSize().x);
    coef /= BackgroundImage.getSize().y;

    sf::Sprite BckgSprite(BackgroundImage);
    float coef_to_resize = 1080;
    coef_to_resize /= BackgroundImage.getSize().x;
    BckgSprite.setScale(coef_to_resize, coef_to_resize);

    sf::Vector2u cur_size, prev_size = window.getSize(), window_size;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized){
                window_size = window.getSize();
                window_size.x = window_size.y * coef;
                window.setSize(window_size);
            }
        }

        window.clear();
        window.draw(BckgSprite);
        //----------------------------
        window.draw(routeLine);
        //-------------------------
        window.display();
        
    }

    return EXIT_SUCCESS;
}