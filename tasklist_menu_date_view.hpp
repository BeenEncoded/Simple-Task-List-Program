#ifndef TASKLIST_MENU_DATE_VIEW_HPP_INCLUDED
#define TASKLIST_MENU_DATE_VIEW_HPP_INCLUDED
#include "tasklist_menu_globals.hpp"
#include <vector>
#include "task_class.hpp"

namespace tasklist_menu_date_view
{
    tasklist_menu_globals::menu_return_data task_list_menu_by_date(std::vector<taskList::task_class>&);
}

#endif