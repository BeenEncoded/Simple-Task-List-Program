#ifndef TASKLIST_MENU_LIST_VIEW_HPP_INCLUDED
#define TASKLIST_MENU_LIST_VIEW_HPP_INCLUDED
#include <vector>

#include "../../classes/task_class.hpp"
#include "tasklist_menu_globals.hpp"

namespace tasklist_menu_list_view
{
    tasklist_menu_globals::menu_return_data task_list_menu(std::vector<taskList::task_class>&);
}



#endif
