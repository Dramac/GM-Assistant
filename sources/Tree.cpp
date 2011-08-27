#include "Tree.h"
#include <sstream>
// for debugging
/*#include <iostream>
#include <typeinfo>
*/

using namespace std;

// constructors

Tree::Tree()
{
}

Tree::Tree(const Tree &tree)
{
    for (vector<Branch*>::const_iterator it=tree.vChildren.begin(); it != tree.vChildren.end(); it++)
    {
        Branch *branch = new Branch(**it);
        vChildren.push_back(branch);
    }
}

Tree::Tree(const xmlpp::Element &root)
{
    fromXML(root);
}

Tree::Tree(const string &fileName)
{
    fromXML(fileName);
}

// destructor

Tree::~Tree()
{
    clear();
}

// copy operator

Tree& Tree::operator=(const Tree &tree)
{
    clear();
    for (vector<Branch*>::const_iterator it=tree.vChildren.begin(); it != tree.vChildren.end(); it++)
    {
        Branch *branch = new Branch(**it);
        vChildren.push_back(branch);
    }
    return *this;
}

// inherited methods

void Tree::toXML(const string &fileName) const
{
    using namespace xmlpp;

    Document document;
    Element *root = document.create_root_node("tree");
    toXML(*root);
    document.write_to_file_formatted(fileName,"UTF-8");
}

void Tree::toXML(xmlpp::Element &root) const
{
    using namespace xmlpp;

    for (vector<Branch*>::const_iterator it=vChildren.begin(); it != vChildren.end(); it++)
    {
        Element *tmp = root.add_child("item");
        stringstream buf(stringstream::in | stringstream::out);
        buf << (*it)->first.second;
        tmp->set_attribute("state",buf.str());
        tmp->set_child_text((*it)->first.first);
        (*it)->second.toXML(*tmp);
    }
}

void Tree::fromXML(const string &fileName)
{
    using namespace xmlpp;
    
    DomParser parser(fileName);
    Document *document = parser.get_document();
    Element *root = document->get_root_node();
    if (root->get_name()!="tree")
    {
        throw string("Tree::fromXML : the given XML file doesn't contain a tree");
    }
    fromXML(*root);
}

void Tree::fromXML(const xmlpp::Element &root)
{
    clear();
    using namespace xmlpp;

    Node::NodeList list = root.get_children("item");
    for (Node::NodeList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        Branch *branch = new Branch;
        Element *elem = dynamic_cast<Element*>(*it);
        Attribute *attr = elem->get_attribute("state");
        int state = 0;
        if (attr!=NULL)
        {
            stringstream buf(stringstream::in | stringstream::out);
            buf << attr->get_value();
            buf >> state;
        }
        branch->first = Item(elem->get_child_text()->get_content(),State(state));
        branch->second = Tree(*elem);
        vChildren.push_back(branch);
    }
}

void Tree::clear()
{
    for (vector<Branch*>::const_iterator it=vChildren.begin(); it!=vChildren.end(); it++)
    {
        delete *it;
    }
    vChildren.clear();
}

// new methods

Tree::iterator Tree::begin() const
{
    vector<vector<Branch*>::const_iterator> stack;
    stack.push_back(vChildren.begin());
    return iterator(stack);
}

Tree::iterator Tree::beginUnchecked() const
{
    vector<vector<Branch*>::const_iterator> stack;
    stack.push_back(vChildren.begin());
    iterator it(stack);
    while (it != end() && ((*it).second==sSuccess || (*it).second==sFailure))
    {
        it++;
    }
    it.setType(itUnchecked);
    return it;
}

Tree::iterator Tree::beginState(State state) const
{
    vector<vector<Branch*>::const_iterator> stack;
    stack.push_back(vChildren.begin());
    iterator it(stack,itNormal,state);
    while (it != end() && (*it).second!=state)
    {
        it++;
    }
    it.setType(itState);
    return it;
}

Tree::iterator Tree::end() const
{
    vector<vector<Branch*>::const_iterator> stack;
    stack.push_back(vChildren.end());
    return iterator(stack);
}

Tree::iterator Tree::endUnchecked() const
{
    iterator it=beginUnchecked(),it2=it;
    if (it==end())
    {
        return it;
    }
    it.setType(itNormal);
    while (it!=end())
    {
        if ((*it).second==sNone || (*it).second==sProgress)
        {
            it2 = it;    
        }
        it++;
    }
    return ++it2;
}

Tree::iterator Tree::endState(State state) const
{
    iterator it=beginState(state),it2=it;
    if (it==end())
    {
        return it;
    }
    it.setType(itNormal);
    while (it!=end())
    {
        if ((*it).second==state)
        {
            it2 = it;    
        }
        it++;
    }
    return ++it2;
}

Item Tree::operator[](const string &indices) const
{
    stringstream buf(stringstream::in | stringstream::out);
    int pos = indices.find("_");
    int n;
    if (pos==-1)
    {
        buf << indices;
    }
    else
    {
        buf << indices.substr(0,pos);
    
    }
    buf >> n;
    if (n<0 || (unsigned int) n >= vChildren.size())
    {
        throw string("Tree::operator[] : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (pos==-1)
    {
        return vChildren[n]->first;
    }
    else
    {
        return vChildren[n]->second[sub];
    }
}

Item& Tree::operator[](const string &indices)
{
    stringstream buf(stringstream::in | stringstream::out);
    int pos = indices.find("_");
    int n;
    if (pos==-1)
    {
        buf << indices;
    }
    else
    {
        buf << indices.substr(0,pos);
    
    }
    buf >> n;
    if (n<0 || (unsigned int) n >= vChildren.size())
    {
        throw string("Tree::operator[] : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (pos==-1)
    {
        return vChildren[n]->first;
    }
    else
    {
        return vChildren[n]->second[sub];
    }
}

Branch* Tree::branch(const string &indices)
{
    stringstream buf(stringstream::in | stringstream::out);
    int pos = indices.find("_");
    int n;
    if (pos==-1)
    {
        buf << indices;
    }
    else
    {
        buf << indices.substr(0,pos);
    
    }
    buf >> n;
    if (n<0 || (unsigned int) n >= vChildren.size())
    {
        throw string("Tree::branch : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (pos==-1)
    {
        return vChildren[n];
    }
    else
    {
        return vChildren[n]->second.branch(sub);
    }
}

void Tree::insert(const string &indices, const string &content, State state)
{
    stringstream buf(stringstream::in | stringstream::out);
    int pos = indices.find("_");
    int n;
    if (pos==-1)
    {
        buf << indices;
    }
    else
    {
        buf << indices.substr(0,pos);
    }
    buf >> n;
    if (n<0 || (unsigned int) n > vChildren.size()) // n can be equal to vChildren.size() but only for the last index
    {
        throw string("Tree::insert : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (pos == -1)
    {
        Branch *branch = new Branch;
        branch->first = Item(content,state);
        vChildren.insert(vChildren.begin()+n,branch);
    }
    else
    {
        if ((unsigned int)n==vChildren.size())
        {
            throw string("Tree::insert : Index out of bounds");
        }
        vChildren[n]->second.insert(sub,content,state);
    }
}

void Tree::insert(const string &indices, Branch *branch)
{
    stringstream buf(stringstream::in | stringstream::out);
    int pos = indices.find("_");
    int n;
    if (pos==-1)
    {
        buf << indices;
    }
    else
    {
        buf << indices.substr(0,pos);
    }
    buf >> n;
    if (n<0 || (unsigned int) n > vChildren.size()) // n can be equal to vChildren.size() but only for the last index
    {
        throw string("Tree::insert : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (pos==-1)
    {
        vChildren.insert(vChildren.begin()+n,branch);
    }
    else
    {
        if ((unsigned int)n==vChildren.size())
        {
            throw string("Tree::insert : Index out of bounds");
        }
        vChildren[n]->second.insert(sub,branch);
    }
}

void Tree::add(const string &content, State state)
{
    if (vChildren.size()==0 || vChildren.back()->second.vChildren.size()==0)
    {
        Branch *branch = new Branch;
        branch->first = Item(content,state);
        vChildren.push_back(branch);
    }
    else
    {
        vChildren.back()->second.add(content,state);
    }
}

void Tree::add(int depth, const string &content, State state)
{
    // insert at the given depth (1 is root)
    if (depth<1)
    {
        throw string("Tree::add : depth must be non-negative");
    }
    if (depth==1)
    {
        Branch *branch = new Branch;
        branch->first = Item(content,state);
        vChildren.push_back(branch);
    }
    else
    {
        if (vChildren.size()==0)
        {
            throw string("Tree::add : the given depth is unavailable");
        }
        vChildren.back()->second.add(depth-1,content,state);
    }
}

void Tree::addChild(const string &content, State state)
{
    if (vChildren.size()==0)
    {
        Branch *branch = new Branch;
        branch->first = Item(content,state);
        vChildren.push_back(branch);
    }
    else
    {
        vChildren.back()->second.addChild(content,state);
    }
}

void Tree::remove(const string &indices, bool toDelete)
{
    stringstream buf(stringstream::in | stringstream::out);
    int pos = indices.find("_");
    int n;
    if (pos==-1)
    {
        buf << indices;
    }
    else
    {
        buf << indices.substr(0,pos);
    }
    buf >> n;
    if (n<0 || (unsigned int) n >= vChildren.size())
    {
        throw string("Tree::remove : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (pos==-1)
    {
        Branch *branch = vChildren[n];
        vChildren.erase(vChildren.begin()+n);
        if (toDelete)
        {
            delete branch;
        }
    }
    else
    {
        vChildren[n]->second.remove(sub,toDelete);
    }
}

void Tree::move(const string &currentIndices, const string &newIndices)
{
    stringstream buf(stringstream::in | stringstream::out);
    insert(newIndices,branch(currentIndices));
    // we now determine if there is need to modify newIndices for the suppression
    int pos = currentIndices.rfind("_");
    int pos2 = newIndices.rfind("_");
    string indices(currentIndices);
    if ((pos==-1 && pos2==-1) || currentIndices.substr(0,pos)==newIndices.substr(0,pos2))
    {
        int n,n2;
        buf << currentIndices.substr(pos+1) << " " << newIndices.substr(pos2+1);
        buf >> n >> n2;
        if (n>n2)
        {
            stringstream buf2(stringstream::in | stringstream::out);
            buf2 << currentIndices.substr(0,pos+1) << (n+1);
            indices = buf2.str();
        }
    }
    remove(indices,false);    
}

// iterator's methods

Tree::iterator::iterator(const vector<vector<Branch*>::const_iterator>& its, IterationType type, State state): Model::iterator(type,state), qIts(its)
{
}

bool Tree::iterator::operator!=(const iterator& it) const
{
    if (it.qIts.size()!=qIts.size())
    {
        return true;
    }
    vector<vector<Branch*>::const_iterator>:: const_iterator it2 = it.qIts.begin();
    for (vector<vector<Branch*>::const_iterator>:: const_iterator it1 = qIts.begin(); it1 !=qIts.end(); it1++)
    {
        if (*it1 != *it2)
        {
            return true;
        }
        it2++;
    }
    return false;
}

bool Tree::iterator::operator==(const iterator& it) const
{
    return !operator!=(it);
}

Tree::iterator& Tree::iterator::operator++()
{
    vector<vector<Branch*>::const_iterator>::reverse_iterator it = qIts.rbegin();
    if (it==qIts.rend())
    {
        throw string("Tree::iterator::operator++ : End of the tree reached");
    }
    if ((*(*it))->second.vChildren.size()!=0)   // if there are children, go to the first child
    {
        qIts.push_back((*(*it))->second.vChildren.begin());
    }
    else    // otherwise, go to the next (if there is one)
    {
        (*it)++;
        while (it+1 != qIts.rend() && *it == (*(*(it+1)))->second.vChildren.end())
        {
            qIts.pop_back();
            it++;
            (*it)++;
        }
    }   
    return *this;
}

Tree::iterator Tree::iterator::operator++(int i)
{
    iterator it = *this;
    operator++();
    return it;
}

Item Tree::iterator::operator*()
{
    switch (type())
    {
        case itUnchecked:   while ((*(qIts.back()))->first.second==sSuccess || (*(qIts.back()))->first.second==sFailure)
                                operator++();
                            break; 
        case itState:
            {
                State sState = state();
                while ((*(qIts.back()))->first.second!=sState)
                    operator++();
                break;
            }
        default:            break;
    }
    return (*(qIts.back()))->first;
}

int Tree::iterator::depth() const
{
    return qIts.size();
}