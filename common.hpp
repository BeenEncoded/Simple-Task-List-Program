#ifndef COMMON_HPP_INCLUDED
#define COMMON_HPP_INCLUDED
#include <string>
#include <vector>

namespace common
{
    void cl();
    void cls();
    char gkey();
    std::vector<int> gkey_funct();
    bool kbhit();
    void wait();
    
    
    bool is_letter(const char&);
    bool is_number(const char&);
    bool is_special(const char&);
    bool is_char(const char&);
    void center(const std::string&);
    
    bool string_is_int(const std::string&);
    
    inline std::string lcase(const std::string& s)
    {
        std::string temps(s);
        for(std::string::iterator it = temps.begin(); it != temps.end(); it++)
        {
            *it = tolower(*it);
        }
        return temps;
    }
    
    namespace inp
    {
        std::string get_user_string(const std::string&);
        bool is_sure(const std::string&);
    }
}


#endif