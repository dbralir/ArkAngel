//
// Created by Anthony on 5/18/2016.
//

#ifndef ARKANGEL_SCRIPTPLAYGROUND_HPP
#define ARKANGEL_SCRIPTPLAYGROUND_HPP

#include "Engine.hpp"

#include <scriptbuilder.h>

class ScriptSandbox {
    Engine* engine;
    const std::string SUID;

    CScriptBuilder builder;
    sf::Clock Clock;

    void executeScript(std::string filename);
    void executeScript(std::string filename, std::string moduleName);

    void startNewModule(std::string moduleName);
    void addSection(std::string filename);
    void buildModule();
    void defineWord(std::string word);
    void removeModule(std::string moduleName);

    std::string addScriptAsModule(std::string filename);

public:
    ScriptSandbox() = default;
    ScriptSandbox(std::string suid, Engine *engine);
    ~ScriptSandbox();
    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return true; }
    void handleEvent(sf::Event event);
    void update();
    void draw();
    const std::string getSUID() { return SUID; }
    void onPause() {}
    void onResume() {}
};


#endif //ARKANGEL_SCRIPTPLAYGROUND_HPP
