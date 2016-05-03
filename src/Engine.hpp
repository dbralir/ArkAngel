#ifndef ARKANGEL_ENGINE_HPP
#define ARKANGEL_ENGINE_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include "Terminal.hpp"

class Engine
{
private:
    sfg::SFGUI Sfgui;
    sfg::Desktop ConsoleDesktop;
    sf::Clock SFGClock;
    Terminal::Ptr Console;

    sf::Clock EngineClock;
    float TotalTime;

    void poll_events();
    void update();
    void draw();

public:
    sf::RenderWindow window;

    Engine();
    void go();
};

#endif //ARKANGEL_ENGINE_HPP
