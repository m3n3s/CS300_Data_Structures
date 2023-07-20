#include "Stack.h"

using namespace std;

// Constructor
template <class Object>
Stack<Object>::Stack()
{
	topOfStack = nullptr;
}

template <class Object>
Stack<Object>::Stack(const Stack<Object> & rhs)
{
	topOfStack = nullptr;
    *this = rhs;
}

template <class Object>
Stack<Object>::~Stack()
{
	makeEmpty();
}

template <class Object>
bool Stack<Object>::isEmpty() const
{
	return topOfStack == nullptr;
}

template <class Object>
bool Stack<Object>::isFull() const
{
	return false;
}

template <class Object>
void Stack<Object>::makeEmpty()
{
	while (!isEmpty())
		pop();
}


template <class Object>
void Stack<Object>::pop()
{
	if (!isEmpty())
	{
		StackNode* oldTop = topOfStack;
		topOfStack = topOfStack->next;
		delete oldTop; 
	}
}

template <class Object>
void Stack<Object>::push(const Object & x)
{
	topOfStack = new StackNode(x, topOfStack);
}

template <class Object>
Object Stack<Object>::topAndPop( )
{
	Object topItem = top();
    pop();
    return topItem;
}


template <class Object>
const Object & Stack<Object>::top() const
{
	if (!isEmpty())        
		return topOfStack->element;
}

template <class Object>
const Stack<Object> & Stack<Object>::operator=(const Stack<Object> & rhs)
{
	if (this != &rhs)
    {
		makeEmpty( );
        if (rhs.isEmpty())
			return *this;

        StackNode *rptr = rhs.topOfStack;
        StackNode *ptr  = new StackNode(rptr->element);
        topOfStack = ptr;
		
		for (rptr = rptr->next; rptr != NULL; rptr = rptr->next)
			ptr = ptr->next = new StackNode( rptr->element );
     }

     return *this;
}