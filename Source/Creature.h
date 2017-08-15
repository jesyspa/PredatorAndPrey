#pragma once
#include <SFML/Graphics.hpp>

enum class CreatureType
{
    Nothing     = 0,
    Prey        = 1,
    Predator    = 2,
};

struct Creature
{
    Creature();
    explicit Creature(CreatureType type);

    sf::Color getColour();
    void update();
    void heal(int amount);

    void reproduce(Creature& other);
    void move(Creature& other);

    CreatureType type;
    static const int MAX_HEALTH = 100;
    static const int INITIAL_HEALTH = MAX_HEALTH / 5;
    int health = INITIAL_HEALTH;

    static Creature const nothing;
};
