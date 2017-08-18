/*******************************************************
*   BTetris©
*
*
*   @author  YNOP (ynop@acm.org)
*   @vertion beta
*   @date    Sept 18 1999
*******************************************************/
#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>
#include <StorageKit.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "Globals.h"
#include "BTetrisWindow.h"
#include "BTetrisView.h"

/*******************************************************
*   Our wonderful BWindow, ya its kewl like that.
*   we dont do much here but set up the menubar and 
*   let the view take over.  We also nead some message
*   redirection and handling
*******************************************************/
BTetrisWindow::BTetrisWindow(BRect frame) : BWindow(frame,"BTetris",B_TITLED_WINDOW,B_NOT_ZOOMABLE|B_NOT_RESIZABLE){//
   BRect r;
   BMenu *menu;
   BMenuItem *item;
   
   SetPulseRate(10);
   
   r = Bounds();
   // Creat a standard menubar
   menubar = new BMenuBar(r, "MenuBar");
   // Standard File menu
   menu = new BMenu("File");
   menu->AddItem(item=new BMenuItem("New Game",new BMessage(NEW_GAME),'N'));
   menu->AddItem(new BSeparatorItem());
   menu->AddItem(item=new BMenuItem("About BTetris", new BMessage(B_ABOUT_REQUESTED), 'A'));
   item->SetTarget(be_app);
   menu->AddItem(new BSeparatorItem());
   menu->AddItem(item=new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED)));//, 'Q'
   // add File menu now
   menubar->AddItem(menu);
   
   menu = new BMenu("Optons");
   menu->AddItem(item=new BMenuItem("Pause",new BMessage(PAUSE_GAME),'P'));
   menu->AddItem(item=new BMenuItem("Key Layout", new BMessage(KEY_LAYOUT_WIN)));
   menu->AddItem(item=new BMenuItem("Toggle Sound", NULL));
   menubar->AddItem(menu);
   
    
   
   // Attach the menu bar to he window
   AddChild(menubar);
   
   // Do a little claculating to take menubar int account
   r = Bounds();
   r.bottom = r.bottom - menubar->Bounds().bottom;
   
   View = new BTetrisView(this,r);
   
   View->MoveBy(0, menubar->Bounds().Height() + 1);
   AddChild(View);
   View->MakeFocus(true);
   Show();
   

}

/*******************************************************
*  
*******************************************************/
void BTetrisWindow::FrameResized(float,float){
}

/*******************************************************
*  
*******************************************************/
void BTetrisWindow::WindowActivated(bool active){
   if(!active){ View->MessageReceived(new BMessage(PAUSE_GAME));}
}

/*******************************************************
*   More nothingness. pass menu msg down to View.
*   like new game and pause and stuff like that.
*******************************************************/
void BTetrisWindow::MessageReceived(BMessage* msg){
   switch(msg->what){
   case NEW_GAME:
   case PAUSE_GAME:
   case NEW_THEME:
   case NEW_KEY_LAYOUT:
   case KEY_LAYOUT_WIN:
      View->MessageReceived(msg);
      break;
   default:
      BWindow::MessageReceived(msg);
   }
}

/*******************************************************
*   Someone asked us nicely to quit. I gess we should
*   so clean up. save our setings (position of win)
*   and tell the main be_app to shut it all down .. bye
*******************************************************/
bool BTetrisWindow::QuitRequested(){
   BPath path;
   int ref;

   if(find_directory (B_USER_SETTINGS_DIRECTORY, &path, true) == B_OK) {
      path.Append("BTetris_settings");
      ref = creat(path.Path(), 0777);
      if (ref >= 0) {
         BRect frm = Frame();
         //frm = ConvertToScreen(frm);
         write(ref, (char *)&frm, sizeof(frm));
         close(ref);
      }
   }
   be_app->Lock();
   be_app->Quit();
   be_app->Unlock();
   //be_app->PostMessage(B_QUIT_REQUESTED);
   return true;
}
