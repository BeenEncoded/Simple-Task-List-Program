#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include "common.hpp"
#include "scroll_display.hpp"
#include "task_class.hpp"
#include "date_class.hpp"
#include "global_defines.hpp"
#include "tasklist_menu_globals.hpp"
#include "tasklist_menu_list_view.hpp"
#include "filesystem.hpp"

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
        using namespace tasklist_menu_globals;
        using namespace tasklist_menu_sorting;
        
        //declarations:
        vector<int> ch;
        bool finished(false);
        scrollDisplay::scroll_display_class display;
        vector<string> display_list;
        sort_method sort(tasklist_menu_sorting::date);
        menu_return_data ret;
        string sort_names[(int(METHOD_MAX) + 1)] = {
            "none",
            "date",
            "name",
            "priority"
        };
        
        ret.modified_data = false;
        ret.command = list_view;
        
        //initialization work (display setup, etc...):
        display = scrollDisplay::scroll_display_class(display_list);
        create_task_display(tasks, display_list, sort);
        
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
            display_tasks(tasks, display);
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
                                    create_task_display(tasks, display_list, sort);
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
                                        temptask.info.ddate.t = get_time();
                                        if(modify_task(temptask))
                                        {
                                            tasks.push_back(temptask);
                                            if(!ret.modified_data) ret.modified_data = true;
                                        }
                                        create_task_display(tasks, display_list, sort);
                                    }
                                    break;
                                    
                                    case 's':
                                    {
                                        {
                                            toggle_sort_method(sort);
                                            create_task_display(tasks, display_list, sort);
                                        }
                                    }
                                    break;
                                    
                                    case 'e':
                                    {
                                        finished = true;
                                        ret.command = quit;
                                    }
                                    break;
                                    
                                    case 'd':
                                    {
                                        finished = true;
                                        ret.command = date_view;
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
                                            if(modify_task(temptask))
                                            {
                                                tasks.at(display.gpos().whole) = temptask;
                                                create_task_display(tasks, display_list, sort);
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