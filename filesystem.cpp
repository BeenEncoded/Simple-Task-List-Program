#include <iostream>
#include <boost/filesystem.hpp>
#include <string>
#include <exception>

#include "filesystem.hpp"
#include "global_defines.hpp"

namespace
{
}

namespace fsys
{
    bool is_file(const std::string& path)
    {
        boost::system::error_code err;
        return boost::filesystem::is_regular_file(boost::filesystem::path(path), err);
    }
    
    bool is_folder(const std::string& path)
    {
        boost::system::error_code err;
        return boost::filesystem::is_directory(boost::filesystem::path(path), err);
    }
    
    bool is_valid_path(const std::string& p)
    {
        using namespace boost::filesystem;
        boost::system::error_code err;
        return ((is_regular_file(path(p), err) || (is_directory(path(p), err) && !is_symlink(path(p), err))) && 
                path_chars_valid(path(p).filename().string()));
    }
    
    bool path_chars_valid(const std::string& s)
    {
        bool valid(false);
        int count(0);
        std::string badchars(BAD_DIRECTORY_NAME_CHARS);
        
        badchars.shrink_to_fit();
        for(std::string::const_iterator it = badchars.begin(); ((it != badchars.end()) && (count == 0)); ++it)
        {
            if(s.find(*it) != std::string::npos)
            {
                count++;
            }
        }
        valid = (count == 0);
        
        badchars.erase();
        return valid;
    }
    
    std::string fname(const std::string& s)
    {
        return boost::filesystem::path(s).filename().string();
    }
    
    std::string current_path()
    {
        return boost::filesystem::current_path().string();
    }
    
    std::string parent_path(const std::string& p)
    {
        boost::filesystem::path tempp(p);
        std::string temps("");
        switch(tempp.has_parent_path())
        {
            case true:
            {
                temps = tempp.parent_path().string();
            }
            break;
            
            case false:
            {
            }
            
            default:
            {
                temps = p;
            }
            break;
        }
        temps.shrink_to_fit();
        return temps;
    }
    
    /* Returns true if no errors occured, and the folder was created. */
    bool create_folders(const std::string& s)
    {
        boost::filesystem::path p(s);
        boost::system::error_code err;
        if(!is_valid_path(s))
        {
            boost::filesystem::create_directories(p, err);
        }
        return ((!err) && is_folder(s));
    }
    
    
}