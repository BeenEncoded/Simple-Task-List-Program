#ifndef DATE_CLASS_HPP_INCLUDED
#define DATE_CLASS_HPP_INCLUDED
#include <time.h>

namespace date
{
    /* Used to apply math to dates.  Allows
     addition and subraction of days from/to
     dates.  Simple object to allow flexible manipulation
     of dates, and converting that into, and from, struct tm.*/
    struct date_val
    {
        void operator=(const date_val&);
        void operator=(const struct tm&);
        
        bool operator==(const date_val&);
        bool operator!=(const date_val&);
        
        void to_tm(struct tm&) const;
        
        date_val operator++(void);
        date_val operator--(void);
        
        date_val operator+(const unsigned int&) const;
        date_val operator-(const unsigned int&) const;
        
        date_val operator+=(const unsigned int&);
        date_val operator-=(const unsigned int&);
        
        /** all vars start from 0. (except for the year)*/
        unsigned int year = 0, yday = 0;
        unsigned short month = 0, mday = 0, weekday = 0;
    };
}

#endif