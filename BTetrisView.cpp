/*******************************************************
*   BTetris©
*
*   @author  YNOP (ynop@acm.org)
*   @vertion beta
*   @date    Sept 18 1999
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <String.h>
#include <Path.h>
#include <TranslationUtils.h>
#include <StringView.h>
#include <Roster.h>
#include <Entry.h> 
#include <Path.h> 
#include <Resources.h>
#include <AppFileInfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "Globals.h"
#include "BTetrisView.h"
#include "BTetrisGameBoard.h"
#include "TetrisEngine.h"


/*******************************************************
*
*******************************************************/
BTetrisView::BTetrisView(BWindow *parentWin, BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS|B_NAVIGABLE){
   SetViewColor(216,216,216,0);
 
   parentWindow = parentWin;
   this->MakeFocus(true);
 
   // Shold be in the New Game function
   TE = new TetrisEngine(16,9);
   //TE->Go(); 
   
   BRect b;
   b = Bounds();

   StatusBar = new BBox(BRect(b.left-3,b.bottom-14,b.right+3,b.bottom+3),"StatusBar",B_FOLLOW_LEFT_RIGHT|B_FOLLOW_BOTTOM); 
   AddChild(StatusBar);

   b.InsetBy(5,5);
   
   BBox *Bb = new BBox(BRect(b.right-175,b.top,b.right,b.bottom - 15),"Box",B_FOLLOW_TOP_BOTTOM|B_FOLLOW_RIGHT);
   Bb->SetLabel("BTetris Controls");
   AddChild(Bb); 
   
   BRect bounds = Bb->Bounds();
   BMenu *Fmenu = new BMenu(" Block Style ");
   BMessage *msg;
   BDirectory dir;
   BEntry entry;
   BPath path;
   char name[B_FILE_NAME_LENGTH];
   
   
   app_info active;
   be_roster->GetAppInfo(APP_SIGNATURE,&active);
   
   	app_info ai;
	BFile file;
	BAppFileInfo afi;
	BPath file_path;
	BPath parent;
		
	be_app->GetAppInfo(&ai);
	file.SetTo(&ai.ref, B_READ_WRITE);
	afi.SetTo(&file);
	BEntry entry2(&ai.ref, FALSE);
	entry2.GetParent(&entry2);
	entry2.GetPath(&file_path);
	
	file_path.Append("./Themes/");
//	printf("path %s\n", file_path.Path());
  
    dir.SetTo(file_path.Path());

   // loop until we get them all
   while (dir.GetNextEntry(&entry, true) == B_NO_ERROR) {
      if (entry.IsDirectory()){
         entry.GetName(name);
         
         msg = new BMessage(NEW_THEME);
         msg->AddString("ThemeName",name);
         Fmenu->AddItem(item = new BMenuItem(name,msg));
         item->SetTarget(TGB);
      }
   }

   int curpos = 0;
 
   favmenu = new BMenuField(BRect(bounds.left+40,curpos+=25,bounds.right,20),"","",Fmenu);
   favmenu->SetDivider(0);
   Bb->AddChild(favmenu);
  
   SpeedBar = new BStatusBar(BRect(bounds.left+10,curpos+=40,bounds.right-10,20),"","Speed","");
   SpeedBar->SetMaxValue(10);
   Bb->AddChild(SpeedBar);

   BBox *NextBox = new BBox(BRect(bounds.left+10,curpos+=50,bounds.right-10,270),"NBox",B_FOLLOW_TOP_BOTTOM|B_FOLLOW_RIGHT);
   NextBox->SetLabel("Next Block");
   Bb->AddChild(NextBox); 
 
   BStringView *s = new BStringView(BRect(bounds.left+10,curpos+10,bounds.right/2,300),"score","Score:");
   Bb->AddChild(s);
   
   BStringView *score = new BStringView(BRect(bounds.right/2,curpos+10,bounds.right-10,300),"","000");
   Bb->AddChild(score);
 
 //  score->SetText("");
 
   TGB = new BTetrisGameBoard(BRect(b.left,b.top,b.right-180,b.bottom - 15),this,TE);   
   AddChild(TGB);
  
  // delete Fmenu;
}

/*******************************************************
*
*******************************************************/
BTetrisView::~BTetrisView(){


}

/*******************************************************
*
*******************************************************/
void BTetrisView::KeyDown(const char *bytes, int32 numBytes){
   if(TGB->Paused){ TGB->Paused = false; return; }
   SpeedBar->Update(5 - TGB->Speed - SpeedBar->CurrentValue());//put someplace else
   if(numBytes==1){
      switch(bytes[0]){
      case B_SPACE:
         //TE->();
         break;
      case B_RIGHT_ARROW: // move right
         TE->MoveRight();
         break;
      case B_LEFT_ARROW: // move left
         TE->MoveLeft();
         break;
      case B_DOWN_ARROW:  // rotate cw
         TE->RotateCW();
         break;
      case B_UP_ARROW:  // rotate ccw
         TE->RotateCCW();
         break;
         
      default:
         BView::KeyDown(bytes, numBytes);
         break;
      }
   }
}

/*******************************************************
*   Central messageing area for the whole app. All msgs
*   get sent here if they are one of ours, then they
*   get sent back to the apropriate place. This is the
*   command center of the app!
*******************************************************/
void BTetrisView::MessageReceived(BMessage *msg){
   switch(msg->what){
   case NEW_GAME:
      SpeedBar->Reset("Speed");
      SpeedBar->SetMaxValue(10);
      TGB->Speed = 3;
      TGB->Paused = false;
      TE->InitBoard();
      TE->Go();
      SpeedBar->Update(5 - TGB->Speed - SpeedBar->CurrentValue());
      break;
   case PAUSE_GAME:
      TGB->Paused = true;//!TGB->Paused;
      break;  
   case NEW_THEME:
      TGB->MessageReceived(msg);
      break;
   /*case SPEED_CHANGED:
      break;
   case NEXT_BLOCK:
      break;
   case SCORE_CHANGED:
      break;*/
   case KEY_LAYOUT_WIN:
      (new BAlert(NULL,"Currently I have not implemented this.  I hope you can live with the arrow keys till then :)","I will live!"))->Go();
      break;
   default:
      BView::MessageReceived(msg);
   }
}
