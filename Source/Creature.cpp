#include "Creature.h"

#include "Random.h"
#include <algorithm>

Creature::Creature() : type(CreatureType::Nothing)
{}

Creature::Creature(CreatureType type) : type(type)
{}

sf::Color Creature::getColour()
{
    if (health == 0)
        return sf::Color::Black;

    float normalisedHealth = (float)health / (float)MAX_HEALTH;
    uint8_t col = normalisedHealth * 255;

    switch (type)
    {
        case CreatureType::Predator:
            return {col, 0, 0};

        case CreatureType::Prey:
            return {0, col, 0};

        default:
            return sf::Color::Black;
    }
}

void Creature::heal(int amount)
{
    health += amount;
    health = std::min(health, MAX_HEALTH);
}

void Creature::update()
{
    switch (type)
    {
        case CreatureType::Predator:
            heal(-1);
            break;

        case CreatureType::Prey:
            heal(1);
            break;

        default:
            break;
    }
}

void Creature::reproduce(Creature& other)
{
    other.health = 10;
    other.type   = CreatureType::Prey;
}

void Creature::move(Creature& other)
{
    other.health = health;
    other.type   = type;
    type = CreatureType::Nothing;
}

Creature const Creature::nothing(CreatureType::Nothing);
int const Creature::MAX_HEALTH;
