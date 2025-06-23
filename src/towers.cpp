#include "towers.hpp"
#include <cmath>

using namespace Towers;

Tower::Tower(sf::Vector2f crds){
    cords = crds;
};

void Tower::tick(std::vector<Enemies::Enemy*>& enemies){
    float dx, dy, sqared_dist;
    bool fired = false;
    for(int i = 0; i < enemies.size() && (damage_clock.getElapsedTime().asSeconds() >= damage_interval);){
        Enemies::Enemy* e = enemies[i];
        sf::Vector2f e_cords = e->get_cords();

        dx = e_cords.x - cords.x;
        dy = e_cords.y - cords.y;
        sqared_dist = (dx * dx + dy * dy);
        if(sqared_dist <= range * range){
            if(e->health_decrease(damage)){
                delete e;
                enemies.erase(enemies.begin() + i);
                continue; // shifts aim when the enemy is killed  
            }
            damage_clock.restart();
        }
        i++;  
    };
}

sf::Sprite Tower::get_sprite(){
    return sprite;
}

// virtual
Tower::~Tower(){}

Cannon::Cannon(sf::Vector2f crds) : Tower(crds)
{
    range = 200;
    damage = 2;
    damage_interval = 2;

    if (!texture.loadFromFile("assets/images/cannon_100.png"))
        throw std::runtime_error("Failed to load cannon texture");
    
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(crds);
}

Archer::Archer(sf::Vector2f crds) : Tower(crds)
{
    range = 150;
    damage = 1;
    damage_interval = 1;

    if (!texture.loadFromFile("assets/images/archer_100.png"))
        throw std::runtime_error("Failed to load archer texture");

    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(crds);
}

Wizzard::Wizzard(sf::Vector2f crds) : Tower(crds)
{
    range = 250;
    damage = 1;
    damage_interval = 0.5;

    if (!texture.loadFromFile("assets/images/wizzard_100.png"))
        throw std::runtime_error("Failed to load wizzard texture");

    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(crds);
}