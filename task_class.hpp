#ifndef TASK_CLASS_HPP_INCLUDED
#define TASK_CLASS_HPP_INCLUDED
#include <time.h>
#include <string>

/** Task_class may not be much, but operators and other functions may be
 added in the future.*/
namespace taskList
{
    class task_class;
    
    struct due_date_data
    {
        bool on = false;
        struct tm t;
        
        void operator=(const due_date_data& ddd)
        {
            if(this != &ddd)
            {
                this->on = ddd.on;
                this->t = ddd.t;
            }
        }
        
        
    };
    
    struct task_data
    {
        due_date_data ddate;
        std::string name = "No_Name", description = "No_Description";
        short priority = 4;
        
        void operator=(const task_data& t)
        {
            if(this != &t)
            {
                this->ddate = t.ddate;
                this->name = t.name;
                this->description = t.description;
                this->priority = t.priority;
            }
        }
        
    };
    
    /* An object to represent a single task: */
    class task_class
    {
    public:
        explicit task_class(const task_data& td) : info(td) {}
        explicit task_class() : info() {}
        ~task_class(){}
        
        void operator=(const task_class& t)
        {
            if(this != &t)
            {
                this->info = t.info;
            }
        }
        
        task_data info;
        
    };
    
    
}

#endif