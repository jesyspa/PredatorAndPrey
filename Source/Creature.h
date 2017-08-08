#ifndef CREATURE_H_INCLUDED
#define CREATURE_H_INCLUDED

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

        sf::Color getColour() noexcept;

        CreatureType getType() const noexcept;
        void setType(CreatureType type) noexcept;

        void heal(int amount);
        int  getHealth() const noexcept;
        void setHealth(int val) noexcept;
        void update();

        void reproduce(Creature& other);

        void move(Creature& other);

    private:
        CreatureType m_type;

        int m_health = MAX_HEALTH / 5;
};

#endif // CREATURE_H_INCLUDED
