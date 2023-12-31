#ifndef __UTILS_H__
#define __UTILS_H__

// reference for conio _kbhit and _getch
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getch-getwch?view=msvc-170&redirectedfrom=MSDN
// for version that doesn't lock other threads(?)
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getch-nolock-getwch-nolock?view=msvc-160
// reference for keycodes
// https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-6.0/aa299374(v=vs.60)
// also interesting approaches, but likely time-consuming and too much win32 api
// https://stackoverflow.com/questions/2067893/c-console-keyboard-events/
// for termios approach, example:
// https://en.wikibooks.org/wiki/Serial_Programming/termios

#ifdef _WIN32
///[x]TODO: ifdef windows environment
//         [x]ok, works
#include "conio.h"
namespace gm {

int getch(){
   int result{0};
   if(_kbhit()){
      // std::cout << "_kbhit: true" << std::endl;
      result = _getch_nolock();
      if (result == 0 || result == 0xE0 ){
         // std::cout << "result == 0 || result = 0xE0, getting extra byte" << std::endl;
         result = _getch_nolock();
      }
   } else {
      // std::cout << "_kbhit: false" << std::endl;
   }
   // std::cout << "int from _getch: " << result << std::endl;
   return result;
}

}; // ; is not needed after namespaces (;
#elif __linux__
#include <termios.h>
///[x]TODO: ifdef linux environment
//         []implement set termios attributes
namespace gm {

int getch(){
   int result{0};
   // if(_kbhit()){
   //    // std::cout << "_kbhit: true" << std::endl;
   //    result = _getch_nolock();
   //    if (result == 0 || result == 0xE0 ){
   //       // std::cout << "result == 0 || result = 0xE0, getting extra byte" << std::endl;
   //       result = _getch_nolock();
   //    }
   // } else {
   //    // std::cout << "_kbhit: false" << std::endl;
   // }
   // std::cout << "int from _getch: " << result << std::endl;
   return result;
}

}; // ; is not needed after namespaces (;

#else
#error Platform not supported
#endif

// previous comments for input handling saga:
//[x]after all considerations, decided path was: conio for windows, termios for linux,
//   still avoiding ncurses. maybe ncurses for final version. abstraction layer
//   shall be manually created to include both cases, and when graphical display
//   pipeline is implemented then use an input library like sdl or allegro or sfml
//   for environments other than windows and linux terminals
// input handling, ascii only
//        find reasonable way to stop input buffering and avoid need to do \n in terminal input
//        []tl;dr: non-buffered input is not easy to do without low-level apis
//        [x]apparently (n)curses is the only reasonable platform-agnostic method,
//           others would be sdl, sfml, allegro, or platform-dependent methods
//           [x]sadly, ncurses destroys my text framebuffer, I will have to surrender to using their
//              framework for everything even if I wanted only getch()
//              https://stackoverflow.com/questions/58490734/is-it-possible-to-use-curses-only-to-read-a-keypress-but-nothing-else
//              "if I put some print statement before, then getkey will clear the screen,
//              no matter if I call filter or not. I do believe that it works, because thats also what
//              I understand from the documentation, just not for me :(. I am now considering to either
//               get my key without curses or switch to using curses for all io"
//                ^ same
//           [dead]alternative: https://forums.justlinux.com/showthread.php?135548-SOLVED-Unbuffered-input-without-ncurses
//           [dead]alternative: http://www.justlinux.com/forum/showthread.php?s=&threadid=45316
//           []TODO: check which system-specific apis ncurses uses
//           []last try: https://man7.org/linux/man-pages/man3/termios.3.html
//                in non-canonical mode, the equivalent to getch:
//                MIN > 0, TIME == 0 (blocking read)
//                    read(2) blocks until MIN bytes are available, and returns
//                    up to the number of bytes requested.
//               example "raw" mode:
//               cfmakeraw() sets the terminal to something like the "raw" mode of
//               the old Version 7 terminal driver: input is available character
//               by character, echoing is disabled, and all special processing of
//               terminal input and output characters is disabled.  The terminal
//               attributes are set as follows:
//                   termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
//                                   | INLCR | IGNCR | ICRNL | IXON);
//                   termios_p->c_oflag &= ~OPOST;
//                   termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
//                   termios_p->c_cflag &= ~(CSIZE | PARENB);
//                   termios_p->c_cflag |= CS8;
// struct termios {
//   tcflag_t c_iflag;    /* input specific flags (bitmask) */
//   tcflag_t c_oflag;    /* output specific flags (bitmask) */
//   tcflag_t c_cflag;    /* control flags (bitmask) */
//   tcflag_t c_lflag;    /* local flags (bitmask) */
//   cc_t     c_cc[NCCS]; /* special characters */
// https://en.wikibooks.org/wiki/Serial_Programming/termios
// There are more than 45 different flags that can be set (via tcsetattr()) or got (via tcgetattr()) with the help of the struct termios. The large number of flags, and their sometimes esoteric and pathologic meaning and behavior, is one of the reasons why serial programming under Unix can be hard. In the device configuration, one must be careful not to make a mistake.
// ^good examples in the link
// };

#endif // __UTILS_H__
