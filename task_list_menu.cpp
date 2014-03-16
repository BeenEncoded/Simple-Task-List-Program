#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <vector>
#include <type_traits>
#include <functional>

#include "task_list_menu.hpp"
#include "common.hpp"
#include "global_defines.hpp"
#include "date_class.hpp"
#include "task_class.hpp"
#include "scroll_display.hpp"
#include "filesystem.hpp"

#include <assert.h>

using namespace std;

namespace
{
    struct tm get_time();
    date::date_val to_dv(const struct tm&);
    bool modify_date(date::date_val&);
    void mod_date(date::date_val&, const char&, const int&);
    void create_task_display(const vector<taskList::task_class>&, vector<string>&);
    
    
    template<class type1, class type2>
    inline type2 conv(const type1& t1)
    {
        stringstream ss;
        type2 t2;
        ss<< t1;
        ss>> t2;
        return t2;
    }
    
    /* Retrieves the current time as a struct tm. */
    inline struct tm get_time()
    {
        struct tm *temptm;
        time_t temp_time;
        
        (void)time(&temp_time);
        temptm = localtime(&temp_time);
        
        return *temptm;
    }
    
    /* Makes it easier to use date_vals instead of struct tm. */
    inline date::date_val to_dv(const struct tm& t)
    {
        date::date_val d;
        d = t;
        return d;
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
                    assert(!(is_pointer<type1>::value));
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
                            
                            case 't': case 'T':
                            {
                                if(to_dv(get_time()) != d)
                                {
                                    d = to_dv(get_time());
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
            
        }while(!finished);
        
        return modified;
    }
    
    
}

namespace taskDisplay
{
#define METHOD_MAX 3
    enum sort_method{
        none = 0,
        date = 1,
        name = 2,
        priority = 3
    };
    
    
    //main functions
    void create_task_display(vector<taskList::task_class>&, vector<string>&, const sort_method&);
    void create_task_display(vector<taskList::task_class>&, vector<string>&);
    void sort_tasklist(vector<taskList::task_class>&, const sort_method&);
    void display_tasks(const vector<taskList::task_class>&, scrollDisplay::scroll_display_class&);
    
    
    //helper functions
    bool sort_compare(const taskList::task_class&, const taskList::task_class&, const sort_method&);
    bool compare_by_date(const taskList::task_class&, const taskList::task_class&);
    bool compare_by_name(const taskList::task_class&, const taskList::task_class&);
    bool no_compare(const taskList::task_class&, const taskList::task_class&);
    bool compare_by_priority(const taskList::task_class&, const taskList::task_class&);
    
    
    //task comparisons: 
    bool no_compare(const taskList::task_class& task1, const taskList::task_class& task2)
    {
        return true;
    }
    
    bool compare_by_date(const taskList::task_class& task1, const taskList::task_class& task2)
    {
        date::date_val d1, d2;
        d1 = task1.info.ddate.t;
        d2 = task2.info.ddate.t;
        return (d1 < d2);
    }
    
    bool compare_by_name(const taskList::task_class& task1, const taskList::task_class& task2)
    {
        return (task1.info.name < task2.info.name);
    }
    
    bool compare_by_priority(const taskList::task_class& task1, const taskList::task_class& task2)
    {
        return (task1.info.priority < task2.info.priority);
    }
    
    //task comparisons end
    
    
    inline bool sort_compare(const taskList::task_class& task1, const taskList::task_class& task2, const sort_method& method)
    {
        function<bool(const taskList::task_class&, const taskList::task_class&)> compare_function[4] = {
            no_compare,
            compare_by_date,
            compare_by_name,
            compare_by_priority
        };
        return compare_function[method](task1, task2);
    }
    
    inline void sort_tasklist(vector<taskList::task_class>& tasks, const sort_method& method)
    {
        if((tasks.size() > 1) && (method != none))
        {
            taskList::task_class temptask(*tasks.begin());
            tasks.erase(tasks.begin());
            for(vector<taskList::task_class>::iterator it = tasks.begin(); it != tasks.end(); it++)
            {
                if(sort_compare(*it, temptask, method))
                {
                    swap(*it, temptask);
                }
            }
            
            sort_tasklist(tasks, method);
            
            /* Based on the sorting method, we will want to
             use different ordering: */
            switch(method)
            {
                //greatest -> least
                case date:
                case priority:
                {
                    tasks.push_back(temptask);
                }
                break;
                
                //least -> greatest
                case name:
                {
                    tasks.insert(tasks.begin(), temptask);
                }
                break;
                
                default:
                {
                    tasks.insert(tasks.begin(), temptask);
                }
                break;
            }
        }
        tasks.shrink_to_fit();
    }
    
    inline void create_task_display(vector<taskList::task_class>& tasks, vector<string>& disp_list, const sort_method& sort)
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
    
    inline void create_task_display(vector<taskList::task_class>& tasks, vector<string>& disp_list)
    {
        create_task_display(tasks, disp_list, none);
    }
    
    inline void display_tasks(const vector<taskList::task_class>& tasks, scrollDisplay::scroll_display_class& display)
    {
        date::date_val d1, d2;
        d2 = get_time();
        for(char x = 0; x < signed(display.window().size()); x++)
        {
            d1 = tasks.at(display.window_beg() + x).info.ddate.t;
            if(d1 == d2) cout<< " *";
            if(d1 < d2) cout<< ">>";
            if(d2 < d1) cout<< "  ";
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
            cout<< endl;
        }
    }
    
    
}

namespace filesystem
{
    bool save_tasklist(const std::vector<taskList::task_class>&, const std::string&);
    
    
    inline bool save_tasklist(const std::vector<taskList::task_class>& tasks, const std::string& filename)
    {
        /* add function to true/false if string contains bad chars */
    }
    
    
}

namespace taskListMenu
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
            for(char x = 0; x < 3; x++) cout<< endl;
            
            cout<< " 1 --------  Name: \""<< task.info.name<< "\""<< endl;
            cout<< " 2 -  Description: \""<< task.info.description<< "\""<< endl;
            cout<< " 3 --------  Date: ";
            if(to_dv(task.info.ddate.t) != tempdate)
            {
                tempdate = task.info.ddate.t;
            }
            
            switch(task.info.ddate.on)
            {
                case true:
                {
                    cout<< date::display(tempdate);
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
    
    void task_list_menu()
    {
        using namespace common;
        
        //declarations:
        vector<int> ch;
        bool finished(false);
        scrollDisplay::scroll_display_class display;
        vector<string> display_list;
        vector<taskList::task_class> tasks;
        taskDisplay::sort_method sort(taskDisplay::date);
        string sort_names[(int(METHOD_MAX) + 1)] = {
            "none",
            "date",
            "name",
            "priority"
        };
        
        //initialization work (display setup, etc...):
        display = scrollDisplay::scroll_display_class(display_list);
        taskDisplay::create_task_display(tasks, display_list, sort);
        
        /*window size can be set to anything.  Feel free to change it 
         based on your preference! :)  */
        display.window_size() = 15;
        
        //main loop:
        do
        {
            cls();
            cout<< endl;
            center("Task List: ");
            cout<< endl;
            for(char x = 0; x < 3; x++) cout<< '\n';
            
            taskDisplay::display_tasks(tasks, display);
            
            cout<< '\n';
            for(char x = 0; x < 4; x++) cout<< '\n';
            
            if(display.window().size() > 0)
            {
                cout<< "[ENTER] -  Modify";
                cout<< "  [DELETE] -  Delete  ";
                cout<< "[HOME] -  First task  ";
                cout<< "[END] -  Last task  ";
            }
            cout<< "\n";
            cout<< " s -  Toggle Sort: "<< sort_names[sort]<< '\n';
            cout<< " a -  Add new task\n";
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
                            if((ch == RIGHT_KEY) || (ch == PGUP_KEY))
                            {
                                display.pg_up();
                            }
                            if((ch == LEFT_KEY) || (ch == PGDOWN_KEY))
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
                                //if is_sure()!  add it to common.cpp
                                tasks.erase(tasks.begin() + display.gpos().whole);
                                taskDisplay::create_task_display(tasks, display_list, sort);
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
                                        }
                                        taskDisplay::create_task_display(tasks, display_list, sort);
                                    }
                                    break;
                                    
                                    case 's':
                                    {
                                        {
                                            taskDisplay::sort_method tempsort[4] = {
                                                taskDisplay::none,
                                                taskDisplay::date,
                                                taskDisplay::name,
                                                taskDisplay::priority};
                                            sort = tempsort[((int(sort) + 1) % (int(METHOD_MAX) + 1))];
                                            taskDisplay::create_task_display(tasks, display_list, sort);
                                        }
                                    }
                                    break;
                                    
                                    case 'e':
                                    {
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
                                                taskDisplay::create_task_display(tasks, display_list, sort);
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
        
        //function end
    }
    
    
}

