#ifndef CHATGUI_H_
#define CHATGUI_H_

// need next line for for all the terms that begin with wx
#include <wx/wx.h>
#include <memory>  // need this for using std::unique_ptr

/* include the next line so that the compiler knows that ChatLogic refers to a class */
class ChatLogic; // forward declaration

/* ChatBotPanelDialog class is derived from/child class of wxScrolledWindow class.  wxScrolledWindow is the base/parent class. Public means the public and protected members of the base class (wxScrolledWindow) keep their member access in the derived class (ChatBotPanelDialog). */
// middle part of the window containing the dialog between user and chatbot
class ChatBotPanelDialog : public wxScrolledWindow
{
private:
    // control elements
    wxBoxSizer *_dialogSizer;
    wxBitmap _image;

    std::unique_ptr<ChatLogic> _chatLogic;
  
public:
    // constructor / destructor
    ChatBotPanelDialog(wxWindow *parent, wxWindowID id);

    // getter / setter
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
