#ifndef SCROLL_DISPLAY_HPP_INCLUDED
#define SCROLL_DISPLAY_HPP_INCLUDED
#include <vector>
#include <string>

#include "global_defines.hpp"

namespace scrollDisplay
{
    class scroll_display_class;
    
    struct window_data
    {
        int beg = 0;
        signed long size = 15;
    };
    
    struct position_data
    {
        short part = 0;
        unsigned int whole = 0;
    };
    
    /* scroll_display_class: a class designed to manage a window and make 
     * it easy to create a scroll display.*/
    class scroll_display_class
    {
    public:
        explicit scroll_display_class() : display(NULL), wind(), pos() {}
        explicit scroll_display_class(std::vector<std::string> d) : display(&d),
                    wind(),
                    pos()
        {
            //no one should have to give this thing a null pointer...
            assert(d != std::nullptr_t);  //if you're using a null pointer, you're doing it wrong!
        }
        
        ~scroll_display_class(){}
        
        bool mv_up();
        bool mv_down();
        bool pg_up();
        bool pg_down();
        
        std::vector<std::string> window();
        
        const position_data& gpos() const;
        long &window_size()
        {
            return this->wind.size;
        }
        
        const int& window_beg() const
        {
            return this->wind.beg;
        }
        
    private:
        std::vector<std::string> *display;
        window_data wind;
        position_data pos;
        
        void sync();
        bool scroll_up();
        bool scroll_down();
        bool scroll_pg_up();
        bool scroll_pg_down();
    };
    
    
}

#endif