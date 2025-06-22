#include <SFML/Graphics.hpp>
#include "level.hpp"
#include "enemies.hpp"

#define castle_hp 10

int main()
{
    //-----------------------------------------------------------------------------
    // Example route
    //-----------------------------------------------------------------------------

    Level::Level level1(Level::level1_points_Bezier, 1, castle_hp);

    // Convert the route to a vertex array so that it can be drawn using SFML.
    sf::VertexArray routeLine(sf::LineStrip, level1.get_route_length());
    for (std::size_t i = 0; i < level1.get_route_length(); ++i)
    {
        routeLine[i].position = level1.get_cords(i).first;
        routeLine[i].color = sf::Color::Black;
    }

    sf::RenderWindow window(sf::VideoMode({1024, 1024}), "Tower Defense"); 

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

    sf::Clock spawn_clock, move_clock, damage_clock;

    float 
        spawn_interval  =   10.0f, 
        move_interval   =   0.01f, 
        damage_interval =   1.0f;

    std::vector<Enemies::Enemy*> enemies;

    bool castle_is_destroyed = false;

    sf::Font castle_hp_font;
    if(!castle_hp_font.loadFromFile("assets\\fonts\\AGENCYB.TTF"))
        return EXIT_FAILURE;
    sf::Text castle_hp_text;
    castle_hp_text.setFont(castle_hp_font);
    castle_hp_text.setCharacterSize(20);
    castle_hp_text.setFillColor(sf::Color::Red);

    sf::FloatRect textRect = castle_hp_text.getLocalBounds();
    castle_hp_text.setOrigin(textRect.width, textRect.height); 
    castle_hp_text.setPosition(window.getSize().x - 10.f, window.getSize().y - 10.f);
    int cur_castle_hp = level1.castle.get_health();
    std::string castle_hp_string = std::to_string(cur_castle_hp) + " HP";

    castle_hp_text.setString(castle_hp_string);

    sf::Texture defeat_screen;
    if(!defeat_screen.loadFromFile("assets\\images\\defeat_screen.png"))
        return EXIT_FAILURE;
    
    sf::Sprite defeat_sprite = sf::Sprite(defeat_screen);

    while (window.isOpen())
    {
        if(!castle_is_destroyed){
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                else if (event.type == sf::Event::Resized){
                    window_size = window.getSize();
                    window_size.x = window_size.y * coef;
                    window.setSize(window_size);
                    castle_hp_text.setPosition(window.getSize().x - 10.f, window.getSize().y - 10.f);
                }
            }

            if (spawn_clock.getElapsedTime().asSeconds() >= spawn_interval){
                enemies.push_back(new Enemies::Goblin());
                spawn_clock.restart();
            }
            if(move_clock.getElapsedTime().asSeconds() >= move_interval){
                for(Enemies::Enemy* enemy : enemies){
                    castle_is_destroyed = enemy->tick(level1);
                }
                move_clock.restart();
            }
        }

        cur_castle_hp = level1.castle.get_health();
        castle_hp_string = std::to_string(cur_castle_hp) + " HP";
        castle_hp_text.setString(castle_hp_string);

        if(castle_is_destroyed){
            window.draw(defeat_sprite);
        }
        else{
            window.clear();

            window.draw(BckgSprite);

            //----------------------------
            window.draw(routeLine);
            //-------------------------

            for(Enemies::Enemy* enemy : enemies){
                window.draw(enemy->get_sprite());
            }

            window.draw(castle_hp_text);                    
        }  

        window.display(); 
    }

    return EXIT_SUCCESS;
}