#include <iostream>
#include <string>
#include <fstream>
#include <sstream> 
#include "AVLSearchTree.cpp"

//#include "vld.h"

using namespace std;

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

bool validWord(string & q)
{
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
	for(int i = 0; i < s.length(); i++)
	{
		if(s[i] < 'A' ||s[i] > 'z')
			s[i] = ' ';
	}
}

void newNodeData(AVLNode<string, WordItem*>* node, string file)
{
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

void findQuery(string q, AVLSearchTree <string, WordItem *> & tree)
{
	if(validWord(q))
	{
		AVLNode<string, WordItem*>* temp;
		temp = tree.find(q);

		if(temp != NULL)
		{
			for(DocumentItem* p = temp->data->list; p != NULL; p = p->next)
			{
				cout << "in " << p->documentName << ", " << q << " found "
					<< p->count << " times." << endl;
			}
		}
		else
			cout << "not found" << endl;
	}

}


bool findQuery(string q, AVLSearchTree <string, WordItem *> & tree, const string & file);


void printInorder(AVLNode<string, WordItem*>* node) 
{ 
    if (node == NULL) 
        return; 
  
    /* first recur on left child */
    printInorder(node->left); 
  
    /* then print the data of node */
	cout << node->data->word << " "; 
  
    /* now recur on right child */
    printInorder(node->right); 
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
		cout<<"succesfully processed"<< i+1<<endl; // !!!
	}

	//////////////////
	printInorder(myTree.getRoot());
	cout << endl;
	//////////////////

	string line;

	cout << "Enter queried words in one line: ";

	cin.ignore();
	getline(cin, line);

	bool found = false;
	for(int i = 0; i < fileNumber; i++)
	{
		string result = "";
		if(findQuery(line, myTree, fileNames[i], result))
		{
			cout << result;
			found = true;
		}
	}

	if(!found)
		cout << " not found in any of the files";
		
	// DELETE DYNAMIC MEMORY
	delete [] fileNames;


	return 0;
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

bool findQuery(string q, AVLSearchTree <string, WordItem *> & tree, const string & file, string & r)
{
	istringstream ss(q);
	string word;

	bool notFound = false;


	while(ss >> word && !notFound)
	{
		AVLNode<string, WordItem*>* temp;		
		temp = tree.find(word);

		int count = inFile(temp->data->list, file);
		
		if(temp == NULL || count != 0)
			notFound = true;

		else if(temp != NULL)
		{
			r += 
		}
	}

	return notFound;
}