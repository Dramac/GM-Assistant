#include "Branch.h"
#include "ItemFactory.h"

// constructors

Branch::Branch(Item* item, Branch *parent): pItem(item), tTree(this), pParent(parent)
{
}

Branch::Branch(Item* item, const Tree &tree, Branch* parent): pItem(item), tTree(tree,this), pParent(parent)
{
}

Branch::Branch(Item* item, const xmlpp::Element &root, Branch* parent): pItem(item), tTree(root,this), pParent(parent)
{
}

Branch::Branch(const Branch &branch, Branch* parent): pParent(parent)
{
    *this = branch;
}

// destructor

Branch::~Branch()
{
    delete pItem;
}

// methods

Item* Branch::item() const
{
    return pItem;
}

Tree& Branch::tree()
{
    return tTree;
}

Branch* Branch::parent() const
{
    return pParent;
}

// operators

Branch& Branch::operator=(const Branch &branch)
{
    pItem = ItemFactory::copyItem(branch.item());
    tTree = branch.tTree;

    return *this;
}