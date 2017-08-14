#include "Random.h"

#include <ctime>

Random::Random()
:   m_randEngine(std::time(nullptr))
{ }

Random& Random::get()
{
    static Random rand;
    return rand;
}

int Random::intInRange(int low, int high)
{
    std::uniform_int_distribution<int> dist(low, high);
    return dist(m_randEngine);
}

bool Random::boolWithChance(double chance)
{
    std::bernoulli_distribution dist(chance);
    return dist(m_randEngine);
}
