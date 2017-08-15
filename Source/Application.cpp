#include "Application.h"
#include "Random.h"

#include <iostream>

Application::Application(unsigned width, unsigned height)
:   m_window    ({width, height}, "Predator And Prey")
,   m_world     (width, height)
,   m_view      ({0, 0}, {(float)width, (float)height})
{
    m_view.setCenter(width / 2., height / 2.);
    m_window.setFramerateLimit(60);

    m_outline.setFillColor({0, 0, 0, 0});
    m_outline.setOutlineColor(sf::Color::White);
    m_outline.setOutlineThickness(5);
    m_outline.setSize({(float)width, (float)height});

    m_world.populate();

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

bool Application::isRunning() const
{
    return m_window.isOpen();
}

void Application::clear()
{
    m_window.clear();
}

void Application::update()
{
    m_world.update();

    ++m_frameCount;

    m_predatorCountText.setString   ("Predator: " + std::to_string(m_world.predatorCount()));
    m_preyCountText.setString       ("Prey    : " + std::to_string(m_world.preyCount()));
    m_frameCountText.setString      ("Steps   : " + std::to_string(m_frameCount));
}

void Application::draw()
{
    m_window.setView(m_view);
    m_world.draw(m_window);
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
        m_world.addPrey();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        inputDelayClock.restart();
        m_world.addPredators();
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

