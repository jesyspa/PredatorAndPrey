#include "World.h"
#include "Random.h"

World::World(unsigned width, unsigned height)
:   WIDTH (width)
,   HEIGHT(height)
,   m_pixels    (WIDTH * HEIGHT)
,   m_creatures (WIDTH * HEIGHT)
{}

unsigned World::getIndex(unsigned x, unsigned y) const
{
    return y * WIDTH + x;
}

void World::populate()
{
    m_preyCount = 0;
    m_predatorCount = 0;
    forEachCell([&](auto x, auto y)
    {
        auto index = getIndex(x, y);
        m_creatures[index].type = (CreatureType)Random::get().intWithChance({0.9, 0.05, 0.05});
        m_creatures[index].health = Creature::INITIAL_HEALTH;
        if (m_creatures[index].type == CreatureType::Prey)
            ++m_preyCount;
        else if (m_creatures[index].type == CreatureType::Predator)
            ++m_predatorCount;
        m_pixels[index].position    = {(float)x, (float)y};
        m_pixels[index].color       = m_creatures[index].getColour();
    });
}

void World::update()
{
    forEachCell([&](auto x, auto y) {
        auto index = getIndex(x, y);
        auto& thisCreature = m_creatures[index];

        if (thisCreature.type == CreatureType::Nothing)
            return;

        // Note: using unsigned here is fine, since unsigned arithmetic is modulo arithmetic,
        // and -1 modulo N still acts as -1 under addition..
        unsigned xChange = Random::get().intInRange(-1, 1);
        unsigned yChange = Random::get().intInRange(-1, 1);
        unsigned xAdj = x + xChange;
        unsigned yAdj = y + yChange;

        if (xAdj >= WIDTH ) return;
        if (yAdj >= HEIGHT) return;

        auto adjIndex       = getIndex(xAdj, yAdj);
        auto& otherCreature = m_creatures[adjIndex];

        thisCreature.update();
        switch (thisCreature.type)
        {
            case CreatureType::Predator:
                updatePredator(thisCreature, otherCreature);
                break;

            case CreatureType::Prey:
                updatePrey(thisCreature, otherCreature);
                break;

            default:
                break;
        }

        m_pixels[index].color = thisCreature.getColour();
    });
}

void World::updatePredator(Creature& thisCreature, Creature& otherCreature)
{
    if (thisCreature.health <= 0)
    {
        m_predatorCount--;
        thisCreature.type = CreatureType::Nothing;
        return;
    }

    switch(otherCreature.type)
    {
        case CreatureType::Prey:
            m_preyCount--;
            m_predatorCount++;
            otherCreature.type = CreatureType::Predator;
            thisCreature.heal(otherCreature.health);
            break;

        case CreatureType::Predator:
            break;

        case CreatureType::Nothing:
            thisCreature.move(otherCreature);
            break;
    }
}

void World::updatePrey(Creature& thisCreature, Creature& otherCreature)
{
    bool reproduce = false;
    if (thisCreature.health >= Creature::MAX_HEALTH)
    {
        thisCreature.health = 10;
        reproduce = true;
    }

    if (otherCreature.type != CreatureType::Nothing)
        return;

    if (reproduce)
    {
        m_preyCount++;
        thisCreature.reproduce(otherCreature);
    }
    else
    {
        thisCreature.move(otherCreature);
    }
}

void World::addPredators()
{
    forEachCell([&](auto x, auto y) {
        auto index = getIndex(x, y);
        auto& creature = m_creatures[index];

        if (creature.type == CreatureType::Nothing && Random::get().boolWithChance(0.7))
        {
            creature.type = CreatureType::Predator;
            creature.heal(100);
            m_predatorCount++;
        }
    });
}

void World::addPrey()
{
    forEachCell([&](auto x, auto y)
    {
        auto index = getIndex(x, y);
        auto& creature = m_creatures[index];

        if (creature.type == CreatureType::Nothing && Random::get().boolWithChance(0.7))
        {
            creature.type = CreatureType::Prey;
            m_preyCount++;
        }
    });
}

void World::draw(sf::RenderWindow& window) const
{
    window.draw(m_pixels.data(), m_pixels.size(), sf::Points);
}

int World::predatorCount() const
{
    return m_predatorCount;
}

int World::preyCount() const
{
    return m_preyCount;
}
