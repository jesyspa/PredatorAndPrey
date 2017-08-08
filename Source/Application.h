#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

#include "Creature.h"

constexpr int WIDTH = 500, HEIGHT = 400;

class Application
{
    public:
        Application();

        void run();

    private:
        void pollEvents();
        void setCellColour(int x, int y, sf::Uint8 colour);
        void update();

        void updatePredator (Creature& thisCreature, Creature& otherCreature);
        void updatePrey     (Creature& thisCreature, Creature& otherCreature);


        sf::RenderWindow m_window;
        std::vector<sf::Vertex> m_pixels;
        std::vector<Creature> m_creatures;

        int m_preyCount = 0;
        int m_predatorCount = 0;
        int m_frameCount;

        sf::Font m_font;
        sf::Text m_preyCountText;
        sf::Text m_predatorCountText;
        sf::Text m_frameCountText;

};

#endif // APPLICATION_H_INCLUDED
