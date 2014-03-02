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
            if(((unsigned)this->wind.beg + ((unsigned)this->wind.size - 1)) < this->display->size())
            {
                this->wind.beg++;
                success = true;
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
            if(((unsigned)this->wind.beg + (2 * ((unsigned)this->wind.size - 1))) < this->display->size())
            {
                success = true;
                this->wind.beg += (this->wind.size - 1);
            }
        }
        return success;
    }
    
    /* True/false based on whether it moved the window or not. */
    bool scroll_display_class::scroll_pg_up()
    {
        this->sync();
        bool success(false);
        if(this->wind.beg >= (this->wind.size - 1))
        {
            success = true;
            this->wind.beg -= (this->wind.size - 1);
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
    
    /* Keeps the window within the bounds of the display. */
    void scroll_display_class::sync()
    {
        //never ever use any member function if the display is null!!
        assert(this->display != std::nullptr_t);
        
        //make sure the beg is within defined bounds
        if(this->wind.size > 0)
        {
            if(((unsigned)this->wind.beg + ((unsigned)this->wind.size - 1)) >= this->display->size())
            {
                this->wind.beg = (this->display->size() - this->wind.size);
            }
            if(this->wind.beg < 0) this->wind.beg = 0;
        }
    }
    
    bool scroll_display_class::mv_up()
    {
        this->sync();
        bool success(false);
        if(this->display->size() > 0)
        {
            switch(this->pos.part == 0)
            {
                case true:
                {
                    success = this->scroll_up();
                }
                break;
                
                case false:
                {
                    this->part--;
                }
                break;
                
                default:
                {
                }
                break;
            }
        }
        this->pos.whole = (this->pos.part + this->wind.beg);
        return success;
    }
    
    bool scroll_display_class::mv_down()
    {
        this->sync();
        bool success(false);
        if(this->display->size() > 0)
        {
            switch(this->pos.part == (this->wind.size - 1))
            {
                case true:
                {
                    success = this->scroll_down();
                }
                break;
                
                case false:
                {
                    this->pos.part++;
                }
                break;
                
                default:
                {
                }
                break;
            }
            this->pos.whole = (this->wind.beg + this->pos.part);
        }
        return success;
    }
    
    bool scroll_display_class::pg_up()
    {
        this->sync();
        bool success(false);
        if(this->display->size() > 0)
        {
            switch(this->pos.part == (this->wind.size - 1))
            {
                case true:
                {
                    this->pos.part = 0;
                    this->pos.whole = (this->wind.beg + this->pos.part);
                }
                break;
                
                case false:
                {
                    switch(this->pos.whole >= (this->wind.size - 1))
                    {
                        case true:
                        {
                            this->pos.whole -= (this->wind.size - 1);
                            this->pos.part = 0;
                            this->wind.beg = this->pos.whole;
                            success = true;
                        }
                        break;
                        
                        case false:
                        {
                            this->pos.part = 0;
                            this->pos.whole = 0;
                            this->wind.beg = 0;
                            success = true;
                        }
                        break;
                        
                        default:
                        {
                        }
                        break;
                    }
                }
                break;
                
                default:
                {
                }
                break;
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
            if(this->pos.whole < this->display->size())
            {
                switch((this->pos.whole + (this->wind.size - 1)) < this->display->size())
                {
                    case true:
                    {
                        switch(this->pos.part == 0)
                        {
                            case true:
                            {
                                this->pos.part = (this->wind.size - 1);
                                this->pos.whole = (this->wind.beg + this->pos.part);
                            }
                            break;
                            
                            case false:
                            {
                                this->pos.whole += (this->wind.size - 1);
                                this->pos.part = (this->wind.size - 1);
                                this->wind.beg = (this->pos.whole - (this->wind.size - 1));
                                success = true;
                            }
                            break;
                            
                            default:
                            {
                            }
                            break;
                        }
                        
                        
                    }
                    break;
                    
                    case false:
                    {
                        this->pos.whole = (this->display->size() - this->wind.size);
                        this->wind.beg = (this->display->size() - this->wind.size);
                        this->pos.part = (this->wind.size - 1);
                    }
                    break;
                    
                    default:
                    {
                    }
                    break;
                }
            }
        }
        return success;
    }
    
    
}

