#include <string>
#include <sstream>
#include <vector>

#include "scroll_display.hpp"

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

namespace scrollDisplay
{
    /* True/false based on whether it moved the window or not. */
    bool scroll_display_class::scroll_up()
    {
        this->sync();
        bool success(false);
        if(this->wind.size > 0)
        {
            if(this->wind.beg > 0)
            {
                success = true;
                this->wind.beg--;
            }
        }
        return success;
    }
    
    /* True/false based on whether it moved the window or not. */
    bool scroll_display_class::scroll_down()
    {
        this->sync();
        bool success(false);
        if(this->wind.size > 0)
        {
            if(this->end_pos() > 0)
            {
                if((unsigned(this->end_pos()) + 1) < this->display->size())
                {
                    this->wind.beg++;
                    success = true;
                }
            }
        }
        return success;
    }
    
    /* True/false based on whether it moved the window or not. */
    bool scroll_display_class::scroll_pg_down()
    {
        this->sync();
        bool success(false);
        if(this->wind.size > 0)
        {
            if(this->end_pos() > 0)
            {
                if(unsigned((this->end_pos() + (this->wind.size - 1))) < this->display->size())
                {
                    success = true;
                    this->wind.beg += (this->wind.size - 1);
                }
            }
        }
        return success;
    }
    
    /* True/false based on whether it moved the window or not. */
    bool scroll_display_class::scroll_pg_up()
    {
        this->sync();
        bool success(false);
        switch(this->wind.beg >= (this->wind.size - 1))
        {
            case true:
            {
                success = true;
                this->wind.beg -= (this->wind.size - 1);
            }
            break;
            
            case false:
            {
                if(this->wind.beg != 0)
                {
                    this->wind.beg = 0;
                    success = true;
                }
            }
            break;
            
            default:
            {
            }
            break;
        }
        return success;
    }
    
    std::vector<std::string> scroll_display_class::window()
    {
        this->sync();
        using namespace std;
        vector<string> tempv;
        for(unsigned int x = this->wind.beg; 
                ((x < (this->wind.beg + this->wind.size)) && (x < this->display->size())); x++)
        {
            tempv.push_back(this->display->at(x));
        }
        return tempv;
    }
    
    /* Keeps the window within the bounds of the display, the
     display the proper size, and the positions within bounds. */
    void scroll_display_class::sync()
    {
        //make sure the beg is within defined bounds
        if(this->wind.size > 0)
        {
            //make sure the current position is within the vector:
            while(this->pos.whole >= this->display->size()) this->pos.whole--;
            
            //make sure the current position is in the field of view:
            while(this->pos.whole < this->wind.beg)
            {
                this->wind.beg--;
            }
            while(this->pos.whole > this->end_pos())
            {
                this->wind.beg++;
            }
            
            //reset the part position:
            this->pos.part = (this->pos.whole - this->wind.beg);
            
            //make sure that we can always have a full window, within the vector's bounds:
            while((unsigned(this->end_pos()) == (this->display->size() - 1)) && (this->current_wsize() < this->wind.size))
            {
                this->wind.beg--;
            }
            
            //correct if beg is < 0
            if(this->wind.beg < 0) this->wind.beg = 0;
        }
    }
    
    bool scroll_display_class::mv_up()
    {
        this->sync();
        bool success(false);
        if(this->display->size() > 0)
        {
            if(this->pos.whole > 0)
            {
                this->pos.whole--;
            }
        }
        return success;
    }
    
    bool scroll_display_class::mv_down()
    {
        this->sync();
        bool success(false);
        if(this->display->size() > 0)
        {
            if((this->pos.whole + 1) < this->display->size())
            {
                this->pos.whole++;
                this->sync();
                success = true;
            }
        }
        return success;
    }
    
    bool scroll_display_class::pg_up()
    {
        this->sync();
        bool success(false);
        if(this->display->size() > 0)
        {
            if(this->pos.whole >= (this->wind.size - 1))
            {
                this->pos.whole -= (this->wind.size - 1);
                this->sync();
                success = true;
            }
        }
        return success;
    }
    
    bool scroll_display_class::pg_down()
    {
        this->sync();
        bool success(false);
        if(this->display->size() > 0)
        {
            if(unsigned(((this->display->size() - 1) - this->pos.whole)) >= (this->wind.size - 1))
            {
                this->pos.whole += (this->wind.size - 1);
                success = true;
                this->sync();
            }
        }
        return success;
    }
    
    const position_data& scroll_display_class::gpos()
    {
        this->sync();
        return this->pos;
    }
    
    
}

