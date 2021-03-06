/*************************************************************************
* Copyright © 2011-2012 Vincent Prat & Simon Nicolas
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*************************************************************************/

#ifndef HEADER_SCENARIO
#define HEADER_SCENARIO

#include "Tree.h"
#include "CharacterList.h"
#include "SkillList.h"

/*!
 * \brief Game scenario
 *
 * Contains everything necessary for a game
 */
class Scenario
{
    public:
        // types
        /*!
         * \brief Type of user interface used by the game
         */
        enum UserInterface {
            //! Full interface
            uiFull,
            //! Simple interface with scenario, musics and sounds
            uiSimple,
            //! Interface with only musics and sounds
            uiMusic,
            //! Design-phase interface with scenario, characters and notes
            uiDesign,
            //! Interface without musics and sounds
            uiNoMusic
        };
        // constructors
        Scenario();
        Scenario(const std::string &fileName) throw(xmlpp::exception);
        // loading and saving methods
        /*!
         * \brief Loads a game from a file
         * \param fileName File name of the game to load
         * \throw xmlpp::exception Exception thrown by the XML library when the file is uncorrect
         * \throw std::invalid_argument Thrown when an item has not been loaded correctly
         * \throw std::overflow_error Thrown when the size of a file exceeds the limit (if limited)
         */
        void fromFile(const std::string &fileName) throw(xmlpp::exception, std::invalid_argument, std::overflow_error);
        /*!
         * \brief Saves a game into a file
         * \param fileName Name of the file to be saved
         */
        void toFile(const std::string &fileName) const;
        // accessors
        Tree& scenario();
        std::string& notes();
        Tree& history();
        Tree& music();
        Tree& effects();
        SkillList& skills();
        CharacterList& characters();
        /*!
         * \brief Get the user interface
         * \return Current user interface
         */
        UserInterface userInterface() const;
        /*!
         * \brief Set the user interface
         * \param interface New value of the user interface
         */
        void setUserInterface(UserInterface interface);
        // emptying
        void clear();
        // static methods
        /*!
         * \brief Convertion method from UserInterface to string
         * \param interface User interface
         * \return String equivalent to the interface
         */
        static std::string interfaceToString(UserInterface interface);
        /*!
         * \brief Convertion method form string to UserInterface
         * \param interface String to convert
         * \return User interface equivalent to the string 
         * \throw std::invalid_argument Exception thrown when the string does not correspond to any user interface
         */
        static UserInterface stringToInterface(const std::string& interface) throw(std::invalid_argument);
    private:
        Tree tScenario,tHistory,tMusic,tEffects;
        std::string sNotes;
        CharacterList lCharacters;
        SkillList lSkills;
        //! User interface used by the game
        UserInterface uiInterface;
};

#endif
