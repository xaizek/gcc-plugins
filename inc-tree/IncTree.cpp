// Copyright (C) 2015 xaizek <xaizek@openmailbox.org>
//
// This plugin is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The plugin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the plugin.  If not, see <http://www.gnu.org/licenses/>.

#include "IncTree.hpp"

#include <algorithm>
#include <memory>
#include <ostream>
#include <string>

#include <iostream>

#include "tree.hpp"

class IncTree::pimpl
{
public:
    /**
     * @copydoc IncTree::addUnit
     */
    void addUnit(const std::string &path);

    /**
     * @copydoc IncTree::addIncludeTrail
     */
    void addIncludeTrail(const std::vector<std::string> &trail);

    /**
     * @brief Prints all includes as forest with units being roots.
     *
     * @param os Stream to output data.
     */
    void print(std::ostream &os) const;

private:
    /**
     * @brief Type of tree we're using.
     */
    using tree = treexx::tree<std::string>;

    /**
     * @brief Type of sibling iterator for the tree.
     */
    using sibIt = tree::sibling_iterator;

private:
    /**
     * @brief Actual tree/forest of includes.
     */
    tree tr;
};

IncTree::IncTree()
{
    impl.reset(new pimpl());
}

IncTree::~IncTree()
{
    // Do nothing.
}

void
IncTree::addUnit(const std::string &path)
{
    impl->addUnit(path);
}

void
IncTree::addIncludeTrail(const std::vector<std::string> &trail)
{
    impl->addIncludeTrail(trail);
}

void
IncTree::pimpl::addUnit(const std::string &path)
{
    sibIt begin = tr.begin();
    sibIt end = tr.end();

    auto sib = std::find(begin, end, path);
    if (sib == end) {
        tr.insert(tr.begin(), path);
    }
}

void
IncTree::pimpl::addIncludeTrail(const std::vector<std::string> &trail)
{
    sibIt begin = tr.begin();
    sibIt end = tr.end();
    for (const std::string &entry : trail) {
        auto sib = std::find(begin, end, entry);
        if (sib == end) {
            sibIt it = tr.insert(end, entry);
            begin = tr.begin(it);
            end = tr.end(it);
        } else {
            begin = tr.begin(sib);
            end = tr.end(sib);
        }
    }
}

void
IncTree::pimpl::print(std::ostream &os) const
{
    tree::iterator it = tr.begin();
    tree::iterator end = tr.end();
    while (it != end) {
        for (int i = 0; i < tr.depth(it); ++i) {
            os << "    ";
        }
        os << *it << '\n';
        ++it;
    }
}

std::ostream &
operator<<(std::ostream &os, const IncTree &incTree)
{
    incTree.impl->print(os);
    return os;
}
