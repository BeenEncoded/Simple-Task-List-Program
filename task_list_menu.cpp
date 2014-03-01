#include <iostream>
#include <string>
#include <sstream>
#include <time.h>

#include "task_list_menu.hpp"
#include "common.hpp"
#include "global_defines.hpp"
#include "date_class.hpp"

using namespace std;

namespace
{
    struct tm get_time();
    
    
    
    template<class type1, class type2>
    inline type2 conv(const type1& t1)
    {
        stringstream ss;
        type2 t2;
        ss<< t1;
        ss>> t2;
        return t2;
    }
    
    inline struct tm get_time()
    {
        struct tm *temptm;
        time_t temp_time;
        
        (void)time(&temp_time);
        temptm = localtime(&temp_time);
        
        return *temptm;
    }
    
    
}

namespace taskListMenu
{
    
}

