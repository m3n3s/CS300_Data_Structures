#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <vector>

using namespace std;

template <class Key, class Value>
class HashEntry
{
	public:
		Key element;
		Value data;
		string info;

	    HashEntry(const Key & e = Key(), const Value & d = Value(), string i = "EMPTY")
			: element(e), data(d), info(i){}
};


template <class Key, class Value>
class HashTable
{
	public:
		explicit HashTable(const Key & notFound, int size = 53);
        HashTable(const HashTable & rhs)
			: ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), myArray(rhs.myArray), currentSize(rhs.currentSize){}

        HashEntry<Key, Value>* find(const Key & x);
        void insert(const Key & x, Value data);
        void remove(const Key & x);

		double getLoadFactor() {return double(currentSize) / myArray.size();}
		int getArraySize() {return myArray.size();}
		int getUniqueWordCount() {return currentSize;}

	private:            
		vector<HashEntry<Key, Value>> myArray;
		unsigned int currentSize; // shows how many elements are in the hash table
		const Key ITEM_NOT_FOUND;

		bool isActive(int currentPos) const;
		int findPos(const Key & x) const;
		void rehash();
		int nextPrime(int n);
		bool isPrime(int n);
 };

unsigned int myHash (const string & key, int tableSize);

#endif