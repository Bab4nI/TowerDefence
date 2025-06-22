#include "enemies.hpp"
#include "level.hpp"
#include <SFML/Graphics.hpp>

using namespace Enemies;

Enemy::Enemy(){
    cur_step = 0;
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

// virtual base class destructor
Enemy::~Enemy() {}

Goblin::Goblin(){
    speed = 1;
    health = 10;
    damage = 1;
    damage_interval = 1.0f;
    if(!texture.loadFromFile("assets\\images\\goblin_100.png"))
            throw std::runtime_error("Failed to load goblin texture.");
    else{
        sprite = sf::Sprite(texture);
        sprite.setOrigin(sf::Vector2f(50, 50));
    }
}

bool Goblin::health_decrease(int hp) {
    health -= hp;
    return(health > 0);
}




