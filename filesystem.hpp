#ifndef FILESYSTEM_HPP_INCLUDED
#define FILESYSTEM_HPP_INCLUDED
#include <string>

namespace fsys
{
    bool is_file(const std::string&);
    bool is_folder(const std::string&);
    bool is_valid_path(const std::string&);
    bool path_chars_valid(const std::string&);
    std::string fname(const std::string&);
    std::string current_path();
    std::string parent_path(const std::string&);
}

#endif