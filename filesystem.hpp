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
    bool create_folders(const std::string&);
    
    /* Used to specify the save folder for the task list. */
    inline std::string tasklist_folder()
    {
        return std::string(fsys::current_path() + "/Task list program");
    }
    
    /* used to specify the save file for the task list */
    inline std::string tasklist_file()
    {
        return std::string((tasklist_folder() + "/Your Tasks.tl"));
    }
    
    
}

#endif