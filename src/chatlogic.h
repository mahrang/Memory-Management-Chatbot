#ifndef CHATLOGIC_H_
#define CHATLOGIC_H_

#include <vector>
#include <string>
/* If you don't include the next line, you'll get an error b/c of line 275 in chatlogic.cpp b/c it uses _panelDialog, which is declared in line 71 of chatgui.h, and chatlogic.cpp includes chatlogic.h.  Also, since chatgui.h uses #include <memory> & #include <wx/wx.h>, you don't need those here.  You would need those here if you don't use the next line b/c you use unique pointer here so you need #include <memory> and you use wxBitmap, so you need #include <wx/wx.h>. */
#include "chatgui.h"

/* You need the next 3 forward declarations so that the compiler knows that ChatBot, GraphEdge, & GraphNode used in the definition of class ChatLogic are classes. */
// forward declarations
class ChatBot;
class GraphEdge;
class GraphNode;

class ChatLogic
{
private:

    //// STUDENT CODE
    ////

    // data handles (owned)
    /* Task 3:  The vector _nodes are adapted in a way that the instances of GraphNodes to which the vector elements refer are exclusively owned by the class ChatLogic. An appropriate type of smart pointer is used to achieve this. */
    std::vector<std::unique_ptr<GraphNode>> _nodes;
    std::vector<GraphEdge *> _edges;

    ////
    //// EOF STUDENT CODE

    // data handles (not owned)
    GraphNode *_currentNode;
    ChatBot *_chatBot;
    ChatBotPanelDialog *_panelDialog;  // ChatBotPanelDialog is in chatgui.h

    // proprietary type definitions
    typedef std::vector<std::pair<std::string, std::string>> tokenlist;

    // proprietary functions
    template <typename T>
    void AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element);

public:
    // constructor / destructor
    ChatLogic();
    ~ChatLogic();

    // getter / setter
    void SetPanelDialogHandle(ChatBotPanelDialog *panelDialog);
    void SetChatbotHandle(ChatBot *chatbot);

    // proprietary functions
    void LoadAnswerGraphFromFile(std::string filename);
    void SendMessageToChatbot(std::string message);
    void SendMessageToUser(std::string message);
    wxBitmap *GetImageFromChatbot();
};

#endif /* CHATLOGIC_H_ */
