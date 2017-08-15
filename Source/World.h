#pragma once
#include "Creature.h"

#include <SFML/Graphics.hpp>
#include <vector>

class World {
        unsigned WIDTH,
                 HEIGHT;
        std::vector<sf::Vertex> m_pixels;
        std::vector<Creature> m_creatures;

        int m_preyCount     = 0;
        int m_predatorCount = 0;

        unsigned getIndex(unsigned x, unsigned y) const;

        void updatePredator (Creature& thisCreature, Creature& otherCreature);
        void updatePrey     (Creature& thisCreature, Creature& otherCreature);

        template<typename F>
        void forEachCell(F f) const;

    public:
        World(unsigned width, unsigned height);

        void populate();
        void update();
        void addPredators();
        void addPrey();

        void draw(sf::RenderWindow& window) const;

        int preyCount() const;
        int predatorCount() const;
};

template<typename F>
void World::forEachCell(F f) const {
    for (unsigned x = 0; x < WIDTH; ++x)
        for (unsigned y = 0; y < HEIGHT; ++y)
            f(x, y);
}
