#ifndef _STACK_H
#define _STACK_H

template <class Object>
class Stack
{
	public:
		Stack();
        Stack(const Stack & rhs);
        ~Stack();

        bool isEmpty() const;
        bool isFull() const;
        void makeEmpty();

        void pop();
        void push(const Object & x);
        Object topAndPop();
	    const Object & top() const;
		const Stack & operator=(const Stack & rhs);

	private:
		struct StackNode
		{
			Object element;
			StackNode* next;

			StackNode(const Object & theElement, StackNode * n = NULL)
				:element(theElement), next(n) {}
		};

		StackNode* topOfStack;
};

#endif