#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include "AVLSearchTree.cpp"
#include "HashTable.cpp"
#include <vector>

using namespace std;

// MEHMET ENES BATTAL - HW4

// NOTE: It takes long to process the files and timing after query.
// It may take a while especially with large number of files but it gives the result eventually.

struct DocumentItem 
{
	string documentName;
	int count;

	DocumentItem(string n = "", int c = 0)
		:documentName(n), count(c){}
};

struct WordItem 
{
	string word;
	vector<DocumentItem> list;

	WordItem(string element = "", vector<DocumentItem> v = vector<DocumentItem>()):
		word(element), list(v){}
};

unsigned int myHash(const string & key, int tableSize)
{ 
    unsigned int hashVal = 0;
  
    for (unsigned int i = 0; i < key.length();i++) 
	  hashVal = 37 * hashVal + key[i];

    hashVal = hashVal % tableSize;

    if (hashVal < 0) 
	  hashVal = hashVal + tableSize;

   return hashVal;
}

void deleteDynamicMemoryTree(AVLNode<string, WordItem*>* node)
{
	// Deletes the dynamically allocated memory of the tree which is inside the nodes

	if (node == NULL) 
        return; 
  
    /* first recur on left child */
    deleteDynamicMemoryTree(node->left); 
  
    /* now recur on right child */
    deleteDynamicMemoryTree(node->right); 

	// deletion
	delete node->data;
}

void toLower(string & q)
{
	// changes all the letters of the word to lowercase.

	string lower = "";
	for(unsigned int i = 0; i < q.length(); i++)
	{
		if(q[i] >= 'A' && q[i] <= 'Z')
			lower += q[i] + 32;
		
		else	
			lower += q[i];
	}
	q = lower;
}

void changeChar(string & s)
{
	// changes non-alphabetical charcters to space (" ")
	for(unsigned int i = 0; i < s.length(); i++)
	{
		if(s[i] < 'A' ||s[i] > 'z')
			s[i] = ' ';
	}
}

void newNodeData(AVLNode<string, WordItem*>* node, string file)
{
	// edits the data of the existing node

	vector<DocumentItem> &p = node->data->list;

	for(unsigned int i = 0; i < p.size(); i++)
	{
		if(p[i].documentName == file) // in an existing file, increase count
		{
			p[i].count++;
			return;
		}
	}

	// this file doesn't exist, add new DocumentItem
	p.push_back(DocumentItem(file, 1));
}

void newEntryData(HashEntry<string, WordItem*>* entry, string file)
{
	// edits the data of the existing node

	vector<DocumentItem> &p = entry->data->list;

	for(unsigned int i = 0; i < p.size(); i++)
	{
		if(p[i].documentName == file)// in an existing file, increase count
		{
			p[i].count++;
			return;
		}
	}

	// this file doesn't exist, add new DocumntItem
	p.push_back(DocumentItem(file, 1));
}

void newVectorData(WordItem & item, string file)
{
	for(unsigned int i = 0; i < item.list.size(); i++)
	{
		if(item.list[i].documentName == file)// in an existing file, increase count
		{
			item.list[i].count++;
			return;
		}
	}

	// this file doesn't exist, add new DocumntItem
	item.list.push_back(DocumentItem(file, 1));
}

int findInVector(const vector<WordItem> & v, string w)
{
	for(int i = 0; i < v.size(); i++)
	{
		if(v[i].word == w)
			return i;
	}

	return -1;
}

int binarySearch(const vector<WordItem> & v, int l, int r, string x) 
{ 
  while (l <= r) 
  { 
    int m = l + (r-l)/2; 

	if (v[m].word == x)  
        return m;   
  
	if (v[m].word < x)  
        l = m + 1;  
  
    else 
         r = m - 1;  
  } 
  
  // if we reach here, then element was not present 
  return -1;  
}

void preprocessFile(AVLSearchTree <string, WordItem *> & tree, HashTable <string, WordItem *> & table,
					vector<WordItem> & v, const string & file)
{
	// goes over every word and creates the tree structure and the hash table

	ifstream input;
	string line;

	input.open(file);

	while(getline(input, line))
	{
		// change ignored charachters to space
		changeChar(line);

		istringstream ss(line);
		string word;

		while(ss >> word)
		{
			toLower(word);

			// ADD WORD TO THE TREE
			AVLNode<string, WordItem*>* temp;
			temp = tree.find(word);

			if(temp == nullptr) // not in tree, add new node
			{
				DocumentItem item(file, 1);
				WordItem* element = new WordItem(word);
				element->list.push_back(item);

				tree.insert(word, element);
			}

			else
			{
				// in tree, edit existing node
				newNodeData(temp, file);
			}

			// ADD WORD TO THE HASH TABLE
			HashEntry<string, WordItem*>* t;
			t = table.find(word);

			if(t == nullptr) // not in the table, add new entry
			{
				DocumentItem item = DocumentItem(file, 1);
				WordItem* element = new WordItem(word);
				element->list.push_back(item);

				table.insert(word, element);
			}

			else 
			{
				// in the table edit existing entry
				newEntryData(t, file);
			}

			// ADD WORD TO VECTOR
			int idx = findInVector(v, word);

			if(idx == -1) // not in vector
			{
				DocumentItem item(file, 1);
				WordItem element(word);
				element.list.push_back(item);

				v.push_back(element);
			}

			else
			{
				// in the vector edit existing element
				newVectorData(v[idx], file);
			}
		}
	}
}

int inFile(vector<DocumentItem> t, const string & f)
{
	// returns how many times the word is found in given file
	for(unsigned int i = 0; i < t.size(); i++)
	{
		if(t[i].documentName == f)
			return t[i].count;
	}
	return 0;
}

bool allAppearsTree(string line, const AVLSearchTree <string, WordItem *> & tree, string & r, string f)
{
	// returns true if all of the queried words are in given file and changes the resulting string
	// this function is for avl search tree

	istringstream ss(line);
	string word;

	while(ss >> word)
	{
		AVLNode<string, WordItem*>* temp;
		temp = tree.find(word); // measure only this line

		if(temp != NULL) // if word exists in tree
		{
			int count = inFile(temp->data->list, f);

			if(count != 0) // if it is in the wanted file 
				r += ", " + word + " found " + to_string(count) + " times";

			else
				return false;
		}

		else
			return false;
	}

	return true;
}

bool allAppearsHash(string line, HashTable <string, WordItem *> & table, string & r, string f)
{
	// returns true if all of the queried words are in the given file and changes the resulting string
	// this function is for hash table

	istringstream ss(line);
	string word;

	while(ss >> word)
	{		
		HashEntry<string, WordItem*>* temp;
		temp = table.find(word);

		if(temp != NULL) // if word exists in the table
		{
			int count = inFile(temp->data->list, f);

			if(count != 0) // if it is in the wanted file 
				r += ", " + word + " found " + to_string(count) + " times";

			else
				return false;
		}

		else
			return false;
	}

	return true;
}

bool allAppearsVector(string line, const vector<WordItem> & v, string & r, string f)
{
	// returns true if all of the queried words are in the given file and changes the resulting string
	// this function is for hash table

	istringstream ss(line);
	string word;

	while(ss >> word)
	{
		int temp = binarySearch(v, 0, v.size()-1, word);

		if(temp != -1) // if word exists in the table
		{
			int count = inFile(v[temp].list, f);

			if(count != 0) // if it is in the wanted file 
				r += ", " + word + " found " + to_string(count) + " times";

			else
				return false;
		}

		else
			return false;
	}

	return true;
}

unsigned int timeTree(string line, const AVLSearchTree <string, WordItem *> & tree)
{
	int k = 1000;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < k; i++)
	{
		istringstream ss(line);
		string word;

		while(ss >> word)
		{
			tree.find(word);
		}
	}

	auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);

	return BSTTime.count() / k;
}

unsigned int timeHashTable(string line, HashTable <string, WordItem *> & table)
{
	int k = 1000;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < k; i++)
	{
		istringstream ss(line);
		string word;

		while(ss >> word)
		{
			table.find(word);
		}
	}

	auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);

	return HTTime.count() / k;
}

unsigned int timeVector(string line, const vector<WordItem> & v)
{
	int k = 1000;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < k; i++)
	{
		istringstream ss(line);
		string word;

		while(ss >> word)
		{
			// perform binary search in vector
			binarySearch(v, 0, v.size()-1, word);
		}
	}

	auto BSTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);

	return BSTime.count() / k;
}


// QUICK SORT //
const WordItem & median3(vector<WordItem> & a, int left, int right)
{
	int center = (left + right) / 2;

	if (a[center].word < a[left].word)
		swap(a[left], a[center]);

	if (a[right].word < a[left].word)
		swap(a[left], a[right]);

	if (a[right].word < a[center].word)
		swap(a[center], a[right]);

	// Place pivot at position right - 1
	swap(a[center], a[right - 1]);
	return a[right - 1];
}

void insertionSort(vector<WordItem> & a, int left, int right)
{
	// internal insertion sort for quick sort when array size is small
    for (int p = left + 1; p <= right; p++)
    {
		WordItem tmp = a[p];
        int j;

		for(j = p; j > left && tmp.word < a[j - 1].word; j--)
            a[j] = a[j - 1];

        a[j] = tmp;
    }
}

int random_range(const int & min, const int & max) 
{     
	// RETURNS RANDOM INTEGER BETWEEN AND INCLUDING MIN AND MAX 

	static mt19937 generator(time(0));     
	uniform_int_distribution<int> distribution(min, max);     
	return distribution(generator); 
}

void quicksort(vector<WordItem> & a, int left, int right, string type)
{
        if(left + 10 <= right)
        {
			WordItem pivot;
			int i = left, j = right - 1;

			if(type == "median")
			{
				pivot = median3(a, left, right);
				j--;
			}

			else if(type == "random")
			{
				int random = random_range(left, right);
				pivot = a[random];
				swap(a[random], a[right]);
			}

			else if(type == "first")
			{
				pivot = a[left];
				swap(a[left], a[right]);
			}

            for ( ; ; )
            {
				while(i < a.size() && a[i].word < pivot.word){i++;}

				while(j > 0 && pivot.word < a[j].word){j--;}

                if (i < j)
					swap(a[i], a[j]);
                else
					break;
            }

			// Restore pivot
			if(type == "median")
				swap(a[i], a[right - 1]); 

			else
				swap(a[i], a[right]);

			quicksort(a, left, i - 1, type); // Sort small elements
			quicksort(a, i + 1, right, type); // Sort large elements
        }

        else  // Do an insertion sort on the subarray
			insertionSort(a, left, right);
}
// ---------- //

// INSERTION SORT //
void insertionSort(vector<WordItem> & a)
{
	int j;

	// loop over the passes
	for (int p = 1;  p < a.size(); p++)
	{
		WordItem tmp = a[p]; 
		
		// loop over the elements
		for (j = p; j > 0 &&  tmp.word < a[j - 1].word; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
	}
}
// -------------- //

// MERGE SORT //
void merge(vector<WordItem> & a, int start, int mid, int end) 
{ 
	// in-place merge sorting. Doesn't use extra memory
    int start2 = mid + 1; 

	if (a[mid].word <= a[start2].word){return;} 

    while (start <= mid && start2 <= end)
	{
		if (a[start].word <= a[start2].word)
            start++;

        else 
		{ 
			WordItem value = a[start2]; 
            int index = start2; 
 
            while (index != start)
			{ 
                a[index] = a[index - 1]; 
                index--; 
            } 
            a[start] = value; 

            start++; 
            mid++; 
            start2++; 
        } 
    } 
} 
  
void mergeSort(vector<WordItem> & a, int l, int r) 
{ 
    if (l < r) 
	{
        int m = l + (r - l) / 2; 
  
        mergeSort(a, l, m); 
        mergeSort(a, m + 1, r); 
  
        merge(a, l, m, r); 
    } 
}
// ---------- //

// HEAP SORT //
int leftChild(int i){return 2*i+1;}

void percDown(vector<WordItem> & a, int i, int n)
{
	int child;
	WordItem tmp;

    for (tmp=a[i] ; leftChild(i) < n; i = child)
    {
		child = leftChild(i);
		if (child != n-1 && a[child].word < a[child+1].word)
			child++;
		if (a[child].word > tmp.word)
			a[i] = a[child];
        else
			break;
     }

	a[i] = tmp;
}

void heapsort(vector<WordItem> & a)
{
	// buildHeap
	for (int i = a.size()/2; i >=0; i--)
		percDown(a, i, a.size());

	// sort
	for (int j = a.size() -1; j >0; j--)
	{
  	     swap(a[0], a[j]);    // swap max to the last pos.
         percDown(a, 0, j); // re-form the heap
	}
}
// --------- //

int main()
{
	int fileNumber;

	cout << "Enter number of input files: ";
	cin >> fileNumber;

	string* fileNames = new string [fileNumber];

	for(int i = 0; i < fileNumber; i++)
	{
		cout << "Enter " << i+1 << ". file name: ";
		cin >> fileNames[i];		
	}

	AVLSearchTree <string, WordItem *>  myTree("notFound");
	HashTable <string, WordItem *> myHashTable("notFound");

	vector<WordItem> quickSortVector_median; // to be quick sorted with pivot = median

	for(int i = 0; i < fileNumber; i++)
	{
		preprocessFile(myTree, myHashTable, quickSortVector_median, fileNames[i]);
	}

	cout << "After preprocessing, the unique word count is " << myHashTable.getUniqueWordCount()
		<< ". Current load ratio is " << myHashTable.getLoadFactor() << endl;

	vector<WordItem> quickSortVector_random = quickSortVector_median; // to be quick sorted with pivot = random
	vector<WordItem> quickSortVector_first = quickSortVector_median; // to be quick sorted with pivot = first
	vector<WordItem> insertionSortVector = quickSortVector_median; // to be insertion sorted
	vector<WordItem> mergeSortVector = quickSortVector_median; // to be merge sorted
	vector<WordItem> heapSortVector = quickSortVector_median; // to be heap sorted

	// sort the vector with pivot = median and time it
	auto start = std::chrono::high_resolution_clock::now();
	quicksort(quickSortVector_median, 0, quickSortVector_median.size()-1, "median");
	auto QS_MTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
	unsigned int qs_mTime = QS_MTime.count();

	string line;

	cout << "Enter queried words in one line: ";

	cin.ignore();
	getline(cin, line);

	changeChar(line);
	toLower(line);
	
	// find query for avl search tree
	bool found = false;

	for(int i = 0; i < fileNumber; i++)
	{
		string result = "in Document " + fileNames[i];

		if(allAppearsTree(line, myTree, result, fileNames[i]))
		{
			cout << result << "." << endl;
			found = true;
		}
	}

	if(!found)
		cout << "No document contains the given query" << endl;

	// find query for hash table
	bool foundT = false;

	for(int i = 0; i < fileNumber; i++)
	{
		string resultT = "in Document " + fileNames[i];

		if(allAppearsHash(line, myHashTable, resultT, fileNames[i]))
		{
			cout << resultT << "." << endl;
			foundT = true;
		}
	}

	if(!foundT)
		cout << "No document contains the given query" << endl;

	// find query for vector
	bool foundV = false;

	for(int i = 0; i < fileNumber; i++)
	{
		string resultV = "in Document " + fileNames[i];

		if(allAppearsVector(line, quickSortVector_median, resultV, fileNames[i]))
		{
			cout << resultV << "." << endl;
			foundV = true;
		}
	}

	if(!foundV)
		cout << "No document contains the given query" << endl;
	
	// call timing functions for data structures
	unsigned int treeTime = timeTree(line, myTree);
	unsigned int hashTime = timeHashTable(line, myHashTable);
	unsigned int vectorTime = timeVector(line, quickSortVector_median);

	// timing for sorting algorithms
	start = std::chrono::high_resolution_clock::now();
	quicksort(quickSortVector_random, 0, quickSortVector_random.size()-1, "random"); // quicksort random
	auto QS_RTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
	unsigned int qs_rTime = QS_RTime.count();

	start = std::chrono::high_resolution_clock::now();
	quicksort(quickSortVector_first, 0, quickSortVector_first.size()-1, "first"); // quicksort first
	auto QS_FTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
	unsigned int qs_fTime = QS_FTime.count();
	
	start = std::chrono::high_resolution_clock::now();
	insertionSort(insertionSortVector); // insertionsort
	auto ISTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
	unsigned int isTime = ISTime.count();

	start = std::chrono::high_resolution_clock::now();
	mergeSort(mergeSortVector, 0, mergeSortVector.size()-1); // mergesort
	auto MSTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
	unsigned int msTime = MSTime.count();

	start = std::chrono::high_resolution_clock::now();
	heapsort(heapSortVector); // heapsort
	auto HSTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);
	unsigned int hsTime = HSTime.count();

	cout << endl;
	cout << "Binary Search Tree Time: " << treeTime << endl;
	cout << "Hash Table Time: " << hashTime << endl;
	cout << "Binary Search Time: " << vectorTime << endl << endl;
	
	cout << "Quick Sort(median) Time: " << qs_mTime << endl;
	cout << "Quick Sort(random) Time: " << qs_rTime << endl;
	cout << "Quick Sort(first) Time: " << qs_fTime << endl;
	cout << "Merge Sort Time: " << msTime << endl;
	cout << "Heap Sort Time: " << hsTime << endl;
	cout << "Insertion Sort Time: " << isTime << endl << endl;

	cout << "Speed Up BST/HST: " << treeTime / double(hashTime) << endl;
	cout << "Speed Up Merge Sort/Quick Sort(Median): " << msTime / double(qs_mTime) << endl;
	cout << "Speed Up Heap Sort/Quick Sort(Median): " << hsTime / double(qs_mTime) << endl;
	cout << "Speed Up Insertion Sort/Quick Sort(Median): " << isTime / double(qs_mTime) << endl << endl;

	cout << "Speed Up Binary Search / Binary Search Tree Time: " << vectorTime / double(treeTime) << endl;
	cout << "Speed Up Binary Search / Hash Table Time: " << vectorTime / double(hashTime) << endl;

	// DELETE DYNAMIC MEMORY IN THE TREE STRUCTURE
	deleteDynamicMemoryTree(myTree.getRoot());
	delete [] fileNames;

	return 0;
}