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

#include <cstring>

#include <iostream>
#include <string>
#include <vector>

#include <gcc-plugin.h>

#include <input.h>
#include <c-family/c-pragma.h>

#include "IncTree.hpp"

static void includeCallback(void *includeName, void *userData);
static void finishCallback(void *gccData, void *userData);

extern "C"
{
    /**
     * @brief Confirms GCC that we comply to GPL.
     */
    int plugin_is_GPL_compatible = 1;
}

/**
 * @brief Name of the plugin.
 */
static const char *const PLUGIN_NAME = "inc-tree";

/**
 * @brief Generic information about the plugin.
 */
static plugin_info helpInfo = {
    .version = "0.1",
    .help = "Prints tree of includes",
};

/**
 * @brief Plugin version requirements.
 */
static plugin_gcc_version pluginVer = {
    .basever = "4.9",
};

/**
 * @brief Object that builds the tree.
 */
static IncTree incTree;

/**
 * @brief Plugin entry point.
 *
 * @param info Argument information.
 * @param ver Version info of GCC.
 *
 * @returns @c 0 on success or error code on failure.
 */
int
plugin_init(struct plugin_name_args *info, struct plugin_gcc_version *ver)
{
    // Validate the version information without plugin_default_version_check().
    if (std::strncmp(ver->basever, pluginVer.basever,
                     std::strlen(pluginVer.basever)) < 0) {
        // Incorrect version of GCC.
        return -1;
    }

    // Provide plugin information for --help and --version.
    register_callback(PLUGIN_NAME, PLUGIN_INFO, nullptr, &helpInfo);

    // Setup callback for each #include (and #line) directive.
    register_callback(PLUGIN_NAME, PLUGIN_INCLUDE_FILE, &includeCallback,
                      nullptr);

    // Setup callback that will print out results.
    register_callback(PLUGIN_NAME, PLUGIN_FINISH, &finishCallback, &helpInfo);

    return 0;
}

/**
 * @brief Callback which is called for @c #include and @c #line directives.
 *
 * @param includeName Name of include.
 * @param userData User-provided data.
 */
static void
includeCallback(void */*includeName*/, void */*userData*/)
{
    cpp_buffer *curr = cpp_get_buffer(parse_in);

    // Register root source files as no other files include them, but we want
    // to have them in the output even in the absence on any includes.
    if (cpp_get_prev(curr) == nullptr) {
        incTree.addUnit(cpp_get_path(cpp_get_file(curr)));
        return;
    }

    std::vector<std::string> includeTrail;

    do {
        includeTrail.push_back(cpp_get_path(cpp_get_file(curr)));
        curr = cpp_get_prev(curr);
    } while (curr != nullptr);

    // Not using std::reverse() as there are poisoned symbols.
    incTree.addIncludeTrail({ includeTrail.rbegin(), includeTrail.rend() });
}

/**
 * @brief Callback which is called before GCC quits.
 *
 * @param gccData Unused GCC-provided data.
 * @param userData User-provided data.
 */
static void
finishCallback(void */*gccData*/, void */*userData*/)
{
    std::cout << incTree;
}
