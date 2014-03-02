#include <iostream>
#include <string>
#include <sstream>
#include <time.h>

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
    
    
}

namespace taskList
{
}

