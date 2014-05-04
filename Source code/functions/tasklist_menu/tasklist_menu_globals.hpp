#ifndef TASKLIST_MENU_GLOBALS_HPP_INCLUDED
#define TASKLIST_MENU_GLOBALS_HPP_INCLUDED
#include <vector>
#include <string>
#include <time.h>
#include <sstream>
#include <type_traits>

#include "../../classes/task_class.hpp"
#include "tasklist_menu_sorting.hpp"
#include "../../classes/scroll_display.hpp"
#include "../display/common.hpp"

namespace tasklist_menu_globals
{
    struct menu_return_data;
    
    /* Returns values that menus can return to the caller:  */
    enum menu_command_type{
        err = 0,
        quit = 1,
        list_view = 2,
        date_view = 3
    };
    
    /* Information that a menu can return to the caller.  Such
     information will be used to make decisions like whether 
     to save or whether to switch menu types. */
    struct menu_return_data{
        
        menu_return_data operator=(const menu_return_data& mrt)
        {
            if(this != &mrt)
            {
                this->modified_data = mrt.modified_data;
                this->command = mrt.command;
            }
            return *this;
        }
        
        bool modified_data = false;
        menu_command_type command = list_view;
    };
    
    
    template<class type1, class type2>
    inline type2 conv(const type1& t1)
    {
        std::stringstream ss;
        type2 t2;
        ss<< t1;
        ss>> t2;
        return t2;
    }
    
    
    /* Asks for user input and only modifies the t1 variable if
     the user doesn't cancel. Returns true/false based on whether or
     not the user modified the variable.  Returns false if the
     user canceled.*/
    template<class type1>
    inline bool user_input_string(type1& t1, const std::string& message)
    {
        using namespace std;
        
        string temps;
        bool modified(false);
        temps = common::inp::get_user_string(message);
        if(temps != GSTRING_CANCEL)
        {
            switch(is_fundamental<type1>::value)
            {
                case true:
                {
                    /* This function isn't qualified to handle pointers... */
                    //assert(!(is_pointer<type1>::value));
                    t1 = conv<std::string, type1>(temps);
                    modified = true;
                }
                break;
                
                case false:
                {
                    //make sure it's a string
                    switch(sizeof(type1) == sizeof(std::string))
                    {
                        case true:
                        {
                            /* @CHECK : This is dangerous, but necessary if this function is
                             to work as intended.  I could not come up with anything 
                             better...  This function does make the menus so
                             much cleaner, and it would be a shame to remove the use
                             of this function because of the reinterpret_cast<> alone.*/
                            *(reinterpret_cast<string*>(&t1)) = temps;
                            modified = true;
                        }
                        break;
                        
                        case false:
                        {
                        }
                        
                        default:
                        {
                        }
                        break;
                    }
                }
                break;
                
                default:
                {
                }
                break;
            }
        }
        temps.erase();
        return modified;
    }
    
    
    bool modify_task(taskList::task_class&);
    void display_tasks(const std::vector<taskList::task_class>&, scrollDisplay::scroll_display_class&);
    void create_task_display(std::vector<taskList::task_class>&, std::vector<std::string>&, const tasklist_menu_sorting::sort_method&);
    void create_task_display(std::vector<taskList::task_class>&, std::vector<std::string>&);
    struct tm get_time();
    date::date_val to_dv(const struct tm&);
    bool modify_date(date::date_val&);
    void mod_date(date::date_val&, const char&, const int&);
    void create_task_display(const std::vector<taskList::task_class>&, std::vector<std::string>&);
}

#endif
