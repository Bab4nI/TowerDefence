#include <SFML/Graphics.hpp>
#include "level.hpp"
#include "enemies.hpp"

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
        routeLine[i].position = level1.get_cords(i).first;
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

    sf::Clock spawn_clock, move_clock;
    float spawn_interval = 10.0f, move_interval = 0.01f;

    std::vector<Enemies::Enemy*> enemies;

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

        if (spawn_clock.getElapsedTime().asSeconds() >= spawn_interval){
            enemies.push_back(new Enemies::Goblin());
            spawn_clock.restart();
        }
        if(move_clock.getElapsedTime().asSeconds() >= move_interval){
            for(Enemies::Enemy* enemy : enemies){
                enemy->move();
            }
            move_clock.restart();
        }

        window.clear();

        window.draw(BckgSprite);

        //----------------------------
        window.draw(routeLine);
        //-------------------------

        for(Enemies::Enemy* enemy : enemies){
            std::pair<sf::Vector2f, bool> result = level1.get_cords(enemy->get_step());
            
            // Has the enemy reached the end of the route
            if(result.second){
                // !--- damage logic ----!
            }
            enemy->set_cords(result.first);
            sf::Sprite sprite= enemy->get_sprite();
            window.draw(sprite);
        }

        window.display();
        
    }

    return EXIT_SUCCESS;
}