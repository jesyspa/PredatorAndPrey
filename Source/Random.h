#pragma once
#include <random>

class Random
{
    public:
        static Random& get();
        int intInRange(int low, int high);
        bool boolWithChance(double chance);
        int intWithChance(std::initializer_list<double> weights);

        Random(const Random& other) = delete;
        Random(Random&& other) = delete;

        Random& operator =(const Random& other) = delete;
        Random& operator =(Random&& other) = delete;

    private:
        Random();
        std::mt19937 m_randEngine;
};
