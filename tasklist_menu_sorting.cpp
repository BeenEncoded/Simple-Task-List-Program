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
#include "tasklist_menu_sorting.hpp"

using namespace std;
using namespace tasklist_menu_sorting;

namespace
{
    bool sort_compare(const taskList::task_class&, const taskList::task_class&, const sort_method&);
    bool compare_by_date(const taskList::task_class&, const taskList::task_class&);
    bool compare_by_name(const taskList::task_class&, const taskList::task_class&);
    bool no_compare(const taskList::task_class&, const taskList::task_class&);
    bool compare_by_priority(const taskList::task_class&, const taskList::task_class&);
    std::function<bool(const taskList::task_class&, const taskList::task_class&)> get_compare_funct(const sort_method&);
    
    
    
    inline bool sort_compare(const taskList::task_class& task1, const taskList::task_class& task2, const sort_method& method)
    {
        return (get_compare_funct(method)(task1, task2));
    }
    
    template<class type>
    inline bool cansort(const std::vector<type>& v, const std::function<bool(const type&, const type&)>& lessthan)
    {
        bool needs_sort(false);
        if(v.size() > 1)
        {
            for(typename std::vector<type>::const_iterator it = v.begin(), it2 = (v.begin() + 1);
                    ((it2 != v.end()) && !needs_sort); ++it, ++it2)
            {
                if(!lessthan((*it), (*it2))) needs_sort = true;
            }
        }
        return needs_sort;
    }
    
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
        return (common::lcase(task1.info.name) < common::lcase(task2.info.name));
    }
    
    bool compare_by_priority(const taskList::task_class& task1, const taskList::task_class& task2)
    {
        return (task1.info.priority < task2.info.priority);
    }
    
    inline std::function<bool(\
            const taskList::task_class&,\
            const taskList::task_class&)>\
                    get_compare_funct(const sort_method& sort)
    {
        std::function<bool(const taskList::task_class&, const taskList::task_class&)> f[(METHOD_MAX + 1)] = {
            no_compare,
            compare_by_date,
            compare_by_name,
            compare_by_priority
        };
        return f[sort];
    }
    
    
}

namespace tasklist_menu_sorting
{
    void toggle_sort_method(sort_method& sort)
    {
        sort_method sort_vals[(METHOD_MAX + 1)] = {
            none,
            date,
            name,
            priority
        };
        
        short x(sort);
        x++;
        x %= (METHOD_MAX + 1);
        sort = sort_vals[x];
    }
    
    void sort_dates(vector<date::date_val>& dates)
    {
        if(dates.size() > 1)
        {
            date::date_val temp_date(*(dates.begin()));
            dates.erase(dates.begin());
            
            for(vector<date::date_val>::iterator it = dates.begin(); it != dates.end(); it++)
            {
                if(*it < temp_date)
                {
                    swap(*it, temp_date);
                }
            }
            sort_dates(dates);
            dates.push_back(temp_date);
        }
    }
    
    void sort_tasklist(std::vector<taskList::task_class>& tasks, const sort_method& method)
    {
        if((tasks.size() > 1) && (method != none) && 
                cansort<taskList::task_class>(tasks, get_compare_funct(method)))
        {
            taskList::task_class temptask(*tasks.begin());
            tasks.erase(tasks.begin());
            bool reallocated(false);
            unsigned int itref(0);
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
            
            if(method == date)
            {
                //now we move all the tasks with no due date to the back:
                itref = 0;
                for(std::vector<taskList::task_class>::iterator it = tasks.begin(); it != tasks.end(); ++it, itref++)
                {
                    if(!it->info.ddate.on)
                    {
                        temptask = *it;
                        it = tasks.erase(it);
                        reallocated = ((tasks.size() + 1) > tasks.capacity());
                        tasks.push_back(temptask);
                        if(reallocated)
                        {
                            reallocated = false;
                            it = (tasks.begin() + itref);
                        }
                    }
                }
            }
        }
        tasks.shrink_to_fit();
    }
    
}