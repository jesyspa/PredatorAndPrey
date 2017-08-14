#include "Application.h"
#include "Random.h"

#include <iostream>

Application::Application(unsigned width, unsigned height)
:   WIDTH (width)
,   HEIGHT(height)
,   m_window    ({WIDTH, HEIGHT}, "Predator And Prey")
,   m_pixels    (WIDTH * HEIGHT)
,   m_creatures (WIDTH * HEIGHT)
,   m_view      ({0, 0}, {(float)WIDTH, (float)HEIGHT})
{
    m_view.setCenter(WIDTH / 2, HEIGHT / 2);
    m_window.setFramerateLimit(60);

    m_outline.setFillColor({0, 0, 0, 0});
    m_outline.setOutlineColor(sf::Color::White);
    m_outline.setOutlineThickness(5);
    m_outline.setSize({(float)WIDTH, (float)HEIGHT});

    forEachCell([&](auto x, auto y) {
        auto index = getIndex(x, y);
        m_pixels[index].position    = {(float)x, (float)y};
        m_pixels[index].color       = m_creatures[index].getColour();
        auto type = m_creatures[index].getType();
        switch(type)
        {
            case CreatureType::Prey:
                m_preyCount++;
                break;

            case CreatureType::Predator:
                m_predatorCount++;
                break;

            case CreatureType::Nothing:
                break;
        }
    });
    m_font.loadFromFile         ("font/arial.ttf");
    m_predatorCountText.setFont (m_font);
    m_preyCountText.setFont     (m_font);
    m_frameCountText.setFont    (m_font);

    m_predatorCountText.setCharacterSize(15);
    m_preyCountText.setCharacterSize(15);
    m_frameCountText.setCharacterSize(15);
    m_predatorCountText.move    (8, 0);
    m_preyCountText.move        (8, 20);
    m_frameCountText.move       (8, 40);
}

unsigned Application::getIndex(unsigned x, unsigned y) const
{
    return y * WIDTH + x;
}

bool Application::isRunning() const
{
    return m_window.isOpen();
}

void Application::setCellColour(unsigned x, unsigned y, sf::Uint8 colour)
{
    auto index = getIndex(x, y);
    m_pixels[index].color = {colour, colour, colour};
}

void Application::clear()
{
    m_window.clear();
}

void Application::update()
{
    forEachCell([&](auto x, auto y) {
        auto index = getIndex(x, y);
        auto& thisCreature = m_creatures[index];
        auto thisType  = thisCreature.getType();

        if (thisType == CreatureType::Nothing)
            return;

        unsigned xChange = Random::get().intInRange(-1, 1);
        unsigned yChange = Random::get().intInRange(-1, 1);
        unsigned xAdj = x + xChange;
        unsigned yAdj = y + yChange;

        if (xAdj >= WIDTH ) return;
        if (yAdj >= HEIGHT) return;

        auto adjIndex       = getIndex(xAdj, yAdj);
        auto& otherCreature = m_creatures[adjIndex];

        thisCreature.update();
        switch(thisType)
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

        sf::Vertex& v = m_pixels[index];
        v.color = thisCreature.getColour();
    });

    ++m_frameCount;

    m_predatorCountText.setString   ("Predator: " + std::to_string(m_predatorCount));
    m_preyCountText.setString       ("Prey    : " + std::to_string(m_preyCount));
    m_frameCountText.setString      ("Steps   : " + std::to_string(m_frameCount));
}

void Application::draw()
{
    m_window.setView(m_view);
    m_window.draw(m_pixels.data(), m_pixels.size(), sf::Points);
    m_window.draw(m_outline);

    m_window.setView(m_window.getDefaultView());
    m_window.draw(m_predatorCountText);
    m_window.draw(m_preyCountText);
    m_window.draw(m_frameCountText);

    m_window.display();
}

void Application::pollEvents()
{
    sf::Event e;
    while (m_window.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
        {
            m_window.close();
        }
    }
}


void Application::updatePredator(Creature& thisCreature, Creature& otherCreature)
{
    if (thisCreature.getHealth() <= 0)
    {
        m_predatorCount--;
        thisCreature.setType(CreatureType::Nothing);
        return;
    }

    auto otherType  = otherCreature.getType();

    switch(otherType)
    {
        case CreatureType::Prey:
            m_preyCount--;
            m_predatorCount++;
            otherCreature.setType(CreatureType::Predator);
            thisCreature.heal(otherCreature.getHealth());
            break;

        case CreatureType::Predator:
            break;

        case CreatureType::Nothing:
            thisCreature.move(otherCreature);
            break;
    }
}

void Application::updatePrey(Creature& thisCreature, Creature& otherCreature)
{
    auto otherType  = otherCreature.getType();

    bool reproduce = false;
    if (thisCreature.getHealth() >= MAX_HEALTH)
    {
        thisCreature.setHealth(10);
        reproduce = true;
    }

    if (otherType != CreatureType::Nothing)
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

void Application::handleInput(float dt)
{
    if (!m_window.hasFocus())
        return;
    if (inputDelayClock.getElapsedTime().asSeconds() > 0.5)
        handleDiscreteInput();
    handleContinuousInput(dt);
}

void Application::handleDiscreteInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
    {
        inputDelayClock.restart();
        forEachCell([&](auto x, auto y)
        {
            auto index = getIndex(x, y);
            auto type = m_creatures[index].getType();

            if (type == CreatureType::Nothing && Random::get().boolWithChance(0.7))
            {
                m_creatures[index].setType(CreatureType::Prey);
                m_preyCount++;
            }
        });
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        inputDelayClock.restart();
        forEachCell([&](auto x, auto y) {
            auto index = getIndex(x, y);
            auto type = m_creatures[index].getType();

            if (type == CreatureType::Nothing && Random::get().boolWithChance(0.7))
            {
                m_creatures[index].setType(CreatureType::Predator);
                m_creatures[index].heal(100);
                m_predatorCount++;
            }
        });
    }
}

void Application::handleContinuousInput(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        m_view.move(0, -100 * dt);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_view.move(0, 100 * dt);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_view.move(-100 * dt, 0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        m_view.move(100 * dt, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        m_view.zoom(1.1);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_view.zoom(0.9);
    }
}

void runApplication(unsigned width, unsigned height)
{
    Application app(width, height);
    sf::Clock deltaClock;
    while (app.isRunning())
    {
        auto deltaTime = deltaClock.restart().asSeconds();
        app.clear();

        app.handleInput(deltaTime);
        app.update();
        app.draw();
        app.pollEvents();
    }
}

