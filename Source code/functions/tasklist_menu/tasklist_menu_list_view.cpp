#include <iostream>
#include <string>
#include <vector>

#include "../display/common.hpp"
#include "../../classes/scroll_display.hpp"
#include "../../classes/task_class.hpp"
#include "../../classes/date_class.hpp"
#include "../../globals/global_defines.hpp"
#include "tasklist_menu_globals.hpp"
#include "tasklist_menu_list_view.hpp"
#include "tasklist_menu_sorting.hpp"
#include "../filesystem/filesystem.hpp"

using namespace std;

namespace
{
}

namespace tasklist_menu_list_view
{
    
    /* Displays a modify-able list of tasks */
    tasklist_menu_globals::menu_return_data task_list_menu(std::vector<taskList::task_class>& tasks)
    {
        using namespace common;
        
        //declarations:
        vector<int> ch;
        bool finished(false);
        scrollDisplay::scroll_display_class display;
        vector<string> display_list;
        tasklist_menu_sorting::sort_method sort(tasklist_menu_sorting::date);
        tasklist_menu_globals::menu_return_data ret;
        string sort_names[(int(METHOD_MAX) + 1)] = {
            "none",
            "date",
            "name",
            "priority"
        };
        
        ret.modified_data = false;
        ret.command = tasklist_menu_globals::list_view;
        
        //initialization work (display setup, etc...):
        display = scrollDisplay::scroll_display_class(display_list);
        tasklist_menu_globals::create_task_display(tasks, display_list, sort);
        
        /*window size can be set to anything.  Feel free to change it 
         based on your preference! :)  */
        display.window_size() = 10;
        
        //main loop:
        do
        {
            cls();
            cout.flush();
            cout<< '\n';
            center("Task List: ");
            cout<< '\n';
            for(char x = 0; x < 2; x++) cout<< '\n';
            
            cout<< std::string(50, 'V')<< '\n';
            tasklist_menu_globals::display_tasks(tasks, display);
            if(signed(display.window().size()) != display.window_size())
            {
                cout<< std::string((display.window_size() - display.window().size()), '\n');
            }
            cout<< std::string(50, '^')<< '\n';
            
            cout<< '\n';
            for(char x = 0; x < 1; x++) cout<< '\n';
            
            if(display.window().size() > 0)
            {
                cout<< "[ENTER] -  Modify";
                cout<< "  [DELETE] -  Delete  ";
                cout<< "[HOME] -  First task  ";
                cout<< "[END] -  Last task  ";
            }
            cout<< string(2, '\n');
            cout<< " s -  Toggle Sort: "<< sort_names[sort]<< '\n';
            cout<< " a -  Add new task\n";
            cout<< " d -  Date view\n";
            cout<< " e -  EXIT";
            cout.flush();
            
            ch = common::gkey_funct();
            if(ch.size() > 0)
            {
                switch(IS_CONTROL(ch[0]))
                {
                    case true:
                    {
                        if(tasks.size() > 0)
                        {
                            if(ch == UP_KEY)
                            {
                                display.mv_up();
                            }
                            if(ch == DOWN_KEY)
                            {
                                display.mv_down();
                            }
                            if((ch == LEFT_KEY) || (ch == PGUP_KEY))
                            {
                                display.pg_up();
                            }
                            if((ch == RIGHT_KEY) || (ch == PGDOWN_KEY))
                            {
                                display.pg_down();
                            }
                            if(ch == HOME_KEY)
                            {
                                while(display.pg_up());
                            }
                            if(ch == END_KEY)
                            {
                                while(display.pg_down());
                            }
                            if(ch == DELETE_KEY)
                            {
                                if(common::inp::is_sure(std::string("Are you sure\
 you want to delete \"" + tasks.at(display.gpos().whole).info.name + "\"?")))
                                {
                                    tasks.erase(tasks.begin() + display.gpos().whole);
                                    tasklist_menu_globals::create_task_display(tasks, display_list, sort);
                                    if(!ret.modified_data)
                                    {
                                        ret.modified_data = true;
                                    }
                                }
                            }
                        }
                        
                    }
                    break;
                    
                    case false:
                    {
                        switch(common::is_char(char(ch[0])))
                        {
                            case true:
                            {
                                switch(tolower(char(ch[0])))
                                {
                                    case 'a':
                                    {
                                        taskList::task_class temptask;
                                        temptask.info.ddate.t = tasklist_menu_globals::get_time();
                                        if(tasklist_menu_globals::modify_task(temptask))
                                        {
                                            tasks.push_back(temptask);
                                            if(!ret.modified_data) ret.modified_data = true;
                                        }
                                        tasklist_menu_globals::create_task_display(tasks, display_list, sort);
                                    }
                                    break;
                                    
                                    case 's':
                                    {
                                        {
                                            tasklist_menu_sorting::toggle_sort_method(sort);
                                            tasklist_menu_globals::create_task_display(tasks, display_list, sort);
                                        }
                                    }
                                    break;
                                    
                                    case 'e':
                                    {
                                        finished = true;
                                        ret.command = tasklist_menu_globals::quit;
                                    }
                                    break;
                                    
                                    case 'd':
                                    {
                                        finished = true;
                                        ret.command = tasklist_menu_globals::date_view;
                                    }
                                    break;
                                    
                                    default:
                                    {
                                    }
                                    break;
                                }
                            }
                            break;
                            
                            case false:
                            {
                                switch(ch[0])
                                {
                                    case ENTER_KEY:
                                    {
                                        if(tasks.size() > 0)
                                        {
                                            taskList::task_class temptask(tasks.at(display.gpos().whole));
                                            if(tasklist_menu_globals::modify_task(temptask))
                                            {
                                                tasks.at(display.gpos().whole) = temptask;
                                                tasklist_menu_globals::create_task_display(tasks, display_list, sort);
                                                if(!ret.modified_data)
                                                {
                                                    ret.modified_data = true;
                                                }
                                            }
                                        }
                                    }
                                    break;
                                    
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
                    break;
                    
                    default:
                    {
                    }
                    break;
                }
            }
        }while(!finished);
        return ret;
    }
    
    
}
