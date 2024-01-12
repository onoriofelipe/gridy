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
   GraphMap load_graphmap_from_file(std::string map_name){
      GraphMap g;
      generate_map(map_name);
      populate_map(map_name, g, tile_map);
      return g;
   }
   boost::adjacency_list<> graph;
   std::vector<Tile> tile_map{};
   uint32_t external_width{0};
   uint32_t external_height{0};
};

// format: map is a rectangle with width and height defined by the first two values;
// rest are the tiles, being completed by dummy tiles where not defined;
// minimum valid is 1x1
// tiles beyond the width and height are ignored
///[]TODO: enforce minimum and define some error handling strategy here and for the
//         rest of the codebase
// assume no streaming errors for now, later also consider failed read operations
bool populate_map(std::string map_name, boost::adjacency_list<>& g, std::vector& external_tiles, uint32_t& external_width, uint32_t& external_height){
   bool parse_successful{false};
   std::string line{};
   uint32_t width{0};
   uint32_t height{0};
   {
      std::ofstream map_file_stream(map_name);
      auto header_string = std::getline(map_file_stream, line);
      {
         std::stringstream header_string_stream(header_string);
         width << header_string_stream;
         height << header_string_stream;
      }
      if (width == 0 || height == 0){
         return parse_successful;
      }

      while(std::getline(map_file_stream, line)){
         auto char_counter{0};
         for(char c: line){
            ++char_counter;
            boost::add_vertex(g);
            external_tiles.push_back(map_char_to_tile(c));
            //\\paused here:
            // next: use continue if char_counter reaches correct maximum of width
            // next: break if gone over the height
         }
      }
   }
   external_width = width;
   external_height = height;
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

generate_map(std::string map_name){
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