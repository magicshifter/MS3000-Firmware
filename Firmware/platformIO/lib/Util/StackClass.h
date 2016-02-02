/*
http://www.cprogramming.com/tutorial/computersciencetheory/stackcode.html
     -------------------------------------------------------------------
    |                                                                   |
    |    Stack Class                                                    |
    |    ===========================================================    |
    |    This Stack has been implemented with templates to allow it     |
    |    to accomodate virtually any data type, and the size of the     |
    |    Stack is determined dynamically at runtime.                    |
    |                                                                   |
    |    There is also a new function: peek(), which, given a whole     |
    |    number 'Depth', returns the Stack element which is 'Depth'     |
    |    levels from the top.                                           |
    |                                                                   |
     -------------------------------------------------------------------
*/

#ifndef __StackClassH__
#define __StackClassH__

#include <assert.h>    // For error-checking purposes

//-------------------------------------------------
// Main structure of Stack Class:
//-------------------------------------------------

template <class Elem>
class Stack
{
  public:
    Stack(int MaxSize=500);
    Stack(const Stack<Elem> &OtherStack);
    ~Stack(void);

    inline void        Push(const Elem &Item); // Adds Item to the top
    inline Elem        Pop(void);              // Returns Item from the top
    inline const Elem &Peek(int Depth) const;  // Peek a depth downwards

  protected:
    Elem     *Data;           // The actual Data array
    int       CurrElemNum;    // The current number of elements
    const int MAX_NUM;        // Maximum number of elements
};

//-------------------------------------------------
// Implementation of Stack Class:
//-------------------------------------------------

// Stack Constructor function
template <class Elem>
Stack<Elem>::Stack(int MaxSize) :
    MAX_NUM( MaxSize )    // Initialize the constant
{
  Data = new Elem[MAX_NUM];
  CurrElemNum = 0;
}

// Stack Destructor function
template <class Elem>
Stack<Elem>::~Stack(void)
{
  delete[] Data;
}

// Push() function
template <class Elem>
inline void Stack<Elem>::Push(const Elem &Item)
{
  // Error Check: Make sure we aren't exceeding the maximum storage space
  assert(CurrElemNum < MAX_NUM);
  
  Data[CurrElemNum++] = Item;
}

// Pop() function
template <class Elem>
inline Elem Stack<Elem>::Pop(void)
{
  // Error Check: Make sure we aren't popping from an empty Stack
  assert(CurrElemNum > 0);

  return Data[--CurrElemNum];
}

// Peek() function
template <class Elem>
inline const Elem &Stack<Elem>::Peek(int Depth) const
{
  // Error Check: Make sure the depth doesn't exceed the number of elements
  assert(Depth < CurrElemNum);

  return Data[ CurrElemNum - (Depth + 1) ];
}

#endif /*__StackClassH__*/
