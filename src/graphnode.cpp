#include "graphedge.h"
#include "graphnode.h"

GraphNode::GraphNode(int id)
{
    _id = id;
}

/*  Task 0:  Commenting out the function below gets rid of the error "Segmentation fault (core dumped)" that you get when you close the chatbot window.  This function has also been commented out in line 35 in graphnode.h.
This function was trying to deallocate the chatbot, but the chatbot was already deallocated in line 36 in ChatLogic::~ChatLogic() in chatlogic.cpp, hence the error.  In Task 5, the contents of ChatLogic::~ChatLogic() were commented out.  Read comment in ChatLogic::ChatLogic() in chatlogic.cpp to find out why.
GraphNode::~GraphNode()
{
    //// STUDENT CODE
    ////

    //delete _chatBot;

    ////
    //// EOF STUDENT CODE
}
*/

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
{
    _parentEdges.push_back(edge);
}

// Function below modified due to task 4 where edge was made a unique pointer.
void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
{
    _childEdges.push_back(std::move(edge));
}

//// STUDENT CODE
////
/* Next function is for task 5, which says:  In file chatlogic.cpp, a local ChatBot instance is created on the stack at the bottom of function LoadAnswerGraphFromFile and move semantics are used to pass the ChatBot instance into the root node.  Next function is used in line 252 in file chatlogic.cpp. */
void GraphNode::MoveChatbotHere(ChatBot chatbot)
{
    // Next line given in https://knowledge.udacity.com/questions/922060
    _chatBot = std::move(chatbot);  // _chatBot is a unique_ptr
    _chatBot.SetCurrentNode(this);
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(std::move(_chatBot));
    //_chatBot = nullptr; // invalidate pointer at source
}
////
//// EOF STUDENT CODE

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    //// STUDENT CODE
    ////

    return (_childEdges[index]).get();

    ////
    //// EOF STUDENT CODE
}
