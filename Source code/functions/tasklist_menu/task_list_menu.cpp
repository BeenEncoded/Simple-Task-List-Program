#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>

#include "task_list_menu.hpp"
#include "../display/common.hpp"
#include "../../classes/task_class.hpp"
#include "../filesystem/filesystem.hpp"
#include "../tasklist_menu/tasklist_menu_globals.hpp"
#include "../tasklist_menu/tasklist_menu_list_view.hpp"
#include "../tasklist_menu/tasklist_menu_date_view.hpp"

using namespace std;

namespace
{
    template<class type1, class type2>
    inline type2 conv(const type1& t1)
    {
        stringstream ss;
        type2 t2;
        ss<< t1;
        ss>> t2;
        return t2;
    }
    
    
}

namespace task_iostream
{
    bool save_tasklist(const std::vector<taskList::task_class>&, const std::string&);
    bool load_tasklist(std::vector<taskList::task_class>&, const std::string&);
    
    
    inline bool save_tasklist(std::vector<taskList::task_class>& tasks, const std::string& path)
    {
        bool success(false), give_up(false);
        switch(fsys::path_chars_valid(fsys::fname(path)))
        {
            case true:
            {
                if(!fsys::is_folder(fsys::parent_path(path)))
                {
                    give_up = (!(fsys::create_folders(fsys::parent_path(path))));
                }
                if(!give_up)
                {
                    std::ofstream out;
                    out.open(path.c_str(), ios::out);
                    {
                        using namespace taskList;
                        out<< tasks;
                    }
                    out.close();
                }
                success = fsys::is_file(path);
                if(!success)
                {
                    common::cls();
                    for(char x = 0; x < VCENTER; x++) cout<< '\n';
                    cout<< "Error: stream could not write to file: \""<< path<< "\"\n";
                    if(give_up)
                    {
                        cout<< "Operation was aborted due to an error\n\n\n";
                    }
                    common::wait();
                    common::cls();
                }
            }
            break;
            
            case false:
            {
                common::cls();
                for(char x = 0; x < VCENTER; x++) cout<< '\n';
                cout<< "ERROR: inline bool save_tasklist(std::vector<taskList::task_class>& tasks, const std::string& path)\n\n\
invalid pathname \""<< fsys::fname(path)<< "\"\n";
                common::wait();
                common::cls();
            }
            break;
            
            default:
            {
            }
            break;
            
        }
        return success;
    }
    
    inline bool load_tasklist(std::vector<taskList::task_class>& tasks, const std::string& path)
    {
        bool success(false);
        tasks.erase(tasks.begin(), tasks.end());
        if(fsys::is_file(path) && fsys::is_valid_path(path))
        {
            ifstream in;
            in.open(path.c_str(), ios::in);
            in>> tasks;
            success = (!in.fail());
            in.close();
        }
        tasks.shrink_to_fit();
        return success;
    }
    
    
}

/* Here is how the menu works:
 The base function will call a menu based on what the user sets it
 to.  If data is modified, it will also save the tasklist upon exit.
 A menu must return a menu_return_type specifying what kind of menu 
 to display, or command to execute.*/
namespace taskListMenu
{
    bool call_menu(tasklist_menu_globals::menu_return_data&, vector<taskList::task_class>&);
    bool is_menu_command(const tasklist_menu_globals::menu_command_type&);
    
    /* Returns true/false if the menu command is a command 
     that will apply a menu (!err and !quit) */
    inline bool is_menu_command(const tasklist_menu_globals::menu_command_type& command)
    {
        return ((command != tasklist_menu_globals::err) && (command != tasklist_menu_globals::quit));
    }
    
    /* Return true/false if command doesn't become an error */
    inline bool call_menu(tasklist_menu_globals::menu_return_data& ret, vector<taskList::task_class>& tasks)
    {
        using namespace tasklist_menu_list_view;
        using namespace tasklist_menu_date_view;
        
        bool success(false);
        while(is_menu_command(ret.command))
        {
            switch(ret.command)
            {
                case tasklist_menu_globals::list_view:
                {
                    ret = task_list_menu(tasks);
                }
                break;
                
                case tasklist_menu_globals::date_view:
                {
                    ret = task_list_menu_by_date(tasks);
                }
                break;
                
                default:
                {
                }
                break;
            }
            success = (ret.command != tasklist_menu_globals::err);
        }
        return success;
    }
    
    void task_management_menu()
    {
        vector<taskList::task_class> tasks;
        bool loaded(task_iostream::load_tasklist(tasks, fsys::tasklist_file()));
        tasklist_menu_globals::menu_return_data state;
        
        switch(call_menu(state, tasks))
        {
            //no error
            case true:
            {
                if(loaded || (!loaded && state.modified_data))
                {
                    while(!task_iostream::save_tasklist(tasks, fsys::tasklist_file()) &&
                            common::inp::is_sure("ERROR: could not save, do you want to make another attempt?"));
                }
            }
            break;
            
            //error
            case false:
            {
            }
            break;
            
            default:
            {
            }
            break;
        }
    }
    
    
}

