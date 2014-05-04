#include <iostream>
#include <string>
#include <vector>
#include <time.h>

#include "../../classes/date_class.hpp"
#include "../../classes/task_class.hpp"
#include "../display/common.hpp"
#include "../../globals/global_defines.hpp"
#include "tasklist_menu_globals.hpp"

using namespace std;

namespace
{
}

namespace tasklist_menu_globals
{
    
    /* Returns true/false respectively based on whether it was modified. 
     will also return false if the user cancels. */
    bool modify_task(taskList::task_class& task)
    {
        using namespace std;
        using namespace common;
        
        bool modified(false), finished(false);
        vector<int> ch;
        date::date_val tempdate;
        
        do
        {
            cls();
            cout<< endl;
            common::center("Modify Task");
            cout<< endl;
            for(char x = 0; x < 3; x++) cout<< '\n';
            
            cout<< " 1 --------  Name: \""<< task.info.name<< "\"\n";
            cout<< " 2 -  Description: \""<< task.info.description<< "\"\n";
            cout<< " 3 --------  Date: ";
            if(to_dv(task.info.ddate.t) != tempdate)
            {
                tempdate = task.info.ddate.t;
            }
            
            switch(task.info.ddate.on)
            {
                case true:
                {
                    cout<< date::display(tempdate)<< "  ("<< tempdate.wday_name()<< ")\n";
                }
                break;
                
                case false:
                {
                    cout<< "DISABLED";
                }
                break;
                
                default:
                {
                    cout<< "ERROR";
                }
                break;
            }
            cout<< endl;
            cout<< " 4 ----  Toggle due date: "<< (task.info.ddate.on ? "ON" : "OFF")<< endl;
            cout<< " 5 ----  Priority: "<< task.info.priority<< endl;
            
            cout<< " [ENTER] -----  Done"<< endl;
            cout<< " [BACKSPACE] -  Cancel"<< endl;
            
            common::cl();
            ch = common::gkey_funct();
            if(ch.size() > 0)
            {
                switch(IS_CONTROL(char(ch[0]))) //we may want to add control keys in the future (provided for easy maintenance)
                {
                    case false:
                    {
                        switch(is_char(char(ch[0])))
                        {
                            case true:
                            {
                                switch(char(ch[0]))
                                {
                                    case '1':
                                    {
                                        if(user_input_string(task.info.name, ("[END] -  Cancel\n\n\nCurrent \
value: \"" + task.info.name + "\"\n\n\nEnter the name: ")))
                                        {
                                            modified = true;
                                        }
                                        
                                        if(task.info.name.size() > 40)
                                        {
                                            task.info.name.resize(40 - 3);
                                            task.info.name += "...";
                                        }
                                    }
                                    break;
                                    
                                    case '2':
                                    {
                                        if(user_input_string(task.info.description, ("\
[END] -  Cancel\n\n\nCurrent value: \"" + task.info.description + "\"\n\n\nEnter the Description: ")))
                                        {
                                            modified = true;
                                        }
                                        
                                        //I think 2Kb is a reasonable limit...
                                        if(task.info.description.size() > 2000)
                                        {
                                            task.info.description.resize(2000 - 3);
                                            task.info.description += "...";
                                        }
                                    }
                                    break;
                                    
                                    case '3':
                                    {
                                        if(modify_date(tempdate))
                                        {
                                            modified = true;
                                            tempdate.to_tm(task.info.ddate.t);
                                        }
                                    }
                                    break;
                                    
                                    case '4':
                                    {
                                        task.info.ddate.on = !task.info.ddate.on;
                                        modified = true;
                                    }
                                    break;
                                    
                                    case '5':
                                    {
                                        if(user_input_string<short>(task.info.priority, std::string("\
[END] -  Cancel\n\n\nCurrently set to: " + conv<short, std::string>(task.info.priority) + "\n\n\nEnter a priority (1-10): ")))
                                        {
                                            if(task.info.priority < 0)
                                            {
                                                task.info.priority *= (-1);
                                            }
                                            if(task.info.priority > 10) task.info.priority = 10;
                                            modified = true;
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

                            case false:
                            {
                                switch(ch[0])
                                {
                                    case ENTER_KEY:
                                    {
                                        finished = true;
                                    }
                                    break;

                                    case BACKSPACE_KEY:
                                    {
                                        finished = true;
                                        modified = false;
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
        
        return modified;
    }
    
    void display_tasks(const vector<taskList::task_class>& tasks, scrollDisplay::scroll_display_class& display)
    {
        date::date_val d1, d2;
        d2 = get_time();
        for(char x = 0; x < signed(display.window().size()); x++)
        {
            switch(tasks.at(display.window_beg() + x).info.ddate.on)
            {
                case true:
                {
                    d1 = tasks.at(display.window_beg() + x).info.ddate.t;
                    if(d1 == d2) cout<< " *";
                    if(d1 < d2) cout<< ">>";
                    if(d2 < d1) cout<< "  ";
                }
                break;
                
                case false:
                {
                    cout<< "  ";
                }
                break;
                
                default:
                {
                    cout<< "  ";
                }
                break;
            }
            switch(x == display.gpos().part)
            {
                case true:
                {
                    cout<< "["<< display.window()[x]<< "]";
                }
                break;

                case false:
                {
                    cout<< " "<< display.window()[x];
                }
                break;

                default:
                {
                }
                break;
            }
            cout<< '\n';
        }
    }
    
    void create_task_display(
            vector<taskList::task_class>& tasks,
            vector<string>& disp_list, 
            const tasklist_menu_sorting::sort_method& sort)
    {
        disp_list.erase(disp_list.begin(), disp_list.end());
        sort_tasklist(tasks, sort);
        if(tasks.size() > 0)
        {
            for(vector<taskList::task_class>::const_iterator it = tasks.begin(); it != tasks.end(); ++it)
            {
                disp_list.push_back(it->info.name);
            }
        }
    }
    
    void create_task_display(vector<taskList::task_class>& tasks, vector<string>& disp_list)
    {
        create_task_display(tasks, disp_list, tasklist_menu_sorting::none);
    }
    
    /* Retrieves the current time as a struct tm. */
    struct tm get_time()
    {
        struct tm *temptm;
        time_t temp_time;
        
        (void)time(&temp_time);
        temptm = localtime(&temp_time);
        
        return *temptm;
    }
    
    /* Makes it easier to use date_vals instead of struct tm. */
    date::date_val to_dv(const struct tm& t)
    {
        date::date_val d;
        d = t;
        return d;
    }
    
    /* Modifies a date according to a specific operation on a specific
     piece of the date.  Used in the date modification menu. */
    inline void mod_date(date::date_val& d, const char& targ, const int& op)
    {
        int mon(0), day(0), year(0);
        
        /* Here is how it's done:
         targ == 0: we perform an operation on the month
         targ == 1:  we perform an operation on the day
         targ == 2: we perform an operation on the year
         
         In this function, we attempt to perform these operation while 
         preserving the rest of the date.  Ex: we If we modify the
         month, we want to try and not change the month's day.*/
        switch((unsigned int)targ)
        {
            case 0:
            {
                //op > 0, add 1 month, otherwise subract 1 month
                switch(op > 0)
                {
                    case true:
                    {
                        day = d.mday;
                        mon = ((d.month + 1) % 12);
                        while(d.month != mon) d++;
                        while(((d + 1).month == mon) && (d.mday != day)) d++;
                    }
                    break;
                    
                    case false:
                    {
                        switch(d.month > 0)
                        {
                            case true:
                            {
                                mon = (d.month - 1);
                                
                            }
                            break;
                            
                            case false:
                            {
                                mon = 11;
                            }
                            break;
                            
                            default:
                            {
                            }
                            break;
                        }
                        day = d.mday;
                        while(d.month != mon) d--;
                        while((d.mday != day) && ((d - 1).month == mon)) d--;
                    }
                    break;
                    
                    default:
                    {
                    }
                    break;
                }
            }
            break;
            
            case 1:
            {
                //op > 0, add 1 day, otherwise subract 1 day
                switch(op > 0)
                {
                    case true:
                    {
                        d++;
                    }
                    break;
                    
                    case false:
                    {
                        d--;
                    }
                    break;
                    
                    default:
                    {
                    }
                    break;
                }
            }
            break;
            
            case 2:
            {
                if((d.year > 0) && (d.year < 10000)) //limit the max/min year value.
                {
                    day = d.mday;
                    mon = d.month;
                    year = d.year;

                    //op > 0, add 1 year, otherwise subract 1 year
                    switch(op > 0)
                    {
                        case true:
                        {
                            d += (YDAYS(year) - d.yday);
                            while(d.month != 0) d++;
                        }
                        break;

                        case false:
                        {
                            d -= (YDAYS(year) + 1);
                            while((d.month > 0) || (d.mday > 0)) d--;
                        }
                        break;

                        default:
                        {
                        }
                        break;
                    }

                    //the result should be +/- 1 year, but we should be at month 0, mday 0.
                    while(d.month < mon) d++;
                    while((d.mday < day) && ((d + 1).month == mon)) d++;
                }
            }
            break;
            
            default:
            {
            }
            break;
        }
    }
    
    /* Allows the user to modify the date. */
    inline bool modify_date(date::date_val& d)
    {
        using namespace std;
        
        bool modified(false), finished(false);
        vector<int> ch;
        string disp;
        char targ(0);
        
        do
        {
            common::cls();
            for(char x = 0; x < VCENTER; x++) cout<< endl;
            
            disp.erase();
            
            //create string display for user:
            if(targ == 0) disp += "[";
            disp += d.month_name();
            if(targ == 0) disp += "]";
            disp += " ";
            
            if(targ == 1) disp += "[";
            if((d.mday + 1) < 10) disp += "0";
            disp += conv<unsigned short, std::string>((d.mday + 1));
            if(targ == 1) disp += "]";
            disp += ", ";
            
            if(targ == 2) disp += "[";
            disp += conv<unsigned int, std::string>(d.year);
            if(targ == 2) disp += "]";
            disp += ("   (" + d.wday_name() + ")");
            
            
            common::center(disp);
            cout<< endl;
            for(char x = 0; x < 3; x++) cout<< endl;
            if(d != to_dv(get_time())) cout<< " T -  Goto today"<< endl;
            cout<< "[ENTER] -  Done"<< endl;
            cout<< "[BACKSPACE] -  Cancel"<< endl;
            
            ch = common::gkey_funct();
            if(ch.size() > 0)
            {
                switch(IS_CONTROL(ch[0]))
                {
                    case true:
                    {
                        if(ch == UP_KEY)
                        {
                            mod_date(d, targ, 1);
                            modified = true;
                        }
                        if(ch == DOWN_KEY)
                        {
                            mod_date(d, targ, -1);
                            modified = true;
                        }
                        if(ch == LEFT_KEY)
                        {
                            if(targ > 0) targ--;
                        }
                        if(ch == RIGHT_KEY)
                        {
                            if(targ < 2) targ++;
                        }
                    }
                    break;
                    
                    case false:
                    {
                        switch(common::is_char(ch[0]))
                        {
                            case false:
                            {
                                switch(ch[0])
                                {
                                    case ENTER_KEY:
                                    {
                                        finished = true;
                                    }
                                    break;

                                    case BACKSPACE_KEY:
                                    {
                                        modified = false;
                                        finished = true;
                                    }
                                    break;
                                    
                                    default:
                                    {
                                    }
                                    break;
                                }
                            }
                            break;
                            
                            case true:
                            {
                                switch(tolower(char(ch[0])))
                                {
                                    case 't': case 'T':
                                    {
                                        if(to_dv(get_time()) != d)
                                        {
                                            d = to_dv(get_time());
                                            if(!modified) modified = true;
                                        }
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
                    break;
                    
                    default:
                    {
                    }
                    break;
                }
            }
            
        }while(!finished);
        
        return modified;
    }
    
    
}
