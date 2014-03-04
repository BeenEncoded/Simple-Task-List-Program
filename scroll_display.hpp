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
        void operator=(const window_data& wd)
        {
            if(this != & wd)
            {
                this->beg = wd.beg;
                this->size = wd.size;
            }
        }
        
        signed long size = 15, beg = 0;
    };
    
    struct position_data
    {
        void operator=(const position_data& pd)
        {
            if(this != &pd)
            {
                this->part = pd.part;
                this->whole = pd.whole;
            }
        }
        
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
            if(&d == NULL)
            {
                throw "explicit scroll_display_class(std::vector<std::string> d) : display(&d), wind(), pos()  ERROR: Null argument!";
            }
        }
        
        ~scroll_display_class(){}
        
        void operator=(const scroll_display_class& sdc)
        {
            if(this != &sdc)
            {
                this->display = sdc.display;
                this->pos = sdc.pos;
                this->wind = sdc.wind;
            }
        }
        
        bool mv_up();
        bool mv_down();
        bool pg_up();
        bool pg_down();
        
        std::vector<std::string> window();
        
        const position_data& gpos();
        long &window_size()
        {
            return this->wind.size;
        }
        
        const signed long& window_beg()
        {
            this->sync();
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
        
        /* Calculates the last possible index that can
         be safely referenced from the pointed vector, based on where the window
         is positioned.*/
        signed long end_pos() const
        {
            signed long temp(this->wind.beg + (this->wind.size - 1));
            if(temp > 0)
            {
                while(unsigned(temp) >= this->display->size()) temp--;
            }
            if(temp < this->wind.beg)
            {
                throw "Error:  signed long end_pos() const (end_pos < wind.begin)!!!";
            }
            return temp;
        }
        
        /* Returns the size of the window which has been "resized"
         to fit within the pointed vector.  Does not reflect
         the value of wind.size.  (this should be <= wind.size)*/
        signed long current_wsize() const
        {
            return ((this->end_pos() - this->wind.beg) + 1);
        }
        
        
    };
    
    
}

#endif