#pragma once

#include <vector>

typedef unsigned int ObjectID;

 
template<class T, int MaxElements = 128>
class LookupTable {
public:
    LookupTable() {
        for (int i = 0; i < MaxElements; i++) {
            spacesStack[i] = i;
			storage[i] = Storage();
        }
        stackPointer = 0;
    }
    
    ObjectID Add(T& object){
        ObjectID outID = spacesStack[stackPointer++];
        storage[outID] = Storage(object);
        return outID;
    }
    
    void Remove(ObjectID object) {
		if (Exists(object)) {
			spacesStack[--stackPointer] = object;
			storage[object].used = false;
		}
    }
    
    bool Exists(ObjectID object){
		if (object >= 0 && object < MaxElements) {
			return storage[object].used;
		} else {
			return false;
		}
    }

	T& GetObject(ObjectID object) {
		if (Exists(object)) {
			return storage[object];
		} else {
			return NULL;
		}
	}

private:
    struct Storage {
        Storage():used(false){}
        Storage(T& data):element(data), used(true){}
        T element;
        bool used;        
    };
 
    Storage storage[MaxElements];
    ObjectID spacesStack[MaxElements]; 
    unsigned int stackpointer;
};

template<class T, int InitialSize = 128>
class DynamicLookupTable {
public:
    DynamicLookupTable() {
		storage.reserve(InitialSize);
		spacesStack.reserve(InitialSize);
        for (int i = 0; i < InitialSize; i++) {
			spacesStack.push_back(InitialSize-i);
			storage[i] = Storage();
        }
    }
    
    ObjectID Add(T& object){
        ObjectID outID = spacesStack.back();
        storage[outID] = Storage(object);
		spacesStack.pop_back();
        return outID;
    }
    
    void Remove(ObjectID object) {
		if (Exists(object)) {
			spacesStack.push_back(object);
			storage[object].used = false;
		}
    }
    
    bool Exists(ObjectID object){
		if (object >= 0 && object < MaxElements) {
			return storage[object].used;
		} else {
			return false;
		}
    }

	T& GetObject(ObjectID object) {
		if (Exists(object)) {
			return storage[object];
		} else {
			return NULL;
		}
	}

private:
    struct Storage {
        Storage():used(false){}
        Storage(T& data):element(data), used(true){}
        T element;
        bool used;        
    };
 
    std::vector<Storage> storage;
    std::vector<ObjectID> spacesStack; 
};