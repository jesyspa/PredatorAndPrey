#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <random>

class Random
{
    public:
        static Random& get();
        int intInRange(int low, int high);

        Random(const Random& other) = delete;
        Random(Random&& other) = delete;

        Random& operator =(const Random& other) = delete;
        Random& operator =(Random&& other) = delete;

    private:
        Random();
        std::mt19937 m_randEngine;

};

#endif // RANDOM_H_INCLUDED
