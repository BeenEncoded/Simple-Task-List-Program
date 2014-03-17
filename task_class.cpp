#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <fstream>
#include <type_traits>
#include <functional>

#include "task_class.hpp"

namespace
{
    template<class type1, class type2>
    inline type2 conv(const type1& t1)
    {
        std::stringstream ss;
        type2 t2;
        ss<< t1;
        ss>> t2;
        return t2;
    }
    
    template<class data_t>
    inline bool safe_getline(std::istream& in, data_t& d, const char& delim)
    {
        bool success(false);
        std::string temps;
        
        if(in.good())
        {
            getline(in, temps, delim);
            if(!in.fail() && !(std::is_pointer<data_t>::value))
            {
                switch(std::is_fundamental<data_t>::value)
                {
                    case true:
                    {
                        d = conv<std::string, data_t>(temps);
                        success = true;
                    }
                    break;

                    case false:
                    {
                        if(std::is_same<data_t, std::string>::value)
                        {
                            d = (*reinterpret_cast<data_t*>(&temps));
                            success = true;
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
        return success;
    }
    
    
}

std::ostream& operator<<(std::ostream& out, tm& t)
{
    out<< t.tm_hour<< '\0';
    out<< t.tm_isdst<< '\0';
    out<< t.tm_mday<< '\0';
    out<< t.tm_min<< '\0';
    out<< t.tm_mon<< '\0';
    out<< t.tm_sec<< '\0';
    out<< t.tm_wday<< '\0';
    out<< t.tm_yday<< '\0';
    out<< t.tm_year<< '\0';
    return out;
}

std::istream& operator>>(std::istream& in, tm& t)
{
    if(!safe_getline(in, t.tm_hour, '\0')) return in;
    if(!safe_getline(in, t.tm_isdst, '\0')) return in;
    if(!safe_getline(in, t.tm_mday, '\0')) return in;
    if(!safe_getline(in, t.tm_min, '\0')) return in;
    if(!safe_getline(in, t.tm_mon, '\0')) return in;
    if(!safe_getline(in, t.tm_sec, '\0')) return in;
    if(!safe_getline(in, t.tm_wday, '\0')) return in;
    if(!safe_getline(in, t.tm_yday, '\0')) return in;
    (void)safe_getline(in, t.tm_year, '\0');
    return in;
}

namespace taskList
{
    
    std::ostream& operator<<(std::ostream& out, taskList::due_date_data& ddd)
    {
        out<< ddd.on<< '\0';
        out<< ddd.t;
        return out;
    }

    std::istream& operator>>(std::istream& in, taskList::due_date_data& ddd)
    {
        if(safe_getline(in, ddd.on, '\0'))
        {
            in>> ddd.t;
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, taskList::task_data& task)
    {
        out<< task.ddate;
        out<< task.name<< '\0';
        out<< task.description<< '\0';
        out<< task.priority<< '\0';
        return out;
    }

    std::istream& operator>>(std::istream& in, taskList::task_data& task)
    {
        in>> task.ddate;
        if(!safe_getline(in, task.name, '\0')) return in;
        if(!safe_getline(in, task.description, '\0')) return in;
        (void)safe_getline(in, task.priority, '\0');
        return in;
    }

    std::ostream& operator<<(std::ostream& out, taskList::task_class& task)
    {
        out<< task.info;
        return out;
    }

    std::istream& operator>>(std::istream& in, taskList::task_class& task)
    {
        in>> task.info;
        return in;
    }
    
    std::ostream& operator<<(std::ostream& out, std::vector<taskList::task_class>& tasks)
    {
        for(auto t : tasks) out<< t;
        out<< '\0';
        return out;
    }
    
    std::istream& operator>>(std::istream& in, std::vector<taskList::task_class>& tasks)
    {
        tasks.erase(tasks.begin(), tasks.end());
        while(in.good() && (char(in.peek()) != '\0'))
        {
            tasks.push_back(taskList::task_class());
            in>> tasks.back();
            if(in.fail())
            {
                tasks.pop_back();
            }
        }
        if(char(in.peek()) == '\0') in.get();
        return in;
    }
    
    
}
