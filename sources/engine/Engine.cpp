#include "Engine.h"

using namespace std;

// constructors

Engine::Engine()
{
}

Engine::Engine(const string &fileName) throw(xmlpp::exception)
{
    try
    {
        fromFile(fileName);
    }
    catch (xmlpp::exception)
    {
        clear();
        throw;
    }
}

// XML methods

void Engine::fromFile(const std::string &fileName) throw(xmlpp::exception)
{
    using namespace xmlpp;

    DomParser parser(fileName);
    Document *document = parser.get_document();
    Element *root = document->get_root_node();
    if (root->get_name()!="game")
    {
        throw xmlpp::exception("Bad document content type: game expected");
    }
    // now loading the different parts of the game
    Node::NodeList node = root->get_children("scenario");
    if (!node.empty())
    {
        tScenario.fromXML(*dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("notes");
    if (!node.empty())
    {
        Element *elem = dynamic_cast<Element*>(node.front());
        if (elem->has_child_text())
        {
            sNotes = elem->get_child_text()->get_content();
        }
        else
        {
            sNotes = "";
        }
    }
    node = root->get_children("characters");
    if (!node.empty())
    {
        node = node.front()->get_children("character");
        for (Node::NodeList::const_iterator it = node.begin(); it != node.end(); it++)
        {
            Element *elem = dynamic_cast<Element*>(*it);
            string name="";
            Attribute *attr = elem->get_attribute("name");
            if (attr != NULL)
            {
                name = attr->get_value();
            }
            string playerName="";
            attr = elem->get_attribute("playername");
            if (attr != NULL)
            {
                playerName = attr->get_value();
            }
            Character character = Character(name,playerName);
            character.fromXML(*elem);
            vCharacters.push_back(character);
        }        
    }
    node = root->get_children("history");
    if (!node.empty())
    {
        tHistory.fromXML(*dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("music");
    if (!node.empty())
    {
        tMusic.fromXML(*dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("effects");
    if (!node.empty())
    {
        tEffects.fromXML(*dynamic_cast<Element*>(node.front()));
    }
}

void Engine::toFile(const string &fileName) const
{
    using namespace xmlpp;

    Document document;
    Element *root = document.create_root_node("game");
    Element *tmp = root->add_child("scenario");
    tScenario.toXML(*tmp);
    tmp = root->add_child("notes");
    tmp->add_child_text(sNotes);
    tmp = root->add_child("characters");
    for (vector<Character>::const_iterator it = vCharacters.begin(); it != vCharacters.end(); it++)
    {
        Element *tmp2 = tmp->add_child("character");
        tmp2->set_attribute("name",it->name());
        tmp2->set_attribute("playername",it->playerName());
        it->toXML(*tmp2);
    }
    tmp = root->add_child("history");
    tHistory.toXML(*tmp);
    tmp = root->add_child("music");
    tMusic.toXML(*tmp);
    tmp = root->add_child("effects");
    tEffects.toXML(*tmp);
    document.write_to_file_formatted(fileName,"UTF-8");
}

// accessors

Tree& Engine::scenario()
{
    return tScenario;
}

string& Engine::notes()
{
    return sNotes;
}

Tree& Engine::history()
{
    return tHistory;
}

Tree& Engine::music()
{
    return tMusic;
}

Tree& Engine::effects()
{
    return tEffects;
}

SoundEngine& Engine::soundEngine()
{
    return sSoundEngine;
}

Character& Engine::character(int index) throw(std::out_of_range)
{
    if (index<0 || (unsigned int)index >= vCharacters.size())
    {
        throw out_of_range("Index out of bounds");
    }
    return vCharacters[index];
}

// methods

void Engine::clear()
{
    tScenario.clear();
    sNotes = "";
    tHistory.clear();
    tMusic.clear();
    tEffects.clear();
}

void Engine::addCharacter(const Character &character)
{
    vCharacters.push_back(character);
}

void Engine::removeCharacter(int index) throw(out_of_range)
{
    if (index<0 || (unsigned int)index >= vCharacters.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vCharacters.erase(vCharacters.begin()+index);
}