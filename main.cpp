#include <iostream>
#include <boost/signals2.hpp>
#include <array>
#include <cstdint>

using uchar = unsigned char;

/**
 *  AsciiScreen represents a character buffer for manipulation and printing
 *  to a character terminal with height H and width W, plus one extra column
 *  for newline characters and one extra ending null character
 */

// valid for H >= 1, W >= 1
/// []TODO: try concepts for enforcing valid ranges
template <uint64_t H, uint64_t W>
class AsciiScreen{
public:
    AsciiScreen(){
        for(auto i = 0; i < H*(W+1); ++i){
            buffer[i] = '~';
        }
        for(auto j = 0; j < H; ++j){
            buffer[j*(W+1) + W] = '\n';
        }
        buffer[H*(W+1)] = '\0';
    }
    std::array<uchar, H * (W + 1) + 1> buffer;
    ///[]TODO: rewrite api using ranges/view syntax
    uchar& pixel_ref(uint32_t column, uint32_t line){
        return buffer[line * (W+1) + column ];
    }
    void write_borders(uchar character){
        write_vertical(0, character);
        write_vertical(W-1, character);
        write_horizontal(0, character);
        write_horizontal(H-1, character);
        write_corners(character);
    }
    void write_borders(){
        write_vertical(0, '|');
        write_vertical(W-1, '|');
        write_horizontal(0, '-');
        write_horizontal(H-1, '-');
        write_corners('+');
    }
    // void write(uint32_t column, uint32_t line, uchar character){
    //     buffer[column + line * H] = character;
    // }
    // write a horizontal line of characters
    void write_horizontal(uint32_t line, uchar character){
        for(auto i = 0; i < W; ++i){
            buffer[line * (W+1) + i] = character;
        }
    }
    // write a vertical line of characters
    void write_vertical(uint32_t column, uchar character){
        for(auto j = 0; j < H; ++j){
            buffer[j * (W+1) + column] = character;
        }
    }
    // write a corners with characters
    void write_corners(uchar character){
        buffer[0] = character;
        buffer[W-1] = character;
        buffer[(W+1)*(H-1) + 0] = character;
        buffer[(W+1)*(H-1) + W-1] = character;
        // buffer[line * (W+1) + i] = character;
        // buffer[line * (W+1) + i] = character;
        // buffer[line * (W+1) + i] = character;
    }
    // void write_vertical(uint32_t column, uchar character){
    //     for()
    // }
    // template <typename F>
    // void for_pixel(uint32_t column, uint32_t line, const F& f){
    //     buffer
    // }
    // template <typename F>
    // void for_line(){}
    // template <typename F>
    // void for_column(){}
    // template <typename F>
    void stdout_print(){
        const char * const c_string = reinterpret_cast<const char* const>( buffer.data() );
        std::cout << c_string << std::endl;
    }
};

int main(){
    // boost::signals2::signal<void (int a)> emitter;
    // auto lambda = [](int x){ std::cout << x << std::endl; };
    // emitter.connect(lambda);
    // emitter(23);
    // emitter(14);
    auto screen = AsciiScreen<20,80>{};
    screen.write_horizontal(3, '\\');
    screen.write_vertical(6, '/');
    screen.write_borders();
    screen.stdout_print();
}
