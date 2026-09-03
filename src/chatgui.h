#ifndef CHATGUI_H_
#define CHATGUI_H_

// need next line for for all the terms that begin with wx
#include <wx/wx.h>
#include <memory>  // need this for using std::unique_ptr in Task 1

/* include the next line so that the compiler knows that ChatLogic in Task 1 refers to a class */
class ChatLogic; // forward declaration

/* ChatBotPanelDialog class is derived from/child class of wxScrolledWindow class.  wxScrolledWindow is the base/parent class. Public means the public and protected members of the base class (wxScrolledWindow) keep their member access in the derived class (ChatBotPanelDialog). */
// middle part of the window containing the dialog between user and chatbot
class ChatBotPanelDialog : public wxScrolledWindow
{
private:
    // control elements
    wxBoxSizer *_dialogSizer;
    wxBitmap _image;

// Task 1:  read comment in chatgui.cpp
    //// STUDENT CODE
    ////

    //ChatLogic *_chatLogic;  original code
    std::unique_ptr<ChatLogic> _chatLogic;
    ////
    //// EOF STUDENT CODE

public:
    // constructor / destructor
    ChatBotPanelDialog(wxWindow *parent, wxWindowID id);
/* Task 1:  Function below no longer needed because _chatLogic was made a unique pointer. */
    //~ChatBotPanelDialog();

    // getter / setter
    /* The .get() was added as part of Task 1 because _chatLogic was made a
       unique pointer.
       Use the .get() function to retrieve a raw pointer to the object.
       This will return the address of _chatLogic.
       See file "Part 6 - Smart Pointers". */
    ChatLogic *GetChatLogicHandle() { return _chatLogic.get(); }

    // events
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    // proprietary functions
    void AddDialogItem(wxString text, bool isFromUser = true);
    void PrintChatbotResponse(std::string response);

    DECLARE_EVENT_TABLE()
};

// dialog item shown in ChatBotPanelDialog
class ChatBotPanelDialogItem : public wxPanel
{
private:
    // control elements
    wxStaticBitmap *_chatBotImg;
    wxStaticText *_chatBotTxt;

public:
    // constructor / destructor
    ChatBotPanelDialogItem(wxPanel *parent, wxString text, bool isFromUser);
};

// frame containing all control elements
class ChatBotFrame : public wxFrame
{
private:
    // control elements
    ChatBotPanelDialog *_panelDialog;
    wxTextCtrl *_userTextCtrl;

    // events
    void OnEnter(wxCommandEvent &WXUNUSED(event));

public:
    // constructor / desctructor
    ChatBotFrame(const wxString &title);
};

// control panel for background image display
class ChatBotFrameImagePanel : public wxPanel
{
    // control elements
    wxBitmap _image;

public:
    // constructor / desctructor
    ChatBotFrameImagePanel(wxFrame *parent);

    // events
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    DECLARE_EVENT_TABLE()
};

// wxWidgets app that hides main()
class ChatBotApp : public wxApp
{
public:
    // events
    virtual bool OnInit();
};

#endif /* CHATGUI_H_ */
