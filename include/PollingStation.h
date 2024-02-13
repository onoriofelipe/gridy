#ifndef __POLLING_STATION_H__
#define __POLLING_STATION_H__

#include <vector>
#include "Enums.h"
#include "Things.h"

class PollingStation {
public:
   ///[]TODO: some methods to get the entities... maybe entity manager?
   ///        for now they are the same, separate them later
   std::vector<std::shared_ptr<Thing>> entities{};

   // queue for polling? avoid it for now just to decrease boilerplate
   std::vector<std::shared_ptr<IComponent>> get_components_from_entity(ID id, const std::vector<ComponentType> requirements){
      ///[]TODO: avoid abusing heap during runtime
      std::vector<std::shared_ptr<IComponent>> components{requirements.size(), nullptr};
      
      return components;
   }
};

#endif // __POLLING_STATION_H__