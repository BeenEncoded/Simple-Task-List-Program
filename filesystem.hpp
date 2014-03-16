#ifndef FILESYSTEM_HPP_INCLUDED
#define FILESYSTEM_HPP_INCLUDED

namespace fsys
{
    bool is_file(const std::string&);
    bool is_folder(const std::string&);
    bool is_valid_path(const std::string&);
}

#endif