#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>

#include <wx/event.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/gdicmn.h>
#include <wx/graphics.h>
#include <wx/image.h>
#include <wx/mstream.h>
#include <wx/quantize.h>
#include <wx/scopedptr.h>
#include <wx/sizer.h>
#include <wx/stopwatch.h>
#include <wx/versioninfo.h>
#include <wx/wfstream.h>
#include <wx/wx.h>


#include "image_data.hpp"

class image_window;
class image_frame;

class show_image: public wxApp
{

public:
   virtual bool OnInit();

};

class image_frame: public wxFrame
{
private:
   image_window* window;
public:
   image_frame(const wxString& title, const wxPoint& pos, const wxSize& size);
   void set_window(image_window* wnd) {window = wnd;};

private:
   void OnHello(wxCommandEvent& event);
   void OnSave(wxCommandEvent& event);
   void OnExit(wxCommandEvent& event);
   void OnAbout(wxCommandEvent& event);

   DECLARE_EVENT_TABLE()

};

enum
{
   ID_Hello = 1,
   ID_Save,
   ID_Exit
};

wxBEGIN_EVENT_TABLE(image_frame, wxFrame)
   EVT_MENU(ID_Hello, image_frame::OnHello)
   EVT_MENU(ID_Save, image_frame::OnSave)
   EVT_MENU(ID_Exit, image_frame::OnExit)
   EVT_MENU(wxID_EXIT, image_frame::OnExit)
   EVT_MENU(wxID_ABOUT, image_frame::OnAbout)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(show_image);

class image_window : public wxScrolledWindow
{
private:
   std::unique_ptr<wxBitmap> bitmap;
   uint64_t bitmap_width, bitmap_height;
   image_frame* parent;
   wxImage image;

public:
   image_window(wxWindow* parent, wxWindowID id, wxString filename);
   void OnSave(const wxString& filename);
   void OnLeftClick(wxMouseEvent& event);

private:
   void OnDraw(wxDC& dc);

   wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(image_window, wxScrolledWindow)
   EVT_LEFT_DOWN(image_window::OnLeftClick)
wxEND_EVENT_TABLE()

image_window::image_window(wxWindow* p, wxWindowID id, wxString filename)
   : wxScrolledWindow(p, id), parent(dynamic_cast<image_frame*>(p))
{
   image = wxImage(filename);
   if(!image.IsOk()) {
      std::cerr << "Can't open file " << filename << std::endl;
      return;
   }
   bitmap_width = image.GetWidth();
   bitmap_height = image.GetHeight();
   SetScrollbars(1,1, bitmap_width, bitmap_height, 0, 0);
   bitmap = std::make_unique<wxBitmap>(image);
   return;
}

void image_window::OnDraw(wxDC& dc)
{ 
   dc.DrawBitmap(*bitmap, 0, 0, false);
}

void image_window::OnSave(const wxString& filename)
{
   bitmap->SaveFile(filename, wxBITMAP_TYPE_TIFF);
   return;
}

void image_window::OnLeftClick(wxMouseEvent& event)
{
   wxClientDC context = wxClientDC(this);
   wxColour color;
   wxPoint point = event.GetLogicalPosition(context);
   // turns out this isn't implemented in OSX
   // bool okay = context.GetPixel(point.x, point.y, &color);
   // std::cerr << "GetPixel = " << okay << std::endl;
   int red, green, blue, alpha;
   if(image.HasAlpha()) {
      alpha = image.GetAlpha(point.x, point.y);
   } else {
      alpha = 255;
   }
   red = image.GetRed(point.x, point.y);
   green = image.GetGreen(point.x, point.y);
   blue = image.GetBlue(point.x, point.y);
   std::stringstream coords; 
   coords << "click (" << point.x << ", " << point.y << ") with color (" << alpha << " (" << red << ", " << green << ", " << blue << ")";
   std::cout << coords.str() << std::endl;
   parent->SetStatusText(coords.str());
   return;
}


bool show_image::OnInit()
{
   wxInitAllImageHandlers();

   // the following three are managed by the app, window and frame 
   wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
   image_frame* frame = new image_frame("Image", wxPoint(50, 50), wxSize(640, 480));
   image_window* window = new image_window(frame, wxID_ANY, wxT("../images/girl.tif"));

   frame->set_window(window); 

   sizer->Add(window, 1, wxALL | wxEXPAND, 0);
   frame->SetSizer(sizer);
   frame->Show(true);
   return true;
}

image_frame::image_frame(const wxString& title, const wxPoint& pos, const wxSize& size)
   : wxFrame(NULL, wxID_ANY, title, pos, size)
{
   // Menu items won't appear if the event is already handled by another menu item
   wxMenu * menuFile = new wxMenu;
   menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
   menuFile->Append(ID_Save, "&Save As...\tCtrl-S", "Save File");
   menuFile->AppendSeparator();
   menuFile->Append(ID_Exit, "&Exit...\tCtrl-X", "Exit");
 
   wxMenu *menuHelp = new wxMenu;
   menuHelp->Append(wxID_ABOUT);

   wxMenuBar *menuBar = new wxMenuBar;
   menuBar->Append( menuFile, "&File" );
   menuBar->Append( menuHelp, "&Help" );
   SetMenuBar( menuBar );

   CreateStatusBar();
   SetStatusText( "Welcome to wxWidgets!" );

}

void image_frame::OnSave(wxCommandEvent& event)
{
   wxFileDialog *SaveDialog = new wxFileDialog(this, _("Save File As _?"), wxEmptyString, wxEmptyString,
      _("Tiff (*.tif)|*.tif|"),
      wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

   // Creates a Save Dialog with 4 file types
   if (SaveDialog->ShowModal() == wxID_OK) // If the user clicked "OK"
   {
      wxString doc_path = SaveDialog->GetPath();
      // set the path of our current document to the file the user chose to save under
      
      window->OnSave(doc_path);

      // bitmap->SaveFile(doc_path, wxBITMAP_TYPE_TIFF);
      
      // Set the Title to reflect the file open
      SetTitle(wxString("Edit - ") << SaveDialog->GetFilename());
   }
   // Clean up after ourselves
   SaveDialog->Destroy();
   return;
}



void image_frame::OnExit(wxCommandEvent& event)
{
   Close(true);
}

void image_frame::OnAbout(wxCommandEvent& event)
{
   wxMessageBox( "This is problem 2-1 from Digital Image Processing",
                 "About 2-1", wxOK | wxICON_INFORMATION );
}

void image_frame::OnHello(wxCommandEvent& event)
{
   wxLogMessage("Hello world from wxWidgets!");
}
