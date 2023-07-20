#include "HashTable.h"

using namespace std;

template <class Key, class Value>
HashTable<Key, Value>::HashTable(const Key & notFound, int size)
	: ITEM_NOT_FOUND(notFound), myArray(nextPrime(size))
{
	currentSize = 0;
}

template <class Key, class Value>
bool HashTable<Key, Value>::isActive(int currentPos) const
{
		return myArray[currentPos].info == "ACTIVE";
}

template <class Key, class Value>
void HashTable<Key, Value>::remove(const Key & x)
{
	int currentPos = findPos(x);
    if(isActive(currentPos))
		myArray[currentPos].info = "DELETED";
}

template <class Key, class Value>
int HashTable<Key, Value>::findPos(const Key & x) const
{
	// quadratic probing resolution
	int collisionNum = 0;
    unsigned int currentPos = myHash(x, myArray.size());

    while(myArray[currentPos].info != "EMPTY" && myArray[currentPos].element != x)
	{
		currentPos += 2 * ++collisionNum - 1; // add the difference
        if(currentPos >= myArray.size()) // perform the mod
			currentPos -= myArray.size(); // if necessary
	}

	return currentPos;
}

template <class Key, class Value>
HashEntry<Key, Value>* HashTable<Key, Value>::find(const Key & x)
{
	int currentPos = findPos(x);
    if(isActive(currentPos)) 
		return &myArray[currentPos]; 
 
	return nullptr;
}

template <class Key, class Value>
void HashTable<Key, Value>::insert(const Key & x, Value data)
{
	// Insert x as active
    int currentPos = findPos(x);
    if(isActive(currentPos))
		return;
    myArray[currentPos] = HashEntry<Key, Value>(x, data, "ACTIVE");
	currentSize++;

    // enlarge the hash table if necessary 
	if(currentSize >= myArray.size() / 2)
		rehash();
}

template <class Key, class Value>
void HashTable<Key, Value>::rehash()
{
	vector<HashEntry<Key, Value>> oldArray = myArray;

    // Create new double-sized, empty table
    myArray.resize(nextPrime(2 * oldArray.size()));
    for(unsigned int j = 0; j < myArray.size(); j++)
		myArray[j].info = "EMPTY";

    // Copy table over
    currentSize = 0;
    for(unsigned int i = 0; i < oldArray.size(); i++)
		if(oldArray[i].info == "ACTIVE")
			insert(oldArray[i].element, oldArray[i].data);

	double load = double(currentSize) / myArray.size();
}

template <class Key, class Value>
int HashTable<Key, Value>::nextPrime(int n)
{
	if(n % 2 == 0)
		n++;

    for( ; !isPrime(n); n += 2)
		;

    return n;
}

template <class Key, class Value>
bool HashTable<Key, Value>::isPrime(int n)
{
	if(n == 2 || n == 3)
		return true;

	if(n == 1 || n % 2 == 0)
		return false;
	
	for(int i = 3; i * i <= n; i += 2)
		if(n % i == 0)
			return false;

	return true;
}