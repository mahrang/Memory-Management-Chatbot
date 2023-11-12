#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <string>
#include "chatbot.h"
#include <memory>


// forward declarations
class GraphEdge;

class GraphNode
{
private:
    //// STUDENT CODE
    ////
/* Task 4:  All instances of GraphEdge are changed in a way such that each instance of GraphNode exclusively owns the outgoing GraphEdges and holds non-owning references to incoming GraphEdges. Appropriate smart pointers are used to do this.  */
    // data handles (owned)
    // std::vector<GraphEdge *> _childEdges; original code
    std::vector<std::unique_ptr<GraphEdge>> _childEdges;  // edges to subsequent nodes

    // data handles (not owned)
    std::vector<GraphEdge *> _parentEdges; // edges to preceding nodes
    ChatBot _chatBot;

    ////
    //// EOF STUDENT CODE

    // proprietary members
    int _id;
    std::vector<std::string> _answers;

public:
    // constructor / destructor
    GraphNode(int id);
    //~GraphNode();  Task 0:  read comment in graphnode.cpp

    // getter / setter
    int GetID() { return _id; }
    int GetNumberOfChildEdges() { return _childEdges.size(); }
    GraphEdge *GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() { return _answers; }
    int GetNumberOfParents() { return _parentEdges.size(); }

    // proprietary functions
    void AddToken(std::string token); // add answers to list
    void AddEdgeToParentNode(GraphEdge *edge);
    void AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge);

    //// STUDENT CODE
    ////

    // void MoveChatbotHere(ChatBot *chatbot); original code
    // related to task 5, line 42 in graphnode.cpp, & line 250 in chatlogic.cpp
    void MoveChatbotHere(ChatBot chatbot);

    ////
    //// EOF STUDENT CODE

    void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */
