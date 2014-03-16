#include <iostream>
#include <boost/filesystem.hpp>
#include <string>
#include <exception>

#include "filesystem.hpp"

namespace fsys
{
    bool is_file(const std::string& path)
    {
        return boost::filesystem::is_regular_file(boost::filesystem::path(path));
    }
    
    bool is_folder(const std::string& path)
    {
        return boost::filesystem::is_directory(boost::filesystem::path(path));
    }
    
    bool is_valid_path(const std::string& p)
    {
        using namespace boost::filesystem;
        return (is_regular_file(path(p)) || (is_directory(path(p)) && !is_symlink(path(p))));
    }
    
    
}