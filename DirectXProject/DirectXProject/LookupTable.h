#pragma once

#include <vector>

typedef int ObjectID;

 
template<class T, int MaxElements = 128>
class LookupTable {
private:
	struct Storage {
        Storage():used(false){}
        Storage(T& data):element(data), used(true){}
        T element;
        bool used;        
    };

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

	T& Get(ObjectID object) {
		if (Exists(object)) {
			return storage[object].element;
		} else {
			return T();
		}
	}

private:
    Storage storage[MaxElements];
    ObjectID spacesStack[MaxElements]; 
    unsigned int stackpointer;
};

template<class T, int InitialSize = 128>
class DynamicLookupTable {
private:
	struct Storage {
        Storage():used(false){}
        Storage(T& data):element(data), used(true){}
        T element;
        bool used;        
    };


public:
    DynamicLookupTable() {
		storage.reserve(InitialSize);
		spacesStack.reserve(InitialSize);
        for (int i = 0; i < InitialSize; i++) {
			spacesStack.push_back((InitialSize-1)-i);
			storage.push_back(Storage());
        }
    }
    
    ObjectID Add(T& object){
		ObjectID outID;
		if (spacesStack.size() > 0) {
			outID = spacesStack.back();
			storage[outID] = Storage(object);
			spacesStack.pop_back();
		} else {
			outID = storage.size();
			storage.push_back(Storage(object));
		}
		return outID;
    }
    
    void Remove(ObjectID object) {
		if (Exists(object)) {
			spacesStack.push_back(object);
			storage[object].used = false;
		}
    }
    
    bool Exists(ObjectID object){
		if (object >= 0 && object < storage.size()) {
			return storage[object].used;
		} else {
			return false;
		}
    }

	T& Get(ObjectID object) {
		if (Exists(object)) {
			return storage[object].element;
		} else {
			return T();
		}
	}

	int Size() {
		return storage.size();
	}

private:

    std::vector<Storage> storage;
    std::vector<ObjectID> spacesStack; 
};