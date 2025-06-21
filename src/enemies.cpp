#include <SFML/Graphics.hpp>

class Enemy{
public:
    sf::Texture get_texture(){
        return texture;
    }
    
    int get_speed(){
        return speed;
    }

    int get_health(){
        return health;
    }

    virtual bool health_decrease(int hp) = 0;

protected:
    float speed;
    int health;
    int cur_step;
    sf::Texture texture;
};

