#pragma once
#include "Creature.h"
#include "World.h"

#include <SFML/Graphics.hpp>
#include <vector>

class Application
{
    public:
        Application(unsigned width, unsigned height);

        bool isRunning() const;

        void handleInput(float dt);
        void clear();
        void update();
        void draw();
        void pollEvents();

    private:
        void handleContinuousInput(float dt);
        void handleDiscreteInput();

        sf::RenderWindow m_window;
        World m_world;
        int m_frameCount    = 0;

        sf::Font m_font;
        sf::Text m_preyCountText;
        sf::Text m_predatorCountText;
        sf::Text m_frameCountText;

        sf::View m_view;
        sf::RectangleShape m_outline;

        sf::Clock inputDelayClock;
};

void runApplication(unsigned width, unsigned height);
