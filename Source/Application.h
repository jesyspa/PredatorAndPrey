#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

#include "Creature.h"

class Application
{
    public:
        Application(int width, int height);

        void run();

    private:
        int getIndex(int x, int y);

        void handleInput(float dt);
        void pollEvents();
        void setCellColour(int x, int y, sf::Uint8 colour);
        void update();

        void updatePredator (Creature& thisCreature, Creature& otherCreature);
        void updatePrey     (Creature& thisCreature, Creature& otherCreature);

        const int WIDTH, HEIGHT;

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



};

#endif // APPLICATION_H_INCLUDED
