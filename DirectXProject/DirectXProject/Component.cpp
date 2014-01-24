#include "Component.h"



ComponentType ComponentBase::componentCount = 0;
std::vector<LookupTableInterface*>* ComponentBase::storageInterfaces = NULL;



ComponentBase::ComponentBase()
{
	if (storageInterfaces == NULL) {
		storageInterfaces = new std::vector<LookupTableInterface*>;
	}
}

ComponentBase::~ComponentBase()
{

}

ComponentType ComponentBase::GetNumberOfComponents()
{
	return componentCount;
}

LookupTableInterface* ComponentBase::GetComponentStorage(ComponentType componentTypeID)
{
	return (storageInterfaces->size() >= componentTypeID ? (*storageInterfaces)[componentTypeID] : NULL);
}

void ComponentBase::SetComponentStorage(ComponentType componentTypeID, LookupTableInterface* dataInterface)
{
	(*storageInterfaces)[componentTypeID] = dataInterface;
}

ComponentType ComponentBase::NewComponentType(LookupTableInterface* data)
{
		ComponentType output = componentCount++;
		storageInterfaces->push_back(data);
		return output;
}