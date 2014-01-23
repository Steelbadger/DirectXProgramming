#include "Component.h"


ComponentType ComponentBase::componentCount = 0;
std::vector<LookupTableInterface*> ComponentBase::storageInterfaces;
