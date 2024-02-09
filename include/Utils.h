#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <fstream>

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
      // std::cerr << "_kbhit: true" << std::endl;
      result = _getch_nolock();
      if (result == 0 || result == 0xE0 ){
         // std::cerr << "result == 0 || result = 0xE0, getting extra byte" << std::endl;
         result = _getch_nolock();
      }
   } else {
      // std::cerr << "_kbhit: false" << std::endl;
   }
   // std::cerr << "int from _getch: " << result << std::endl;
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
   result = getchar();
   return result;
}

}; // ; is not needed after namespaces (;

#else
#error Platform not supported
#endif

void redirect_error_stream_to_file(){
   // std::streambuf* old = std::cerr.rdbuf();

   // std::ofstream output_stream("logs.txt");
   // assume will never fail :D
   // std::cerr.rdbuf(output_stream.rdbuf());
   // never restore the original state! `-´
}

template <typename E>
std::ostream& operator<<(std::ostream& os, const std::vector<E>& elements){
   bool first_pass{true};
   for(const E& element: elements){
      if(!first_pass){
         os << ",";
      }
      os << element;
      first_pass = false;
   }
   return os;
}


#endif // __UTILS_H__
