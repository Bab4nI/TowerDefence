#include "enemies.hpp"
#include "level.hpp"
#include <SFML/Graphics.hpp>

using namespace Enemies;

Enemy::Enemy(){
    cur_step = 0;
    max_health = 0;
};

bool Enemy::tick(Level::Level& level){
    move();
    std::pair<sf::Vector2f, bool> res = level.get_cords(cur_step);
    if(res.second){
        if(damage_clock.getElapsedTime().asSeconds() >= damage_interval){
            damage_clock.restart();
            if(level.damage_the_castle(damage))
                return 1;
        }
    }
    set_cords(res.first);
    update_health_bar();
    return 0;
}

sf::Texture Enemy::get_texture(){
    return texture;
};

int Enemy::get_speed(){
    return speed;
};

int Enemy::get_health(){
    return health;
};

void Enemy::move(){
    cur_step += speed;
}

sf::Sprite Enemy::get_sprite(){
    return sprite;
}

sf::RectangleShape Enemy::get_health_bar(){
    return health_bar;
}

int Enemy::get_max_health(){
    return max_health;
}

void Enemy::update_health_bar(){
    if(max_health){
        float percentage = static_cast<float>(health) / static_cast<float>(max_health);
        float width = sprite.getGlobalBounds().width;
        float height = 5.f;
        health_bar.setSize(sf::Vector2f(width * percentage, height));
        health_bar.setFillColor(percentage < 0.3f ? sf::Color::Red : sf::Color::Green);
        health_bar.setOrigin(width / 2.f, height / 2.f);
        health_bar.setPosition(cords.x, cords.y - sprite.getGlobalBounds().height / 2.f - 10.f);
    }
}

void Enemy::set_cords(sf::Vector2f crds){
    cords = crds;
    sprite.setPosition(crds);
}

int Enemy::get_step(){
    return cur_step;
}

int Enemy::get_damage(){
    return damage;
}

sf::Vector2f Enemy::get_cords(){
    return cords;
};

// virtual base class destructor
Enemy::~Enemy() {}

Goblin::Goblin(){
    speed = 1;
    health = 5;
    max_health = health;
    damage = 1;
    damage_interval = 1.0f;
    if(!texture.loadFromFile("assets\\images\\goblin_100.png"))
            throw std::runtime_error("Failed to load goblin texture.");
    else{
        sprite = sf::Sprite(texture);
        sprite.setOrigin(sf::Vector2f(50, 50));
        // health_bar.setSize(sf::Vector2f(sprite.getGlobalBounds().width, 5.f));
        // health_bar.setFillColor(sf::Color::Green);
        // health_bar.setOrigin(health_bar.getSize().x / 2.f, health_bar.getSize().y / 2.f);
        update_health_bar();
    }
}

bool Goblin::health_decrease(int hp) {
    health -= hp;
    update_health_bar();
    return(health <= 0);
}

Orc::Orc(){
    speed = 1;
    health = 10;
    max_health = health;
    damage = 2;
    damage_interval = 1.5f;
    if(!texture.loadFromFile("assets\\images\\or\xD1\x81_100.png"))
            throw std::runtime_error("Failed to load orc texture.");
    else{
        sprite = sf::Sprite(texture);
        sprite.setOrigin(sf::Vector2f(50, 50));
        update_health_bar();
    }
}

bool Orc::health_decrease(int hp) {
    health -= hp;
    update_health_bar();
    return(health <= 0);
}

Golem::Golem(){
    speed = 1;
    health = 20;
    max_health = health;
    damage = 4;
    damage_interval = 2.0f;
    if(!texture.loadFromFile("assets\\images\\golem_100.png"))
            throw std::runtime_error("Failed to load golem texture.");
    else{
        sprite = sf::Sprite(texture);
        sprite.setOrigin(sf::Vector2f(50, 50));
        update_health_bar();
    }
}

bool Golem::health_decrease(int hp) {
    health -= hp;
    update_health_bar();
    return(health <= 0);
}