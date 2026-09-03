#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

#include "chatlogic.h"
#include "graphnode.h"
#include "graphedge.h"
#include "chatbot.h"

// constructor WITHOUT memory allocation
ChatBot::ChatBot()
{
    // invalidate data handles
    _image = nullptr;
    _chatLogic = nullptr;
    _rootNode = nullptr;
}

// constructor WITH memory allocation
ChatBot::ChatBot(std::string filename)
{
    std::cout << "ChatBot Constructor" << std::endl;
    
    // invalidate data handles
    _chatLogic = nullptr;
    _rootNode = nullptr;

    // load image into heap memory
    _image = new wxBitmap(filename, wxBITMAP_TYPE_PNG);
}

// 1: destructor
ChatBot::~ChatBot()
{
    std::cout << "ChatBot Destructor" << std::endl;

    // deallocate heap memory
    if(_image != NULL) // Attention: wxWidgets used NULL and not nullptr
    {
        delete _image;
        _image = NULL;
    }
}

//// STUDENT CODE
////
/* Task 2:  Class design meets the Rule of Five guidelines.
   The Rule of Five states that if you have to write one of the functions listed below then you should consider implementing all of them with a proper resource management policy in place.  You can learn more about The Rule of Five in file "Part 5 - Resource Copying Policies".
The five functions are:
1.    destructor
2.    copy constructor
3.    copy assignment operator
4.    move constructor
5.    move assignment operator

Since the destructor is already provided above by ChatBot::~ChatBot(), I wrote the 2nd - 5th functions listed above to adhere to The Rule of Five.  The code below is similar to code in rule_of_five.cpp written in file "Part 5 - Resource Copying Policies".

The & in &source below indicates that this function received a reference to a parameter (see "Passing Variables by Reference" in file "Part 3 - Variables & Memory"), so it can change the value of source directly everywhere source is used. The & denotes that source is a lvalue reference. The & indicates that the address of source was sent to the function so that source can be modified directly.  W/o &, anything that was done to source in this function would only be done to a local copy of source in this function, not to the value of source everywhere it appears in the program.
Example of passing a variable by reference:
void AddFour(int &val)
{
    val += 4;
}
int main()
{
    int val = 0;
    AddFour(val);
    std::cout << "val = " << val << std::endl;
    return 0;
}

Output:
val = 4
Any changes made to val in AddFour() will also be done to val in main.
W/o the & in line 61, output would've been val = 0.

You might have noticed below that both
2: copy constructor
3: copy assignment operator
take a const reference to the source object as an argument, by which they promise that they won’t (and can’t) modify the content of source.  A common usage of const is to guard against accidentally changing a variable, especially when it is passed-by-reference as a function argument, as is the case below.  The compiler will catch a const variable that changes. For more on const, see "Lesson 1 - Foundations" folder, file "Part 2 - A* Search". */

// 2: copy constructor
ChatBot::ChatBot(const ChatBot &source){
    std::cout << "ChatBot Copy Constructor" << std::endl;
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;
/*  By using new below, we're allocating resource on the heap, just like in line 29.  delete _image is in the destructor in line 41.  For more info on new & delete, see file "Part 4 - Dynamic Memory Allocation" and the "raii_example.cpp" in file "Part 6:  Smart Pointers".
Why use new/delete instead of malloc/free:
The functions malloc and free are library functions and represent the default way of allocating and deallocating memory in C. In C++, they are also part of the standard and can be used to allocate blocks of memory on the heap.
With the introduction of classes and object oriented programming in C++ however, memory allocation and deallocation has become more complex: When an object is created, its constructor needs to be called to allow for member initialization. Also, on object deletion, the destructor is called to free resources and to allow for programmer-defined clean-up tasks. For this reason, C++ introduces the operators new / delete, which represent the object-oriented counterpart to memory management with malloc / free.
The format for allocating memory using new is
MyClass *myClass = new MyClass();
where MyClass() is the constructor.
The call to new has the following consequences:
    1.    Memory is allocated to hold a new object of type MyClass
    2.    A new object of type MyClass is constructed within the allocated memory by calling the constructor of MyClass
In the line below, _image is a pointer of type/class wxBitmap, as indicated in line 14 of chatbot.h (wxBitmap *_image;).  We're allocating memory to hold a new object (_image) of type wxBitmap.  The line below is the same as
wxBitmap *_image = new wxBitmap();
except it's split into 2 lines b/t here and line 14 of chatbot.h. */
    _image = new wxBitmap();
  // line below means value of _image = value stored in source._image
    *_image = *source._image;
// Added next line as stated in https://knowledge.udacity.com/questions/633625
    _chatLogic->SetChatbotHandle(this);
}

// 3: copy assignment operator
ChatBot &ChatBot::operator=(const ChatBot &source){
    std::cout << "ChatBot Copy Assignment Operator" << std::endl;
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;
    _image = new wxBitmap();
    *_image = *source._image;
    _chatLogic->SetChatbotHandle(this);
/* *this means the value stored in the "this" pointer.  See
https://faculty.cs.niu.edu/~mcmahon/CS241/Notes/this_pointer.html */
    return *this;
}

/* In the code below, the move constructor takes as its input an rvalue reference to a source object of the same class. In doing so, we are able to use the object within the scope of the move constructor. As can be seen, the implementation copies the data handle from source to target and immediately invalidates source after copying is complete. The ownership has been successfully changed (or moved) without the need to copy the data on the heap.  The && denontes that source is a rvalue reference.  This means that source holds the address of the object it was passed and that it has exclusive ownership of the object.  The ownership of source changed from the caller function (such as main) to the receiver function below. This is in contrast to a lvalue reference &source, where any change made to source in the receiver function would also be done to source in the caller function.
In
i = 1
j = 2
k = i + j
i, j, and k are lvalues
and 1, 2, and the sum i + j are rvalues.
You cannot pass a lvalue to a rvalue reference, so you can't do
void myFunction(int &&val)
main()
{
myFunction(i)
}
You can only pass rvalues to a rvalue reference, so you can do
myFunction(42)
and
myFunction(i + j)
Rvalue references are themselves lvalues.  Within the scope of myFunction, val is a lvalue as it gives access to the memory location where the number 42 is stored.
If you want to pass a lvalue to a function, then your function should take a lvalue reference as its argument, so you can do
void myFunction(int &val)
main()
{
myFunction(i)
}
If you want to pass a lvalue to a rvalue reference, you have to do
void myFunction(int &&val)
main()
{
myFunction(std::move(i))
}
b/c std::move converts a lvalue into a rvalue.
You can read more about rvalue reference in file "Part 5 - Resource Copying Policies" under the heading "Rvalue references".

1st example of move constructor used in main:
MyMovableClass obj2 = MyMovableClass(300);
Here we have a lvalue obj2, which is created by assigning it an rvalue object.

2nd example of move constructor:
void useObject(MyMovableClass obj)
{
    std::cout << "using object " << &obj << std::endl;
}
int main()
{
    MyMovableClass obj1(100); // constructor
    useObject(std::move(obj1));
    return 0;
}
The std::move function accepts an lvalue argument and returns it as an rvalue.  So by passing an object to std::move we can force the compiler to use move semantics, either in the form of move constructor or the move assignment operator.
You can read more about move constructors in file "Part 5 - Resource Copying Policies" under the heading "The move constructor".
The move constructor below is invoked in line 252 of chatlogic.cpp, which uses the 2nd example of move constructor shown above.  Line 252 of chatlogic.cpp uses the function in line 42 of graphnode.cpp. */
// 4: move constructor
ChatBot::ChatBot(ChatBot &&source){
    std::cout << "ChatBot Move Constructor" << std::endl;
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;
    _image = source._image;
    _chatLogic->SetChatbotHandle(this);

    source._chatLogic = nullptr;
    source._rootNode = nullptr;
    source._image = nullptr;
}

 // 5: move assignment operator
ChatBot &ChatBot::operator=(ChatBot &&source){
    std::cout << "ChatBot Move Assignment Operator" << std::endl;
    if (this == &source)
        return *this;
    delete[] _image;

    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;
    _image = source._image;
    _chatLogic->SetChatbotHandle(this);

    source._chatLogic = nullptr;
    source._rootNode = nullptr;
    source._image = nullptr;;
    return *this;
}

////
//// EOF STUDENT CODE

void ChatBot::ReceiveMessageFromUser(std::string message)
{
    // loop over all edges and keywords and compute Levenshtein distance to query
    typedef std::pair<GraphEdge *, int> EdgeDist;
    std::vector<EdgeDist> levDists; // format is <ptr,levDist>

    for (size_t i = 0; i < _currentNode->GetNumberOfChildEdges(); ++i)
    {
        GraphEdge *edge = _currentNode->GetChildEdgeAtIndex(i);
        for (auto keyword : edge->GetKeywords())
        {
            EdgeDist ed{edge, ComputeLevenshteinDistance(keyword, message)};
            levDists.push_back(ed);
        }
    }

    // select best fitting edge to proceed along
    GraphNode *newNode;
    if (levDists.size() > 0)
    {
        // sort in ascending order of Levenshtein distance (best fit is at the top)
        std::sort(levDists.begin(), levDists.end(), [](const EdgeDist &a, const EdgeDist &b) { return a.second < b.second; });
        newNode = levDists.at(0).first->GetChildNode(); // after sorting the best edge is at first position
    }
    else
    {
        // go back to root node
        newNode = _rootNode;
    }

    // tell current node to move chatbot to new node
    _currentNode->MoveChatbotToNewNode(newNode);
}

void ChatBot::SetCurrentNode(GraphNode *node)
{
    // update pointer to current node
    _currentNode = node;

    // select a random node answer (if several answers should exist)
    std::vector<std::string> answers = _currentNode->GetAnswers();
    std::mt19937 generator(int(std::time(0)));
    std::uniform_int_distribution<int> dis(0, answers.size() - 1);
    std::string answer = answers.at(dis(generator));

    // send selected node answer to user
    _chatLogic->SendMessageToUser(answer);
}

int ChatBot::ComputeLevenshteinDistance(std::string s1, std::string s2)
{
    // convert both strings to upper-case before comparing
    std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);

    // compute Levenshtein distance measure between both strings
    const size_t m(s1.size());
    const size_t n(s2.size());

    if (m == 0)
        return n;
    if (n == 0)
        return m;

    size_t *costs = new size_t[n + 1];

    for (size_t k = 0; k <= n; k++)
        costs[k] = k;

    size_t i = 0;
    for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i)
    {
        costs[0] = i + 1;
        size_t corner = i;

        size_t j = 0;
        for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j)
        {
            size_t upper = costs[j + 1];
            if (*it1 == *it2)
            {
                costs[j + 1] = corner;
            }
            else
            {
                size_t t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }

    int result = costs[n];
    delete[] costs;

    return result;
}
