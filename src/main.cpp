#include <SFML/Graphics.hpp>
#include "level.hpp"
#include "enemies.hpp"
#include "towers.hpp"
#include <cstdlib>
#include <ctime>

#define castle_hp 10
#define road_width 30

enum class GameState { Menu, Playing, Defeat };

int main()
{
    Level::Level level1(Level::level1_points_Bezier, Level::level1_points_Bezier, 1, castle_hp, road_width, Level::level1_castle_cords);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({1024, 1024}), "Tower Defense", sf::Style::Titlebar | sf::Style::Close);
    GameState state = GameState::Menu;

    sf::Texture backgroundTex;
    if (!backgroundTex.loadFromFile("assets/images/background.png")) return EXIT_FAILURE;
    sf::Sprite background(backgroundTex);
    background.setScale(1080.f / backgroundTex.getSize().x, 1080.f / backgroundTex.getSize().x);

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/AGENCYB.TTF")) return EXIT_FAILURE;

    sf::Text castleHpText("", font, 30);
    castleHpText.setFillColor(sf::Color::Red);

    sf::VertexArray route_line(sf::LineStrip, level1.get_route_length());
    for (std::size_t i = 0; i < level1.get_route_length(); ++i) {
        route_line[i].position = level1.route[i];
        route_line[i].color = sf::Color::Green;
    }

    sf::VertexArray left_road(sf::LineStrip, level1.get_road_length());
    sf::VertexArray right_road(sf::LineStrip, level1.get_road_length());
    for (std::size_t i = 0; i < level1.get_road_length(); ++i) {
        left_road[i].position = level1.road[i].first;
        right_road[i].position = level1.road[i].second;
        left_road[i].color = sf::Color::Black;
        right_road[i].color = sf::Color::Black;
    }

    sf::Texture defeatTex;
    if (!defeatTex.loadFromFile("assets/images/defeat_screen.png")) return EXIT_FAILURE;
    sf::Sprite defeatSprite(defeatTex);
    defeatSprite.setColor(sf::Color(50, 0, 0, 128));

    std::vector<Enemies::Enemy*> enemies;
    std::vector<Towers::Tower*> towers;

    sf::Clock spawnClock, moveClock;
    float spawnInterval = 10.f, moveInterval = 0.01f;

    sf::RectangleShape shopPanel(sf::Vector2f(window.getSize().x, 120.f));
    shopPanel.setFillColor(sf::Color(0, 0, 0, 150));

    sf::Texture cannonTex, archerTex, wizardTex;
    if (!cannonTex.loadFromFile("assets/images/cannon_100.png")) return EXIT_FAILURE;
    if (!archerTex.loadFromFile("assets/images/archer_100.png")) return EXIT_FAILURE;
    if (!wizardTex.loadFromFile("assets/images/wizzard_100.png")) return EXIT_FAILURE;

    sf::Sprite cannonIcon(cannonTex), archerIcon(archerTex), wizardIcon(wizardTex);
    cannonIcon.setPosition(10, 10);
    archerIcon.setPosition(120, 10);
    wizardIcon.setPosition(230, 10);

    sf::RectangleShape startBtn(sf::Vector2f(200, 50)), exitBtn(sf::Vector2f(200, 50));
    startBtn.setFillColor(sf::Color(100, 100, 100));
    exitBtn.setFillColor(sf::Color(100, 100, 100));
    startBtn.setPosition(window.getSize().x / 2.f - 100, window.getSize().y / 2.f - 60);
    exitBtn.setPosition(window.getSize().x / 2.f - 100, window.getSize().y / 2.f + 10);

    sf::Text startText("Start", font, 20);
    startText.setFillColor(sf::Color::White);
    startText.setOrigin(startText.getLocalBounds().width / 2, startText.getLocalBounds().height / 2);
    startText.setPosition(startBtn.getPosition().x + 100, startBtn.getPosition().y + 25);

    sf::Text exitText("Exit", font, 20);
    exitText.setFillColor(sf::Color::White);
    exitText.setOrigin(exitText.getLocalBounds().width / 2, exitText.getLocalBounds().height / 2);
    exitText.setPosition(exitBtn.getPosition().x + 100, exitBtn.getPosition().y + 25);

    sf::RectangleShape defeatMenuBtn(sf::Vector2f(200, 50)), defeatExitBtn(sf::Vector2f(200, 50));
    defeatMenuBtn.setFillColor(sf::Color(100, 100, 100));
    defeatExitBtn.setFillColor(sf::Color(100, 100, 100));
    defeatMenuBtn.setPosition(window.getSize().x / 2.f - 100, window.getSize().y / 2.f - 60);
    defeatExitBtn.setPosition(window.getSize().x / 2.f - 100, window.getSize().y / 2.f + 10);

    sf::Text defeatMenuText("Return to menu", font, 20);
    defeatMenuText.setFillColor(sf::Color::White);
    defeatMenuText.setOrigin(defeatMenuText.getLocalBounds().width / 2, defeatMenuText.getLocalBounds().height / 2);
    defeatMenuText.setPosition(defeatMenuBtn.getPosition().x + 100, defeatMenuBtn.getPosition().y + 25);

    sf::Text defeatExitText("Exit", font, 20);
    defeatExitText.setFillColor(sf::Color::White);
    defeatExitText.setOrigin(defeatExitText.getLocalBounds().width / 2, defeatExitText.getLocalBounds().height / 2);
    defeatExitText.setPosition(defeatExitBtn.getPosition().x + 100, defeatExitBtn.getPosition().y + 25);

    enum class TowerType { Cannon, Archer, Wizard };
    TowerType buildType = TowerType::Cannon;
    bool buildMode = false;
    sf::Sprite buildSprite;

    bool castleDestroyed = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (state == GameState::Menu && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (startBtn.getGlobalBounds().contains(mousePos)) {
                    state = GameState::Playing;
                    castleDestroyed = false;
                    enemies.clear();
                    towers.clear();
                    //level1 = Level::Level(Level::level1_points_Bezier, Level::level1_points_Bezier, 1, castle_hp, road_width, Level::level1_castle_cords);
                } else if (exitBtn.getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
            }

            if (state == GameState::Playing && event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (buildMode) {
                        if (level1.is_build_allowed(mousePos)) {
                            switch (buildType) {
                                case TowerType::Cannon: towers.push_back(new Towers::Cannon(mousePos)); break;
                                case TowerType::Archer: towers.push_back(new Towers::Archer(mousePos)); break;
                                case TowerType::Wizard: towers.push_back(new Towers::Wizzard(mousePos)); break;
                            }
                            buildMode = false;
                        }
                    } else if (cannonIcon.getGlobalBounds().contains(mousePos)) {
                        buildMode = true; buildType = TowerType::Cannon; buildSprite.setTexture(cannonTex);
                    } else if (archerIcon.getGlobalBounds().contains(mousePos)) {
                        buildMode = true; buildType = TowerType::Archer; buildSprite.setTexture(archerTex);
                    } else if (wizardIcon.getGlobalBounds().contains(mousePos)) {
                        buildMode = true; buildType = TowerType::Wizard; buildSprite.setTexture(wizardTex);
                    }
                    if (buildSprite.getTexture()) {
                        buildSprite.setOrigin(
                            buildSprite.getTexture()->getSize().x / 2.f,
                            buildSprite.getTexture()->getSize().y / 2.f
                        );
                    }
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    buildMode = false;
                }
            }

            if (state == GameState::Defeat && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (defeatMenuBtn.getGlobalBounds().contains(mousePos)) {
                    state = GameState::Menu;
                } else if (defeatExitBtn.getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
            }
        }

        if (buildMode) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            buildSprite.setPosition(mousePos);
            buildSprite.setColor(level1.is_build_allowed(mousePos) ? sf::Color::White : sf::Color(255, 128, 128));
        }

        if (state == GameState::Playing && !castleDestroyed) {
            if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
                int type = std::rand() % 3;
                if (type == 0) enemies.push_back(new Enemies::Goblin());
                else if (type == 1) enemies.push_back(new Enemies::Orc());
                else enemies.push_back(new Enemies::Golem());
                spawnClock.restart();
            }
            if (moveClock.getElapsedTime().asSeconds() >= moveInterval) {
                for (auto* e : enemies) castleDestroyed = e->tick(level1);
                for (auto* t : towers) t->tick(enemies);
                moveClock.restart();
            }
            if (castleDestroyed) state = GameState::Defeat;
        }

        // --- DRAWING ---
        window.clear();

        if (state == GameState::Menu) {
            window.draw(startBtn); window.draw(exitBtn);
            window.draw(startText); window.draw(exitText);
        }
        else if (state == GameState::Playing) {
            window.draw(background);
            window.draw(route_line); window.draw(left_road); window.draw(right_road);
            for (auto* e : enemies) { window.draw(e->get_sprite()); window.draw(e->get_health_bar()); }
            for (auto* t : towers) window.draw(t->get_sprite());
            window.draw(level1.castle.get_sprite());
            if (buildMode) window.draw(buildSprite);
            window.draw(shopPanel); window.draw(cannonIcon); window.draw(archerIcon); window.draw(wizardIcon);

            int hp = level1.castle.get_health();
            castleHpText.setString(std::to_string(hp) + " HP");
            auto bounds = castleHpText.getLocalBounds();
            castleHpText.setOrigin(bounds.left + bounds.width, bounds.top + bounds.height);
            castleHpText.setPosition(window.getSize().x - 10.0f, window.getSize().y - 30.0f);
            window.draw(castleHpText);
        }
        else if (state == GameState::Defeat) {
            window.draw(defeatSprite);
            window.draw(defeatMenuBtn); window.draw(defeatExitBtn);
            window.draw(defeatMenuText); window.draw(defeatExitText);
        }

        window.display();
    }

    return EXIT_SUCCESS;
}
