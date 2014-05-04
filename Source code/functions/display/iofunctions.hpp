#ifndef IOFUNCTIONS_HPP_INCLUDED
#define IOFUNCTIONS_HPP_INCLUDED

/* A set of windows-equivilant functions for the unix operating system.
 All of them use termios, linux' native terminal I/O API. */

namespace input
{
    //reads a button press
    int getch();
    
    //returns true/false if a key has been pressed
    bool kbhit();
    
    //clears the buffer that getchar() reads from.
    void cl();
}

namespace output
{
    //clears the terminal Screen
    void cls();
}

//shorthand for adding input to a file
#define ADDIN using namespace input;

//shorthand for adding output to a file
#define ADDOUT using namespace output;

#endif