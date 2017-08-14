#pragma once
#include <SFML/Graphics.hpp>

constexpr  int MAX_HEALTH = 100;

enum class CreatureType
{
    Predator    = 0,
    Prey        = 1,
    Nothing     = 2
};

class Creature
{
    public:
        Creature();

        sf::Color getColour();

        CreatureType getType() const;
        void setType(CreatureType type);

        void heal(int amount);
        int  getHealth() const;
        void setHealth(int val);
        void update();

        void reproduce(Creature& other);

        void move(Creature& other);

    private:
        CreatureType m_type;

        int m_health = MAX_HEALTH / 5;
};
