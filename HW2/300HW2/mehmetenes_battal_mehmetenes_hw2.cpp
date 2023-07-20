#include <iostream>
#include <string>
#include <fstream>
#include <sstream> 
#include "AVLSearchTree.cpp"

using namespace std;

// MEHMET ENES BATTAL - HW2

struct DocumentItem 
{
	string documentName;
	int count;
	DocumentItem* next;

	DocumentItem(string n = "", int c = 0, DocumentItem* p = nullptr)
		:documentName(n), count(c), next(p){}
};

struct WordItem 
{
	string word;
	DocumentItem* list;

	WordItem(string element = "", DocumentItem* p = nullptr):
		word(element), list(p){}
};

void deleteDynamicMemory(AVLNode<string, WordItem*>* node)
{
	// Deletes the dynamically allocated memory of the tree which is inside the nodes

	if (node == NULL) 
        return; 
  
    /* first recur on left child */
    deleteDynamicMemory(node->left); 
  
    /* now recur on right child */
    deleteDynamicMemory(node->right); 

	// deletion
	WordItem* ptr = node->data;
	DocumentItem* p = node->data->list;

	while(p != NULL)
	{
		DocumentItem* p2 = p;
		p = p->next;
		delete p2;
	}

	delete ptr;
}

bool validWord(string & q)
{
	// returns true if it is a valid word and changes all the letter of the word to lowercase.

	if(q.length() > 1)
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

	DocumentItem* p = node->data->list;
	DocumentItem* ptr = p;

	while(p != NULL)
	{
		if(p->documentName == file)
		{
			p->count ++;
			return;
		}
		p = p->next;
	}

	// not in the same document, add the current document
	while(ptr->next != NULL){ptr = ptr->next;} // go to last node of document list

	ptr->next = new DocumentItem(file, 1, NULL);
}

void preprocessFile(AVLSearchTree <string, WordItem *> & tree, const string & file)
{
	// goes over evry word and creates the tree structure

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
			// add word to tree
			if(validWord(word))
			{
				AVLNode<string, WordItem*>* temp;
				temp = tree.find(word);

				if(temp == nullptr) // not in tree, add new node
				{
					DocumentItem* item = new DocumentItem(file, 1);
					WordItem* element = new WordItem(word, item);

					tree.insert(word, element);
				}

				else
				{
					// in tree, edit existing node
					newNodeData(temp, file);
				}
			}
		}
	}
}

int inFile(DocumentItem* t, const string & f)
{
	// returns how many times the word is found in given file
	for( ; t != NULL; t = t->next)
	{
		if(t->documentName == f)
			return t->count;
	}
	return 0;
}

bool allAppears(string line, const AVLSearchTree <string, WordItem *> & tree, string & r, string f)
{
	// returns true if all of the queried words are in given file and changes the resulting string

	istringstream ss(line);
	string word;

	while(ss >> word)
	{
		if(validWord(word))
		{
			AVLNode<string, WordItem*>* temp;
			temp = tree.find(word);

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

int main()
{
	int fileNumber;

	cout << "Enter number of input files: ";
	cin >> fileNumber;

	AVLSearchTree <string, WordItem *> myTree("notFound");

	string* fileNames = new string [fileNumber];

	for(int i = 0; i < fileNumber; i++)
	{
		string fileName;

		cout << "Enter " << i+1 << ". file name: ";
		cin >> fileName;

		fileNames[i] = fileName;

		preprocessFile(myTree, fileName);
	}

	string line;

	cout << "Enter queried words in one line: ";

	cin.ignore();
	getline(cin, line);

	// find query
	bool found = false;

	for(int i = 0; i < fileNumber; i++)
	{
		string result = "in Document " + fileNames[i];

		if(allAppears(line, myTree, result, fileNames[i]))
		{
			cout << result << "." << endl;
			found = true;
		}
	}

	if(!found)
		cout << "No document contains the given query" << endl;
		
	// DELETE DYNAMIC MEMORY	
	deleteDynamicMemory(myTree.getRoot());
	delete [] fileNames;

	return 0;
}