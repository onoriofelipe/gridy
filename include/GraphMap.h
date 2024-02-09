#ifndef __GRAPHMAP_H__
#define __GRAPHMAP_H__

#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <boost/graph/adjacency_list.hpp>
#include <boost/signals2.hpp>
#include "PositionPrimitives.h"
#include "ActionHandler.h"
#include "Utils.h"

// using graph_neighbor_requester_t = boost::signals2::signal<Position(Action, Position, Direction)>;

///[]TODO: add an empty dummy tile, instead of using wall as the dummy since
//         walls can have way more funcionality than a simply dummy tile
enum class TileType {
   Wall = 0, // dummy tiles by default
   Floor,
};

struct Tile {
   Tile(TileType type = TileType{}): type{type} {}
   TileType type{};
   // additionally, any other interesting properties
};
inline std::ostream& operator<<(std::ostream& os, const Tile& tile){
   os << static_cast<int>(tile.type);
   return os;
}

void generate_map(std::string map_name);
Tile map_char_to_tile(char c);

class GraphMap {
public:
   GraphMap()
   {
      register_handlers();
   }
   void register_handlers(){
      neighbor_request_handler.register_action_handler(Action::RequestConnectedNeighbor, [this](Position p, Direction d) -> Position {
         return request_neighbor_move(p, d);
      });
   }
   ActionHandler<Position, Position, Direction> neighbor_request_handler;
   // given a position and a direction, what should be the new position?
   ///[]TODO: make this either more simplistic or more convoluted
   Position request_neighbor_move(Position old_position, Direction requested_direction){
      auto resulting_position = old_position;
      // consider other filtering criteria;
      // for now it's enough to either go through a neighbor
      // or get denied due to no connection; should be simple too since the graph
      // of dummy tiles is never connected? maybe the more general approach should be
      // considered, where the interaction with a new tile may have side-effects;
      auto requested_position = old_position + requested_direction;
      if(is_connected_neighbor(old_position, requested_position)){
         resulting_position = requested_position;
         ///[]TODO: debug in godbolt minimal graph neighbors for undirected graph
         // std::cout << "neighbors are connected!" << std::endl;
      }
      return resulting_position;
   }
   ///[]TODO: implement lookup for  all things based on position,
   ///        for now; for this, registration for every thing
   ///        is probably needed
   ///[]TODO: also, consider how feasible it is to separate into
   ///        some other possible component or system
   bool is_connected_neighbor(Position a, Position b){
      bool is_neighbor{false};
      // do boost magic
      auto tile_a_index = index_from_position(a);
      auto tile_b_index = index_from_position(b);
      is_neighbor = boost::edge(tile_a_index, tile_b_index, graph).second; // nigh unreadable implementation in boost
      return is_neighbor;
   }
   std::vector<int32_t> neighbors_of(Position tile_position);

   bool valid_tile_range(Position p){
      return ( p.x >= 0 && p.x < static_cast<int32_t>(width) && p.y >= 0 && p.y < static_cast<int32_t>(height) );
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
      // cerrprint_graph_and_indices();
      return result_valid;
   }
   void connect_neighbors();
   bool populate_map(std::string map_name);
   void cerrprint_graph_and_indices();
   boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS> graph;
   std::vector<Tile> tile_map{};
   std::vector<int32_t> tile_indexes{};
   uint32_t width{0};
   uint32_t height{0};
};

std::vector<int32_t> GraphMap::neighbors_of(Position tile_position){
   static std::vector<Direction> neighbor_directions{ D::N, D::NE, D::E, D::SE,
                                                      D::S, D::SW, D::W, D::NW};
   std::vector<int32_t> neighbor_indexes;
   bool valid_range{false};
   Position neighbor_position{0, 0};

   for(const auto& direction: neighbor_directions){
      neighbor_position = tile_position + direction;
      valid_range = valid_tile_range(neighbor_position);
      if(valid_range){
         neighbor_indexes.push_back(index_from_position(neighbor_position));
      }
   }
   return neighbor_indexes;
}

void GraphMap::connect_neighbors(){
   // D D D F D D
   // D F F F F D
   // D D F F D D
   // D D D F D D
   for(const auto& tile_index: tile_indexes){
      if(tile_map[tile_index].type == TileType::Wall /* || or other criteria for not connecting to anything */){
         continue;
      }
      auto neighbor_indexes = neighbors_of(position_from_index(tile_index));
      // assume that central tile should be connected unless neighbor restricts it
      for (const auto& neighbor_index: neighbor_indexes){
         if(tile_map[neighbor_index].type == TileType::Floor){
            // create connection using boost indices
            boost::add_edge(tile_index, neighbor_index, graph);
         }
      }
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
   std::string header_string{};
   {
      std::ifstream map_file_stream(map_name);
      std::getline(map_file_stream, header_string);
      {
         std::stringstream header_string_stream(header_string);
         header_string_stream >> height;
         header_string_stream >> width;
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
            boost::add_vertex(graph);
            tile_map.push_back(map_char_to_tile(c));
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
         tile = Tile{TileType::Floor};
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
      std::ofstream artificial_map(map_name);
      // 60 width 30 height map
      artificial_map << std::string{
         "30 60\n" \
         "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD\n" \
         "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n"
      };
   }
}

void GraphMap::cerrprint_graph_and_indices(){
   // boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS> graph;
   // std::vector<Tile> tile_map{};
   // std::vector<int32_t> tile_indexes{};
   std::cerr << "tile_map: " << std::endl;
   std::cerr << tile_map << std::endl;
   std::cerr << "tile_indexes: " << std::endl;
   std::cerr << tile_indexes << std::endl;

   // std::pair<boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS>::vertex_iterator,
   // boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS>::vertex_iterator> vs = boost::vertices(graph);
   // std::copy(vs.first, vs.second,
   // std::ostream_iterator<boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS>::vertex_descriptor>{
   //    std::cerr, ","});

   std::cerr << "boost::graph indexes: " << std::endl;
   auto vs = boost::vertices(graph);
   std::copy(vs.first, vs.second,
   std::ostream_iterator<boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS>::vertex_descriptor>{
      std::cerr, ","});
}

#endif // __GRAPHMAP_H__
