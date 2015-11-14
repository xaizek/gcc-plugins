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

#ifndef INCLUDE_TREE__INCTREE_HPP__
#define INCLUDE_TREE__INCTREE_HPP__

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Class for collecting and printing information on includes.
 */
class IncTree
{
    friend std::ostream & operator<<(std::ostream &os, const IncTree &incTree);

public:
    /**
     * @brief Initializes the instance as empty.
     */
    IncTree();
    /**
     * @brief Needed for correct destruction of impl.
     */
    ~IncTree();

public:
    /**
     * @brief Adds single unit as a root entry.
     *
     * There can be more than one root entry, so this method can be called
     * multiple times for each compilation unit.
     *
     * @param path Path to compilation unit.
     */
    void addUnit(const std::string &path);

    /**
     * @brief Adds a trail, intermediate nodes are created automatically.
     *
     * @param trail Trail of include directives.
     */
    void addIncludeTrail(const std::vector<std::string> &trail);

private:
    class pimpl;

    /**
     * @brief Hidden implementation as per PIMP idiom.
     */
    std::unique_ptr<pimpl> impl;
};

/**
 * @brief Prints out IncTree instance into the stream.
 *
 * @param os Stream to use for output.
 * @param incTree Object to print out.
 *
 * @returns @p os
 */
std::ostream & operator<<(std::ostream &os, const IncTree &incTree);

#endif // INCLUDE_TREE__INCTREE_HPP__
