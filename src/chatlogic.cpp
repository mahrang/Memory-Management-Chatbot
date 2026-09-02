#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>
#include <memory>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"

ChatLogic::ChatLogic()
{
}

ChatLogic::~ChatLogic()
{
}

template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element)
{
    // find all occurences for current node
    auto token = tokens.begin();
    while (true)
    {
        token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) { return pair.first == tokenID;; });
        if (token != tokens.end())
        {
            element.AddToken(token->second); // add new keyword to edge
            token++;                         // increment iterator to next element
        }
        else
        {
            break; // quit infinite while-loop
        }
    }
}

void ChatLogic::LoadAnswerGraphFromFile(std::string filename)
{
    // load file with answer graph elements
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file)
    {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr))
        {
            // extract all tokens from current line
            tokenlist tokens;
            while (lineStr.size() > 0)
            {
                // extract next token
                int posTokenFront = lineStr.find("<");
                int posTokenBack = lineStr.find(">");
                if (posTokenFront < 0 || posTokenBack < 0)
                    break; // quit loop if no complete token has been found
                std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

                // extract token type and info
                int posTokenInfo = tokenStr.find(":");
                if (posTokenInfo != std::string::npos)
                {
                    std::string tokenType = tokenStr.substr(0, posTokenInfo);
                    std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

                    // add token to vector
                    tokens.push_back(std::make_pair(tokenType, tokenInfo));
                }

                // remove token from current line
                lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
            }

            // process tokens for current line
            auto type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "TYPE"; });
            if (type != tokens.end())
            {
                // check for id
                auto idToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "ID"; });
                if (idToken != tokens.end())
                {
                    // extract id from token
                    int id = std::stoi(idToken->second);

                    // node-based processing
                    if (type->second == "NODE")
                    {
                        // check if node with this ID exists already
                        auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](std::unique_ptr<GraphNode> &node) { return node->GetID() == id; });

                        // create new element if ID does not yet exist
                        if (newNode == _nodes.end())
                        {
                            _nodes.emplace_back(std::make_unique<GraphNode>(id));
                            newNode = _nodes.end() - 1; // get iterator to last element

                            // add all answers to current node
                            AddAllTokensToElement("ANSWER", tokens, **newNode);
                        }
                    }

                    // edge-based processing
                    if (type->second == "EDGE")
                    {
     
                        // find tokens for incoming (parent) and outgoing (child) node
                        auto parentToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "PARENT"; });
                        auto childToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "CHILD"; });

                        if (parentToken != tokens.end() && childToken != tokens.end())
                        {
                            // get iterator on incoming and outgoing node via ID search
                            auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](std::unique_ptr<GraphNode> &node) { return node->GetID() == std::stoi(parentToken->second); });
                            auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](std::unique_ptr<GraphNode> &node) { return node->GetID() == std::stoi(childToken->second); });

                            // create new edge
                            std::unique_ptr<GraphEdge> edge = std::make_unique<GraphEdge>(id);
/* Added .get() below b/c _nodes & edge were made unique pointers in tasks 3-4. Use the .get() function to retrieve a raw pointer to the object.  This will return the address of _chatLogic.  See file "Part 6 - Smart Pointers".

SetChildNode() has the function header SetChildNode(GraphNode *childNode), as shown in line 9 of graphedge.cpp.  That's an example of Passing Variables by Pointer.  To pass a variable to a function that initializes that variable as a pointer, you must pass that variable as a reference.
Example:
void AddThree(int *val)  // val is initialized as a pointer in function header
{
    *val += 3;
}
int main()
{
    int val = 0;
    AddThree(&val);
    return 0;
}
To learn more about Passing Variables by Pointer, see file "Part 3 - Variables & Memory" under the heading "Passing Variables by Pointer". 
The (*childNode).get() dereferences the held pointer and returns a reference to the object being pointed at.  The .get() gives you the address of childNode, which is the same as doing AddThree(&val) above. See https://stackoverflow.com/questions/62541075/what-is-the-difference-between-smart-ptr-and-smart-ptr-get
to learn more about using *smart_ptr.get() */
                            edge->SetChildNode((*childNode).get());
                            edge->SetParentNode((*parentNode).get());
/* edge is a unique pointer, as shown in line 190 above.  You can retrieve the internal raw pointer of edge with get().  edge.get() gives you the address of edge. You can see an example of get() in file "Part 6 - Smart Pointers" under the heading "The Unique pointer". */
                            _edges.push_back(edge.get());

                            // find all keywords for current node
                            AddAllTokensToElement("KEYWORD", tokens, *edge);

                            // store reference in child node and parent node
                            /* Added .get() b/c edge was made a unique pointer in
                               task 4.  Next line shows GraphNodes holds non-owning reference to incoming GraphEdges for task 4. */
                            (*childNode)->AddEdgeToParentNode(edge.get());
                            /* Next line shows GraphNodes exclusively owns the outgoing GraphEdges for task 4.  It also shows move semantics are used when transferring ownership from class ChatLogic, where all instances of GraphEdge are created, into instances of GraphNode for task 4.  AddEdgeToChildNode() is a function in class GraphNode. In the line below, ownership of edge is being transferred from ChatLogic to GraphNode b/c edge is a unique pointer and only 1 class at a time can own a unique pointer.  See file "Part 5 - Resource Copying Policies" to learn more about std::move.  In that file, under the header "When are move semantics used?", it says "A second area of application are cases where ownership needs to be transferred (such as with unique pointers, as we will soon see). The primary difference to shared references is that with move semantics we are not sharing anything but instead we are ensuring through a smart policy that only a single object at a time has access to and thus owns the resource."  edge is no longer used in the rest of this function, so it's ok to transfer ownership to AddEdgeToChildNode().  parentNode is of type GraphNode b/c it was a node in _nodes vector, as shown by line 184 above and line 24 in chatlogic.h. */
                            (*parentNode)->AddEdgeToChildNode(std::move(edge));
                        }

                        ////
                        //// EOF STUDENT CODE
                    }
                }
                else
                {
                    std::cout << "Error: ID missing. Line is ignored!" << std::endl;
                }
            }
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }

    //// STUDENT CODE
    ////

    // identify root node
    GraphNode *rootNode = nullptr;
    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        // search for nodes which have no incoming edges
        if ((*it)->GetNumberOfParents() == 0)
        {

            if (rootNode == nullptr)
            {
                // Added .get() b/c _nodes was made a unique pointer in Task 3.
                  rootNode = (*it).get(); // assign current node to root
            }
            else
            {
                std::cout << "ERROR : Multiple root nodes detected" << std::endl;
            }
        }
    }

  /* Task 5:  a local ChatBot instance is created on the stack at the bottom of function LoadAnswerGraphFromFile and move semantics are used to pass the ChatBot instance into the root node. The next 2 lines of code are similar to original code provided above in ChatLogic::ChatLogic(). The next 4 lines of code are also given in https://knowledge.udacity.com/questions/922060.  */

  // create instance of chatbot
   ChatBot chatBot("../images/chatbot.png");
    /* Add pointer to chatlogic so that chatbot answers can be passed on to the
       GUI. */
    chatBot.SetChatLogicHandle(this);  // function in class ChatBot in chatbot.h

    // Next 2 lines of code are to add chatbot to graph root node
    chatBot.SetRootNode(rootNode);
    /* MoveChatbotHere() is GraphNode::MoveChatbotHere() in graphnode.cpp.
    By using std::move, we were able to pass the ownership of the resources within chatBot to the function MoveChatbotHere. See file "Part 5 - Resource Copying Policies" to learn more about std::move. */
    rootNode->MoveChatbotHere(std::move(chatBot));
    
    ////
    //// EOF STUDENT CODE
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog)
{
    _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot)
{
    _chatBot = chatbot;
}

void ChatLogic::SendMessageToChatbot(std::string message)
{
    _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message)
{
    _panelDialog->PrintChatbotResponse(message);
}

wxBitmap *ChatLogic::GetImageFromChatbot()
{
    return _chatBot->GetImageHandle();
}
