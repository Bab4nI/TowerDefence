#include <SFML/Graphics.hpp>
#include "level.hpp"
#include "enemies.hpp"
#include "towers.hpp"

#define castle_hp 10
#define road_width 30

int main()
{
    Level::Level level1(Level::level1_points_Bezier, Level::level1_points_Bezier, 1, castle_hp, road_width);

    // Convert the route to a vertex array so that it can be drawn using SFML.
    sf::VertexArray route_line(sf::LineStrip, level1.get_route_length());
    
    for (std::size_t i = 0; i < level1.get_route_length(); ++i)
    {
        route_line[i].position = level1.route[i];
        route_line[i].color = sf::Color::Green;
    }

    sf::VertexArray left_road_line(sf::LineStrip, level1.get_road_length());
    sf::VertexArray right_road_line(sf::LineStrip, level1.get_road_length());
    for (std::size_t i = 0; i < level1.get_road_length(); ++i)
    {

        left_road_line[i].position = level1.road[i].first;
        right_road_line[i].position = level1.road[i].second;
        left_road_line[i].color = sf::Color::Black;
        right_road_line[i].color = sf::Color::Black;
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
 
    int cur_castle_hp = level1.castle.get_health();
    std::string castle_hp_string = std::to_string(cur_castle_hp) + " HP";
    castle_hp_text.setString(castle_hp_string);
    sf::FloatRect textRect = castle_hp_text.getLocalBounds();
        castle_hp_text.setOrigin(textRect.left + textRect.width,
                                textRect.top  + textRect.height);
    castle_hp_text.setPosition(window.getSize().x - 15.0f, window.getSize().y - 15.0f);

    sf::Texture defeat_screen;
    if(!defeat_screen.loadFromFile("assets\\images\\defeat_screen.png"))
        return EXIT_FAILURE;
    
    sf::Sprite defeat_sprite = sf::Sprite(defeat_screen);
    defeat_sprite.setColor(sf::Color(50, 0, 0, 128)); // 128 -> ~50% opacity
    
    // 660 630
    // 670 370
    std::vector<Towers::Tower*> towers;
    // --------- hardcode for test ------------
    sf::Vector2f test_cords = sf::Vector2f(660, 630);
    towers.push_back(new Towers::Cannon(test_cords));
    test_cords = sf::Vector2f(670, 370);
    towers.push_back(new Towers::Cannon(test_cords));
    // ----------------------------------------


    sf::RectangleShape shop_panel;
    shop_panel.setFillColor(sf::Color(0, 0, 0, 150));
    shop_panel.setSize(sf::Vector2f(window.getSize().x, 120.f));

    sf::Texture cannon_icon_texture;
    if(!cannon_icon_texture.loadFromFile("assets\\images\\cannon_100.png"))
        return EXIT_FAILURE;
    sf::Sprite cannon_icon(cannon_icon_texture);
    cannon_icon.setPosition(10.f, 10.f);

    bool build_mode = false;
    sf::Sprite build_sprite;

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
            else if (event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left){
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if(build_mode){
                        towers.push_back(new Towers::Cannon(mousePos));
                        build_mode = false;
                    }else if(cannon_icon.getGlobalBounds().contains(mousePos)){
                        build_mode = true;
                        build_sprite.setTexture(cannon_icon_texture);
                        build_sprite.setOrigin(cannon_icon_texture.getSize().x / 2.f, cannon_icon_texture.getSize().y / 2.f);
                    }
                }
                else if(event.mouseButton.button == sf::Mouse::Right){
                    build_mode = false;
                }
            }
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
                build_mode = false;
            }
        }
        if(build_mode){
            build_sprite.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        }

        if(!castle_is_destroyed){
            if (spawn_clock.getElapsedTime().asSeconds() >= spawn_interval){
                enemies.push_back(new Enemies::Goblin());
                spawn_clock.restart();
            }
            if(move_clock.getElapsedTime().asSeconds() >= move_interval){
                for(Enemies::Enemy* enemy : enemies){
                    castle_is_destroyed = enemy->tick(level1);
                }
                move_clock.restart();

                for(Towers::Tower* tower : towers){
                    tower->tick(enemies);
                }
            }
        }

        cur_castle_hp = level1.castle.get_health();
        castle_hp_string = std::to_string(cur_castle_hp) + " HP";
        castle_hp_text.setString(castle_hp_string);
        sf::FloatRect textRect = castle_hp_text.getLocalBounds();
        castle_hp_text.setOrigin(textRect.left + textRect.width,
                                textRect.top  + textRect.height);

        if(castle_is_destroyed){
            window.draw(defeat_sprite);
        }
        else{
            window.clear();

            window.draw(BckgSprite);

            //----------------------------
            window.draw(route_line);
            window.draw(left_road_line);
            window.draw(right_road_line);
            //-------------------------

            for(Enemies::Enemy* enemy : enemies){
                window.draw(enemy->get_sprite());
                window.draw(enemy->get_health_bar());
            }

            for(Towers::Tower* tower : towers){
                window.draw(tower->get_sprite());
            }

            if(build_mode){
                window.draw(build_sprite);
            }

            window.draw(shop_panel);
            window.draw(cannon_icon);

            window.draw(castle_hp_text);
        }

        window.display(); 
    }

    return EXIT_SUCCESS;
}