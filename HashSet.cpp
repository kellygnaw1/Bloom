#include "HashSet.h"
#include "IntegerHashes.h"
#include "StringHashes.h"

HashSet::HashSet(){
	nitems = 0; 
	nslots = 113; 
	intfn = new SquareRootHash(1, nslots);
	strfn = new JenkinsHash();
	strfn2 = NULL;
	slots = new std::string*[nslots]();
	for(int i = 0; i < nslots; i++){
		slots[i] = NULL;
	}

}

HashSet::~HashSet(){
	delete strfn;
	delete intfn; 
	for(int i = 0; i < nslots; i++)
		delete slots[i];
	delete[] slots;

}

void HashSet::insert(const std::string& value){
	uint64_t index = intfn -> hash(strfn -> hash(value));
	while(slots[index] != nullptr){
		index ++;
		index %= nslots;
	}
	slots[index] = new std::string(value); 
	nitems ++; 
	if(static_cast<double>(nitems)/nslots > 0.6)
		rehash();
}

bool HashSet::lookup(const std::string& value) const{
	uint64_t index = intfn -> hash(strfn -> hash(value));
	while(slots[index] != nullptr){
		if(*(slots[index]) == value)
			return true;
		index++;
		index %= nslots;
	}
	return false; 
}

void HashSet::rehash(){
	int old_nSlots = nslots;
	nslots = nslots*2;
	std::string** temp = slots; 
	slots = new std::string*[nslots]();
	delete intfn; 
	intfn = new SquareRootHash(1, nslots);
	for(int i = 0; i < old_nSlots; i++){
		if(temp[i] != NULL){
			uint64_t newIndex = intfn -> hash(strfn -> hash(*(temp[i])));
			while(slots[newIndex] != nullptr){
				newIndex++;
				newIndex %= nslots;
			}
			slots[newIndex] = temp[i];
		}
	}
	delete [] temp;
}

