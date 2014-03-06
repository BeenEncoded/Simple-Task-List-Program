#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <type_traits>

#include "task_list_menu.hpp"
#include "common.hpp"
#include "global_defines.hpp"
#include "date_class.hpp"
#include "task_class.hpp"

#include <assert.h>

using namespace std;

namespace
{
    struct tm get_time();
    date::date_val to_dv(const struct tm&);
    bool modify_date(date::date_val&);
    void mod_date(date::date_val&, const char&, const int&);
    
    
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
            disp += ("(" + d.wday_name() + ")   ");
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
            
            
            common::center(disp);
            cout<< endl;
            for(char x = 0; x < 3; x++) cout<< endl;
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
            center("Modify Task");
            cout<< endl;
            for(char x = 0; x < 3; x++) cout<< endl;
            
            cout<< " 1 --------  Name: \""<< task.info.name<< "\""<< endl;
            cout<< " 2 -  Description: \""<< task.info.description<< "\""<< endl;
            cout<< " 3 --------  Date: ";
            switch(task.info.ddate.on)
            {
                case true:
                {
                    if(to_dv(task.info.ddate.t) != tempdate) tempdate = task.info.ddate.t;
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
            
            cout<< " 4 ----  Priority: "<< task.info.priority<< endl;
            
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
                                        modified = user_input_string(task.info.name, ("Current \
value: \"" + task.info.name + "\"\n\n\nEnter the name: "));
                                        if(task.info.name.size() > 40)
                                        {
                                            task.info.name.resize(40 - 3);
                                            task.info.name += "...";
                                        }
                                    }
                                    break;
                                    
                                    case '2':
                                    {
                                        modified = user_input_string(task.info.description, ("\
Current value: \"" + task.info.description + "\"\n\n\nEnter the Description: "));
                                        
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
                                        //modify date
                                    }
                                    break;
                                    
                                    case '4':
                                    {
                                        modified = user_input_string<short>(task.info.priority, std::string("\
Currently set to: " + conv<short, std::string>(task.info.priority) + "\n\n\nEnter a priority (1-10): "));
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
    
    
}

