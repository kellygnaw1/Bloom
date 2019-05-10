#include "BloomFilter.h"
#include "IntegerHashes.h"
#include "StringHashes.h"

BloomFilter::BloomFilter(int k , int m, std::string strfn, std::string intfn){
	this->k = k;
	this->m = m;
	bits = new uint64_t[m]();

	if(strfn == "jenkins")
		this->strfn = new JenkinsHash();
	else if(strfn == "pearson")
		this->strfn = new PearsonHash();

	this->intfns = new IntegerHash*[k];
	for(int i = 0; i < k; i++){
		if(intfn == "division")
			intfns[i] = new DivisionHash(i , m);
		else if(intfn == "reciprocal")
			intfns[i] = new ReciprocalHash(i, m);
		else if(intfn == "squareroot")
			intfns[i] = new SquareRootHash(i, m);
	}
	
}

BloomFilter::~BloomFilter(){
	delete [] bits;
	delete strfn;
	for(int i = 0; i < k; i++)
		delete intfns[i];
	delete [] intfns;
}

void BloomFilter::insert(const std::string& value){
	for(int i = 0; i < k; i++){
		uint64_t key = intfns[i] -> hash(strfn->hash(value));
		bits[key/64] |= (uint64_t(1) << (key%64));
	}
}

bool BloomFilter::lookup(const std::string& value) const{
	for(int i = 0; i < k; i++){
		uint64_t key = intfns[i] -> hash(strfn->hash(value));
		if(((uint64_t(1) << (key%64)) & (bits[key/64])) == 0)
			return false; 
	}
	return true;
}

