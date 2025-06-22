#include "enemies.hpp"
#include <SFML/Graphics.hpp>

using namespace Enemies;

Enemy::Enemy(){
    cur_step = 0;
};

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

// virtual base class destructor
Enemy::~Enemy() {}

Goblin::Goblin(){
    speed = 1;
    health = 10;
    damage = 1;
    if(!texture.loadFromFile("assets\\images\\goblin.png"))
            throw std::runtime_error("Failed to load goblin texture.");
    else
        sprite = sf::Sprite(texture); 
}

bool Goblin::health_decrease(int hp) {
    health -= hp;
    return(health > 0);
}




