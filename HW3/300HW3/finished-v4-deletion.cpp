#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include "AVLSearchTree.cpp"
#include "HashTable.cpp"
#include "vld.h" //!!!!!!!!!!!!!!!!!!!
using namespace std;

// MEHMET ENES BATTAL - HW3

// vectore cevrilmis !!!!!

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

// BURAYI UNUTMA !!!!
void deleteDynamicMemoryTable(HashTable <string, WordItem *> & table)
{
	vector<HashEntry<string, WordItem *>> arrayRef = table.getArrayRef();
	
	for(int i = 0; table.getArraySize(); i++)
	{
		if(arrayRef[i].data)
			delete arrayRef[i].data;
	}
}

bool toLower(string & q)
{
	// returns true and changes all the letters of the word to lowercase.

	if(q.length() > 0)
	{
		string lower = "";
		for(unsigned int i = 0; i < q.length(); i++)
		{
			if(q[i] >= 'A' && q[i] <= 'Z')
				lower += q[i] + 32;
		
			else	
				lower += q[i];
		}
		q = lower;
		return true;
	}

	return false;
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
			p[i].count++;
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
		if(p[i].documentName == file) // in an existing file, increase count
			p[i].count++;
	}

	// this file doesn't exist, add new DocumntItem
	p.push_back(DocumentItem(file, 1));
}

void preprocessFile(AVLSearchTree <string, WordItem *> & tree, HashTable <string, WordItem *> & table, const string & file)
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
			if(toLower(word))
			{
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
		if(toLower(word))
		{
			static bool queried = false;

			AVLNode<string, WordItem*>* temp;
			temp = tree.find(word); // measure only this line

			if(temp != NULL && !queried) // if word exists in tree
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
	}

	return true;
}

bool allAppearsHash(string line, HashTable <string, WordItem *> & table, string & r, string f)
{
	// returns true if all of the queried words are in given file and changes the resulting string
	// this function is for hash table

	istringstream ss(line);
	string word;

	while(ss >> word)
	{
		if(toLower(word))
		{
			HashEntry<string, WordItem*>* temp;
			temp = table.find(word);

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
	}

	return true;
}

int timeTree(string line, const AVLSearchTree <string, WordItem *> & tree)
{
	int k = 20000;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < k; i++)
	{
		istringstream ss(line);
		string word;

		while(ss >> word)
		{
			if(toLower(word))
			{
				tree.find(word);
			}
		}
	}

	auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);

	cout << "\nTime: " << BSTTime.count() / k << "\n";

	return BSTTime.count() / k;
}

int timeHashTable(string line, HashTable <string, WordItem *> & table)
{
	int k = 20000;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < k; i++)
	{
		istringstream ss(line);
		string word;

		while(ss >> word)
		{
			if(toLower(word))
			{
				table.find(word);
			}
		}
	}

	auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - start);

	cout << "\nTime: " << HTTime.count() / k << "\n";

	return HTTime.count() / k;
}

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

	for(int i = 0; i < fileNumber; i++)
	{
		preprocessFile(myTree, myHashTable, fileNames[i]);
	}

	cout << endl << endl;
	cout << "After preprocessing, the unique word count is " << myHashTable.getUniqueWordCount()
		<< ". Current load ratio is " << myHashTable.getLoadFactor() << endl;

	string line;

	cout << "Enter queried words in one line: ";

	cin.ignore();
	getline(cin, line);

	changeChar(line);
	
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

	// call timing functions

	int treeTime = timeTree(line, myTree);
	int hashTime = timeHashTable(line, myHashTable);

	cout << "Speed up: " << treeTime / double(hashTime) << endl;

	// DELETE DYNAMIC MEMORY IN THE TREE STRUCTURE
	deleteDynamicMemoryTree(myTree.getRoot());
	delete [] fileNames;

	// DELETE DYNAMIC MEMORY IN THE HASH TABLE STRUCTURE

	deleteDynamicMemoryTable(myHashTable);

	cout << "deletion complete" << endl;

	return 0;
}