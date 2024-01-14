#ifndef __GRAPHMAP_H__
#define __GRAPHMAP_H__

#include <fstream>
#include <string>
#include <sstream>
#include <boost/graph/adjacency_list.hpp>
#include "PositionPrimitives.h"

enum class TileType {
   Wall = 0, // dummy tiles by default
   Floor,
};

struct Tile {
   Tile(TileType type = TileType{}): type{type} {}
   TileType type{};
   // additionally, any other interesting properties
};

class GraphMap {
public:
   // given a position and a direction, what should be the new position?
   ///[]TODO: make this either more simplistic or more convoluted
   Position request_move(Position old_position, Direction requested_direction){
      auto resulting_position = old_position;
      // consider other filtering criteria;
      // for now it's enough to either go through a neighbor
      // or get denied due to no connection; should be simple too since the graph
      // of dummy tiles is never connected? maybe the more general approach should be
      // considered, where the interaction with a new tile may have side-effects;
      auto requested_position = old_position + requested_direction;
      if(is_neighbor(old_position, requested_position)){
         resulting_position = requested_position;
      }
      return resulting_position;
   }
   bool is_neighbor(Position a, Position b){
      // do boost magic
   }
   int32_t index_from_position(Position p){
      int32_t index{0};
      // 0  1  2  3   // 0,0 1,0 2,0 3,0
      // 4  5  6  7   // 0,1 1,1 2,1 3,1
      // 8  9  10 11  // 0,2 1,2 2,2 3,2
      index = p.x + width * p.y;
      return index;
   }
   Position position_from_index(int32_t index){
      // for(auto i: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}){
      //    std::cout << i << " % 4: " << static_cast<int>(i % 4) << std::endl;
      //    std::cout << i << " / 4: " << static_cast<int>(i / 4) << std::endl << std::endl;
      // }
      Position position{0, 0};
      position.x = index % width;
      position.y = index / width;
      return position;
   }
   bool load_graphmap_from_file(std::string map_name){
      bool result_valid{false};
      generate_map(map_name);
      result_valid = populate_map(map_name);
      connect_neighbors();
      return result_valid;
   }
   void connect_neighbors();
   bool populate_map(std::string map_name);
   boost::adjacency_list<> graph;
   std::vector<Tile> tile_map{};
   std::vector<int32_t> tile_indexes{};
   uint32_t width{0};
   uint32_t height{0};
};

std::array<Position> neighbors_of()

void GraphMap::connect_neighbors(){
   // D D D F D D
   // D F F F F D
   // D D F F D D
   // D D D F D D
   for(const auto& tile: tile_map){
      
   }
}

// format: map is a rectangle with width and height defined by the first two values;
// rest are the tiles, being completed by dummy tiles where not defined;
// minimum valid is 1x1
// tiles beyond the width and height are ignored
///[]TODO: enforce minimum and define some error handling strategy here and for the
//         rest of the codebase
// assume no streaming errors for now, later also consider failed read operations
bool GraphMap::populate_map(std::string map_name/*, boost::adjacency_list<>& g, std::vector& external_tiles, uint32_t& external_width, uint32_t& external_height*/){
   bool parse_successful{true};
   bool parse_unsuccessful{false};
   std::string line{};
   {
      std::ofstream map_file_stream(map_name);
      auto header_string = std::getline(map_file_stream, line);
      {
         std::stringstream header_string_stream(header_string);
         width << header_string_stream;
         height << header_string_stream;
      }
      if (width == 0 || height == 0){
         return parse_unsuccessful;
      }
      uint32_t line_counter{0};
      int32_t index_counter{0};
      while(std::getline(map_file_stream, line)){
         // ignore tiles beyong specified height
         if (line_counter >= height){
            break;
         }
         uint32_t char_counter{0};
         for(char c: line){
            // ignore tiles beyond specified width
            if(char_counter >= width){
               break;
            }
            // char_counter
            // 0 1 2 3 4 5
            // width
            // 3
            boost::add_vertex(g);
            external_tiles.push_back(map_char_to_tile(c));
            tile_indexes.push_back(index_counter);
            ++index_counter;
            ++char_counter;
         }
         ++line_counter;
      }
   }
   return parse_successful;
}

Tile map_char_to_tile(char c){
   Tile tile{};
   switch(c){
      case 'F':
         tile = Tile{};
      case 'D':
      default:
         // already initialized to default, no redundancy
         break;
   }
   return tile;
}

void generate_map(std::string map_name){
   {
      ///[]TODO: move hardcoded maps someplace else, then update cmake
      ///[]TODO: move map generation for someplace else, make intricate procedural generation
      //         with maze algorithms and perlin noise and voronoy noise, research how to
      //         use noise for housing complex generation or anything with such arbitrary
      //         geometric requirements
      std::ifstream artificial_map(map_name);
      // 60 width 30 height map
      artificial_map << std::string{"
                        DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n
                        DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n
                        "};
   }
}

#endif // __GRAPHMAP_H__