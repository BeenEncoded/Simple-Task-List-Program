#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <time.h>
#include <map>

#include "common.hpp"
#include "scroll_display.hpp"
#include "task_class.hpp"
#include "date_class.hpp"
#include "global_defines.hpp"
#include "tasklist_menu_globals.hpp"
#include "tasklist_menu_date_view.hpp"

using namespace std;

//classes
namespace
{
    /** Allows the manipulation of sublists matching
     a specific condition.  It directly operates on 
     the parent vector, so no overhead memory is used, 
     and sorting is not necessary. */
    template<class type>
    class sub_list
    {
    public:
        explicit sub_list(typename std::vector<type>& v, 
                std::function<bool(const type&)> mcond) : entire_list(&v), 
                        match_condition(mcond){}
        
        explicit sub_list() : entire_list(nullptr), match_condition(){}
        
        ~sub_list(){}
        
        void operator=(const sub_list& sl)
        {
            if(this != &sl)
            {
                this->entire_list = sl.entire_list;
                this->match_condition = sl.match_condition;
            }
        }
        
        /* as if operating on a vector of the matching 
         elements, returns a reference to the (i)th element
         that matches. */
        type& operator[](const unsigned int& i)
        {
            if(this->entire_list == nullptr)
            {
                throw "\nsub_list class:  CANNOT REFERENCE NULLPTR!\n";
            }
            unsigned int loc(i);
            if(!this->get_element_pos(loc)) loc = 0;
            return ((*(this->entire_list))[loc]);
        }
        
        /* Returns true/false based on whether or not there
         are matches that can be referenced. */
        bool can_reference() const
        {
            bool canit(false);
            if(this->entire_list != nullptr)
            {
                canit = (this->match_count() > 0);
            }
            return canit;
        }
        
        bool del(const unsigned int& i)
        {
            if(this->entire_list == nullptr)
            {
                throw "\nsub_list class:  CANNOT REFERENCE NULLPTR!\n";
            }
            bool success(false);
            unsigned int loc(i);
            if(this->get_element_pos(loc))
            {
                success = true;
                this->entire_list->erase((this->entire_list->begin() + loc));
            }
            return success;
        }
        
        typename std::vector<type> get_list() const
        {
            unsigned int size(this->match_count()), loc(0);
            typename std::vector<type> l;
            for(unsigned int x = 0; x < size; x++)
            {
                loc = x;
                if(this->get_element_pos(loc))
                {
                    l.push_back(((*(this->entire_list))[loc]));
                }
            }
            return l;
        }
        
        unsigned int size() const
        {
            unsigned int size(0);
            if(this->can_reference())
            {
                size = this->match_count();
            }
            return size;
        }
        
        
    private:
        typename std::vector<type>* entire_list;
        std::function<bool(const type&)> match_condition;
        
        unsigned int match_count() const
        {
            if(this->entire_list == nullptr)
            {
                throw "\nsub_list class:  CANNOT REFERENCE NULLPTR!\n";
            }
            unsigned int count(0);
            for(typename vector<type>::const_iterator it = this->entire_list->begin(); 
                    it != this->entire_list->end(); ++it)
            {
                if(this->match_condition(*it)) count++;
            }
            return count;
        }
        
        /* Pass 'i' as the Nth element that matches.  The
         function will modify it to be the Nth element in
         the entire list.  True/false is returned based
         on whether the element was within range.*/
        bool get_element_pos(unsigned int& i) const
        {
            if(this->entire_list == nullptr)
            {
                throw "\nsub_list class:  CANNOT REFERENCE NULLPTR!\n";
            }
            unsigned int size(this->entire_list->size()), loc(0);
            bool found(false);
            for(unsigned int x = 0; ((x < size) && !found); x++)
            {
                if(this->match_condition(((*(this->entire_list))[x])))
                {
                    if(loc == i)
                    {
                        found = true;
                        i = x;
                    }
                    loc++;
                }
            }
            return found;
        }
        
        
    };
    
    
}


//functions
namespace
{
    inline bool modify_tasklist(const std::string&, std::vector<taskList::task_class>&, const date::date_val&);
    void display_dates(scrollDisplay::scroll_display_class&);
    void create_date_display(
            const std::vector<date::date_val>&,
            std::vector<std::string>&);
    vector<date::date_val> get_dates(const vector<taskList::task_class>&);
    
    
    
    inline bool modify_tasklist(
            const std::string& title,
            std::vector<taskList::task_class>& tasks,
            const date::date_val& d)
    {
        using namespace tasklist_menu_globals;
        using namespace tasklist_menu_sorting;
        
        bool modified(false), finished(false);
        
        sub_list<taskList::task_class> tasks_of_date(tasks, [d](const taskList::task_class& temptask)->bool{
            return (((d == to_dv(temptask.info.ddate.t)) && temptask.info.ddate.on) || 
                    ((d == to_dv(get_time())) && !temptask.info.ddate.on));});
        
        vector<string> display_list;
        scrollDisplay::scroll_display_class display(display_list);
        vector<int> ch;
        sort_method sort(tasklist_menu_sorting::date);
        std::string sort_names[(METHOD_MAX + 1)] = {
                "none",
                "date",
                "name",
                "priority"};
        
        /* This is a special function made for this menu. It updates the display.  
         This is a costly operation, so display updates should only happen after
         display modifications. */
        auto update_display = \
[&tasks, &tasks_of_date, &display_list, &sort](void){
            sort_tasklist(tasks, sort);
            std::vector<taskList::task_class> temp(tasks_of_date.get_list());
            create_task_display(temp, display_list);
        };
        display.window_size() = 10;
        
        //Initialize the display first:
        update_display();
        
        while(!finished && (tasks_of_date.size() > 0))
        {
            common::cls();
            cout.flush();
            cout<< '\n';
            common::center(title);
            cout<< '\n';
            cout<< string(3, '\n');
            
            cout<< string(50, 'V')<< '\n';
            display_tasks(tasks_of_date.get_list(), display);
            cout<< string((display.window_size() - display.window().size()), '\n');
            cout<< string(50, '^')<< '\n';
            cout<< string(2, '\n');
            cout<< "[ENTER] - Modify || [DEL] - Delete ||\
 [HOME] - First || [END] - Last"<< '\n';
            cout<< string(2, '\n');
            cout<< "s -  Toggle Sort: "<< sort_names[sort]<< '\n';
            cout<< "[BACSPACE] -  Done\n";
            
            common::cl();
            ch = common::gkey_funct();
            if(ch.size() > 0)
            {
                switch(IS_CONTROL(ch[0]))
                {
                    case true:
                    {
                        if(ch == UP_KEY)
                        {
                            display.mv_up();
                        }
                        else if(ch == DOWN_KEY)
                        {
                            display.mv_down();
                        }
                        else if((ch == RIGHT_KEY) || (ch == PGDOWN_KEY))
                        {
                            display.pg_down();
                        }
                        else if((ch == LEFT_KEY) || (ch == PGUP_KEY))
                        {
                            display.pg_up();
                        }
                        else if(ch == HOME_KEY)
                        {
                            while(display.pg_up());
                        }
                        else if(ch == END_KEY)
                        {
                            while(display.pg_down());
                        }
                        else if(ch == DELETE_KEY)
                        {
                            if(common::inp::is_sure("Are you sure you want to delete\
\"" + tasks_of_date[display.gpos().whole].info.name + "\"?"))
                            {
                                tasks_of_date.del(display.gpos().whole);
                                modified = true;
                                update_display();
                            }
                            if(tasks_of_date.size() == 0) finished = true;
                        }
                        else
                        {
                        }
                    }
                    break;
                    
                    case false:
                    {
                        switch(common::is_char(ch[0]))
                        {
                            case true:
                            {
                                switch(tolower(char(ch[0])))
                                {
                                    case 's':
                                    {
                                        toggle_sort_method(sort);
                                        update_display();
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
                                        {
                                            taskList::task_class temp_task(tasks_of_date[display.gpos().whole]);
                                            if(modify_task(temp_task))
                                            {
                                                tasks_of_date[display.gpos().whole] = temp_task;
                                                modified = true;
                                                update_display();
                                            }
                                            //over-writing the temp_task
                                            temp_task = taskList::task_class();
                                            if(tasks_of_date.size() == 0) finished = true;
                                        }
                                    }
                                    break;
                                    
                                    case BACKSPACE_KEY:
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
        }
        
        display_list.erase(display_list.begin(), display_list.end());
        display_list.shrink_to_fit();
        
        for(unsigned short x = 0; x < (METHOD_MAX + 1); x++) sort_names[x].erase();
        
        return modified;
    }
    
    inline void display_dates(scrollDisplay::scroll_display_class& display)
    {
        short size(display.window().size());
        vector<string> window = display.window();
        for(short x = 0; x < size; x++)
        {
            switch(x == display.gpos().part)
            {
                case true:
                {
                    cout<< '['<< window[x]<< ']';
                }
                break;
                
                case false:
                {
                    cout<< ' '<< window[x];
                }
                break;
                
                default:
                {
                    cout<< "_ERROR";
                }
                break;
            }
            cout<< '\n';
        }
        window.erase(window.begin(), window.end());
    }
    
    inline void create_date_display(std::vector<date::date_val>& dates,
            std::vector<std::string>& display_list)
    {
        display_list.erase(display_list.begin(), display_list.end());
        tasklist_menu_sorting::sort_dates(dates);
        for(std::vector<date::date_val>::const_iterator it = dates.begin(); it != dates.end(); ++it)
        {
            display_list.push_back(date::display(*it));
        }
    }
    
    /* Retrieves a list of dates that tasks are due on. */
    inline vector<date::date_val> get_dates(const vector<taskList::task_class>& tasks)
    {
        using namespace tasklist_menu_globals;
        
        map<date::date_val, char> dates;
        vector<date::date_val> collected_dates;
        for(vector<taskList::task_class>::const_iterator t = tasks.begin(); t != tasks.end(); ++t)
        {
            switch(t->info.ddate.on)
            {
                case true:
                {
                    dates[to_dv(t->info.ddate.t)] = 'a';
                }
                break;
                
                case false:
                {
                    dates[to_dv(get_time())] = 'a';
                }
                break;
                
                default:
                {
                }
                break;
            }
        }
        for(map<date::date_val, char>::iterator d = dates.begin(); d != dates.end(); d++)
        {
            collected_dates.push_back(d->first);
        }
        dates.erase(dates.begin(), dates.end());
        return collected_dates;
    }
    
    
}


namespace tasklist_menu_date_view
{
    
    /* Allows the user to modify tasks based on their date. */
    tasklist_menu_globals::menu_return_data task_list_menu_by_date(vector<taskList::task_class>& tasks)
    {
        using namespace tasklist_menu_globals;
        
        menu_return_data ret;
        scrollDisplay::scroll_display_class display;
        vector<string> list_display;
        vector<date::date_val> dates;
        vector<int> ch;
        bool finished(false);
        auto update_display = [&tasks, &list_display, &dates](void){
            dates = get_dates(tasks);
            create_date_display(dates, list_display);
        };
        
        display = scrollDisplay::scroll_display_class(list_display);
        display.window_size() = 10;
        
        ret.command = date_view;
        ret.modified_data = false;
        
        update_display();
        
        do
        {
            common::cls();
            cout.flush();
            cout<< '\n';
            common::center("Tasks");
            cout<< '\n';
            cout<< "\nToday is "<< date::display(to_dv(get_time()))<< "("<< to_dv(get_time()).wday_name()<< ")";
            cout<< std::string(3, '\n');
            cout<< std::string(50, 'V')<< '\n';
            
            display_dates(display);
            cout<< string((display.window_size() - display.window().size()), '\n');
            
            cout<< std::string(50, '^')<< '\n';
            cout<< '\n';
            cout<< "a - add task\n";
            cout<< "L - List View\n";
            cout<< "e - Exit\n";
            
            cout<< "\n[ENTER] - Select || [HOME] - First || [END] - Last";
            
            
            ch.erase(ch.begin(), ch.end());
            common::cl();
            ch = common::gkey_funct();
            
            if(ch.size() > 0)
            {
                switch(IS_CONTROL(ch[0]))
                {
                    case true:
                    {
                        if(ch == UP_KEY)
                        {
                            display.mv_up();
                        }
                        if(ch == DOWN_KEY)
                        {
                            display.mv_down();
                        }
                        if(ch == LEFT_KEY)
                        {
                            display.pg_up();
                        }
                        if(ch == RIGHT_KEY)
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
                    }
                    break;
                    
                    case false:
                    {
                        switch(common::is_char(ch[0]))
                        {
                            case true:
                            {
                                switch(tolower(char(ch[0])))
                                {
                                    case 'e':
                                    {
                                        ret.command = quit;
                                        finished = true;
                                    }
                                    break;
                                    
                                    case 'l':
                                    {
                                        ret.command = list_view;
                                        finished = true;
                                    }
                                    break;
                                    
                                    case 'a':
                                    {
                                        {
                                            taskList::task_class temptask;
                                            temptask.info.ddate.t = get_time();
                                            if(modify_task(temptask))
                                            {
                                                tasks.push_back(temptask);
                                                ret.modified_data = true;
                                                update_display();
                                            }
                                            temptask = taskList::task_class();
                                        }
                                    }
                                    
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
                                            if(modify_tasklist(("Tasks on " + 
                                                    date::display(dates[display.gpos().whole])),
                                                    tasks, dates[display.gpos().whole]))
                                            {
                                                ret.modified_data = true;
                                                update_display();
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