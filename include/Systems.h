#ifndef __SYSTEMS_H__
#define __SYSTEMS_H__

#include <algorithm> // std::clamp
#include <limits> // std::numeric_limits

///[]TODO: redefine behavior that is specified by groups of components as a system:
//         for example drawing should be defined as the behavior done by the collection
//         of screen + drawable components + required data such as position and representation;
//         each could (should) be a separate component
//
//       []this refactor should be done after some experiments have been performed with
//         boost::signals, after the mechanism has been tried and fitted and properly
//         examined in terms of boilerplate required vs advantages gained, comparing
//         a few different approaches

uint32_t uniform_rand(uint32_t basic){
   // auto mt = std::mersenne_twister_thingy
   return 1*basic; // mt() * basic;
}
// centralizes damage formulas and possibly other combat side-effects?
class CombatMechanics {
public:
   ///[]TODO: listen for attack events and other spell events;

   uint32_t calculate_attack_damage(const Thing& agressor, const Thing& defender){
      return std::clamp(
         uniform_rand(agressor.attributes.attack)
         - uniform_rand(agressor.attributes.defense),
         static_cast<uint32_t>(0),
         std::numeric_limits<uint32_t>::max());
   }

   // for events related to world interaction, such as side-effects of spells
   // like destroying terrain, and maybe reducing health of entities; these are
   // orders, always obeyed; thus every entity listens to combat mechanics,
   // graphmap included as well
   //boost::signals2::signal<?(?)> ? output_emitter;
};

///[]TODO: keep internal redundant data about all the entities located in the map;
//         when asked to move something, always update it;
//         this is the initial plan for lookups based on position required
//         for retrieving affected targets in a given area, or for interactions
//         specially for non-targetable things (hidden walls/doors/traps/
//         invisible monsters?)
//         also, it should be guaranteed to work since on spawn things will be
//         registered and have their position updated

///[]TODO: movement actions are sent to the aether;
//         the mapgraph will subscribe to those events

///[]TODO: use boost bgl for graphs?
// connected tiles list
class MapGraph {
public:
   ///[]TODO: read from file or from preset
   ///[]TODO: initially, only ground tiles and wall tiles
   // bool try_walk(from, to);
   // path_plan a_star_path(from, to);
   // ids get_ids_in_area();
};

#endif // __SYSTEMS_H__