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

#ifndef HEADER_ITEM
#define HEADER_ITEM

#include <string>
#include <stdexcept>
#include <libxml++/libxml++.h>

/*!
 * \brief Base item class
 */
class Item
{
    public:
        // Different possible states for the item   
        enum State {sNone,sProgress,sSuccess,sFailure};
        /*!
         * \brief Type of item
         */
        enum Type
        {
            //! Basic item
            tBasic,
            //! Item related to a file
            tFile,
            //! Item related to a sound file
            tSound,
            //! Item related to an image file
            tImage
        };
        // constructor
        Item(const std::string &content="", State state=sNone);
        // accessors
        std::string content() const;
        void setContent(const std::string &content);
        State state() const;
        void setState(State state);
        virtual Type type() const;
        // XML-related methods
        virtual void fromXML(const xmlpp::Element &root);
        virtual void toXML(xmlpp::Element &root);
        // static methods to get a link between strings and state or type
        static std::string stateToStr(State state);
        /*!
         * \brief Conversion method from Type to string
         * \param type Type
         * \return String equivalent to the type
         */
        static std::string typeToStr(Type type);
        static State strToState(const std::string &name) throw(std::invalid_argument);
        /*!
         * \brief Conversion method from string to Type
         * \param name Name of the type
         * \return Type equivalent to the string
         */
        static Type strToType(const std::string &name) throw(std::invalid_argument);
    private:
        State sState;
        std::string sContent;
};

#endif
