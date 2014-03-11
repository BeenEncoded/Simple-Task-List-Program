#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "iofunctions.hpp"
#include "common.hpp"
#include "global_defines.hpp"

namespace
{
    bool char_match(const char&, const std::string&);
    
    
    template<class type1, class type2>
    inline type2 conv(const type1& t1)
    {
        std::stringstream ss;
        type2 t2;
        ss<< t1;
        ss>> t2;
        return t2;
    }
    
    inline bool char_match(const char& ch, const std::string& s)
    {
        bool match(false);
        for(std::string::const_iterator it = s.begin(); ((it != s.end()) && !match); ++it)
        {
            match = (*it == ch);
        }
        return match;
    }
    
    
}

namespace common
{
    bool is_number(const char& ch)
    {
        return char_match(ch, NUMBERS);
    }
    
    bool is_special(const char& ch)
    {
        return char_match(ch, SPECIALS);
    }
    
    bool is_letter(const char& ch)
    {
        return char_match(ch, LETTERS);
    }
    
    bool is_char(const char& ch)
    {
        return (is_letter(ch) || is_number(ch) || is_special(ch));
    }
    
    void center(const std::string& s)
    {
        using namespace std;
        
        int max = (s.size() / 2);
        max -= int(HCENTER);
        if(max < 0) max = 0;
        for(int x = 0; x < max; x++) cout<< " ";
        cout<< s;
    }
    
    bool string_is_int(const std::string& s)
    {
        unsigned short count(0);
        for(std::string::const_iterator it = s.begin(); ((it != s.end()) && (count == 0)); ++it)
        {
            if(!is_number(*it))
            {
                count++;
            }
        }
        return (count == 0);
    }
    
    void wait()
    {
        for(short x = 0; x < 3; x++) std::cout<< std::endl;
        std::cout<< "Press any key to continue..."<< std::endl;
        cl();
        gkey();
        cl();
    }
    
    void cl()
    {
        input::cl();
    }
    
    void cls()
    {
        output::cls();
    }
    
    char gkey()
    {
        return input::getch();
    }
    
    bool kbhit()
    {
        return input::kbhit();
    }
    
    std::vector<int> gkey_funct()
    {
        using namespace std;
        
        vector<int> key;
        key.push_back(input::getch());
        if(IS_CONTROL(key[0]))
        {
            while(input::kbhit())
            {
                key.push_back(input::getch());
            }
        }
        return key;
    }
    
    namespace inp
    {
        std::string get_user_string(const std::string& s)
        {
            using namespace std;
            
            
            vector<int> key;
            bool finished(false);
            string input;
            
            while(!finished)
            {
                cls();
                for(short x = 0; x < 6; x++) cout<< endl;
                cout<< ((s.size() > 0) ? s : ">> ")<< input;
                cin.rdbuf()->in_avail();
                cl();
                do
                {
                    key = gkey_funct();
                    if(key.size() > 0)
                    {
                        switch(IS_CONTROL(key[0]))
                        {
                            case true:
                            {
                                if(key == END_KEY)
                                {
                                    input.erase();
                                    input = GSTRING_CANCEL;
                                    finished = true;
                                }
                            }
                            break;
                            
                            case false:
                            {
                                switch(is_char(key[0]))
                                {
                                    case true:
                                    {
                                        input += char(key[0]);
                                    }
                                    break;
                                    
                                    case false:
                                    {
                                        switch(key[0])
                                        {
                                            case ENTER_KEY:
                                            {
                                                finished = true;
                                            }
                                            break;
                                            
                                            case BACKSPACE_KEY:
                                            {
                                                if(input.size() > 0)
                                                {
                                                    input.resize((input.size() - 1));
                                                }
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
                            break;
                            
                            default:
                            {
                            }
                            break;
                        }
                    }
                }while(kbhit());
            }
            return input;
        }
        
        
    }
}

