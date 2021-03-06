//
// Created by Anthony on 5/18/2016.
//

#include "ScriptSandbox.hpp"

#include <fstream>
#include <regex>

using namespace std;

ScriptSandbox::ScriptSandbox(string suid, Engine *engine) :
    engine(engine),
    SUID(suid)
{
    int r;
    r = engine->ScriptEngine->RegisterGlobalFunction("void executeScript(string)", asMETHODPR(ScriptSandbox, executeScript, (string), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void executeScript(string, string)", asMETHODPR(ScriptSandbox, executeScript, (string, string), void), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void startNewModule(string)", asMETHOD(ScriptSandbox, startNewModule), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void addSection(string)", asMETHOD(ScriptSandbox, addSection), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void buildModule()", asMETHOD(ScriptSandbox, buildModule), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void defineWord(string)", asMETHOD(ScriptSandbox, defineWord), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("void removeModule(string)", asMETHOD(ScriptSandbox, removeModule), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);
    r = engine->ScriptEngine->RegisterGlobalFunction("string addScriptAsModule(string)", asMETHOD(ScriptSandbox, addScriptAsModule), asCALL_THISCALL_ASGLOBAL, this); assert(r >= 0);

    engine->Console->Show(true);
}

ScriptSandbox::~ScriptSandbox()
{
    engine->Console->Show(false);
}

void ScriptSandbox::handleEvent(sf::Event event)
{
    engine->ConsoleDesktop.HandleEvent(event);
}

void ScriptSandbox::update()
{
    if (engine->wasKeyPressed(sf::Keyboard::Escape))
    {
        engine->Gsm.pop();
        return;
    }

    if (Clock.getElapsedTime().asMicroseconds() >= 5000)
    {
        engine->ConsoleDesktop.Update(static_cast<float>( Clock.getElapsedTime().asMicroseconds() ) / 1000000.f);
        Clock.restart();
    }
}

void ScriptSandbox::draw()
{
    engine->Window.clear(sf::Color::Red);
}

void ScriptSandbox::executeScript(string filename)
{
    ifstream ifs(filename);
    if (!ifs.good())
    {
        PrintErr("\"%$\" doesn't exist\n", filename);
        return;
    }

    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    ExecuteString(engine->ScriptEngine, str.c_str(), engine->ConsoleModule);
}

void ScriptSandbox::executeScript(std::string filename, std::string moduleName)
{
    ifstream ifs(filename);
    if (!ifs.good())
    {
        PrintErr("\"%$\" doesn't exist\n", filename);
        return;
    }

    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    ExecuteString(engine->ScriptEngine, str.c_str(), engine->ScriptEngine->GetModule(moduleName.c_str()));
}

void ScriptSandbox::startNewModule(std::string moduleName)
{
    builder.StartNewModule(engine->ScriptEngine, moduleName.c_str());
    engine->Gsm.registerModule(SUID, moduleName);
}

void ScriptSandbox::addSection(std::string filename)
{
    builder.AddSectionFromFile(filename.c_str());
}

void ScriptSandbox::buildModule()
{
    builder.BuildModule();
}

void ScriptSandbox::defineWord(std::string word)
{
    builder.DefineWord(word.c_str());
}

void ScriptSandbox::removeModule(std::string moduleName)
{
    engine->ScriptEngine->DiscardModule(moduleName.c_str());
}

string ScriptSandbox::addScriptAsModule(std::string filename)
{
    regex rgx("^(?:(.*)[\\/\\\\])?(.*?)(?:\\.(.*))?$");
    smatch match;

    if (std::regex_search(filename, match, rgx))
    {
        string name = match[2];
        PrintLog("Module Name \"%$\"\n", name);

        builder.StartNewModule(engine->ScriptEngine, name.c_str());
        builder.AddSectionFromFile(filename.c_str());
        builder.BuildModule();

        engine->Gsm.registerModule(SUID, name);

        return name;
    }
    else
    {
        PrintErr("\"%$\" is an invalid filename.\n", filename);
        return "";
    }
}
