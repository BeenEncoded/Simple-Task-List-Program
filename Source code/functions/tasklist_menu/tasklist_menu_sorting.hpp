#ifndef TASKLIST_MENU_SORTING_HPP_INCLUDED
#define TASKLIST_MENU_SORTING_HPP_INCLUDED
#include <vector>

#include "../../classes/date_class.hpp"

namespace tasklist_menu_sorting
{
    #define METHOD_MAX 3
    enum sort_method{
        none = 0,
        date = 1,
        name = 2,
        priority = 3
    };
    
    void sort_dates(std::vector<date::date_val>&);
    void toggle_sort_method(sort_method&);
    void sort_tasklist(std::vector<taskList::task_class>&, const sort_method&);
}

#endif
