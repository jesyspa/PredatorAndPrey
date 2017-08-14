#pragma once
#include "Creature.h"

#include <SFML/Graphics.hpp>
#include <vector>

class Application
{
        Application(unsigned width, unsigned height);

        unsigned getIndex(unsigned x, unsigned y) const;
        bool isRunning() const;

        void handleInput(float dt);
        void clear();
        void update();
        void draw();
        void pollEvents();

        void setCellColour(unsigned x, unsigned y, sf::Uint8 colour);

        void updatePredator (Creature& thisCreature, Creature& otherCreature);
        void updatePrey     (Creature& thisCreature, Creature& otherCreature);

        void handleContinuousInput(float dt);
        void handleDiscreteInput();

        template<typename F>
        void forEachCell(F f);

        const unsigned WIDTH,
                       HEIGHT;

        sf::RenderWindow m_window;
        std::vector<sf::Vertex> m_pixels;
        std::vector<Creature> m_creatures;

        int m_preyCount     = 0;
        int m_predatorCount = 0;
        int m_frameCount    = 0;

        sf::Font m_font;
        sf::Text m_preyCountText;
        sf::Text m_predatorCountText;
        sf::Text m_frameCountText;

        sf::View m_view;
        sf::RectangleShape m_outline;

        sf::Clock inputDelayClock;

        friend void runApplication(unsigned width, unsigned height);
};

template<typename F>
void Application::forEachCell(F f) {
    for (unsigned x = 0; x < WIDTH; ++x)
        for (unsigned y = 0; y < HEIGHT; ++y)
            f(x, y);
}

void runApplication(unsigned width, unsigned height);
