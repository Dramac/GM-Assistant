#ifndef HEADER_LIST
#define HEADER_LIST

#include "Model.h"
#include <vector>

class List: public Model
{
    private:
        // data contained in the list
        std::vector<Item> data;
    public:
        // iterator
        class iterator: public Model::iterator
        {            
            private:
                // underlying vector iterator
                std::vector<Item>::const_iterator viIt;
            public:
                // constructor
                iterator(const std::vector<Item>::const_iterator& it, IterationType type=itNormal, State state=sNone);
                // overloaded operators
                bool operator!=(const iterator& it) const;
                bool operator==(const iterator& it) const;
                iterator& operator++();
                iterator operator++(int i);
                Item operator*();
        };
        // constructor
        List(const std::string &fileName = "");
        // inherited methods
        std::string toXML() const;
        void fromXML(const std::string &fileName);
        void clear();
        // iterator-related methods
        iterator begin() const;
        iterator beginUnchecked() const;
        iterator beginState(State state) const;
        iterator end() const;        
        iterator endUnchecked() const;
        iterator endState(State state) const;
        // accessors
        Item operator[](int index) const;   // reader
        Item& operator[](int index);        // writer
        // populating the list
        void insert(int index, const std::string &content, State state);
        void remove(int index);
        void move(int currentIndex, int newIndex);
};

#endif
