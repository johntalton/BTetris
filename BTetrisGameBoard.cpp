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

#include <String.h>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG_ON false

#define EMPTY_BLOCK    0
#define NUMBER_BLOCK   10

#include "Globals.h"
#include "BTetrisView.h"
#include "BTetrisGameBoard.h"
#include "TetrisEngine.h"

/*******************************************************
*
*******************************************************/
BTetrisGameBoard::BTetrisGameBoard(BRect frame,BView *p,TetrisEngine *TEngine):BView(frame, "", B_FOLLOW_ALL_SIDES, B_PULSE_NEEDED|B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE){
   //this->MakeFocus(true);
   parentWindow = p;
   TE = TEngine;
 
   SetViewColor(B_TRANSPARENT_32_BIT); // This gets ride of Flicker!
 
   srandom(system_time());

   Paused = false;
   
   Speed = 0; // 0 - 4 range!!  0 == fast
   Time = Speed;
   ManualDraw = false;
   //INIT THIS some place else
   size = 30; // 10/30/50 Good!
   width = 9;
   height = 16;
   // damn setur crap;
   
   MakeTheme("BW");
/*
  if(count > 2){
  //(new BAlert(NULL,"greater than 2","ok"))->Go();
      for(int Block=1;Block<NUMBER_BLOCK;Block++){
         if(!IsThere[Block]){
            for(int valid=1;valid<NUMBER_BLOCK;valid++){
               if(IsThere[valid]){
                  BlockArray[Block] = BlockArray[valid];
                  IsThere[Block] = true;
               }
            }
         }
         if(!IsThere[Block]){
         //Big Prob, didn't find a non NULL?
         }   
      }
   }else if(count == 2){ // So we always know that the empty is differant from the rest
   //(new BAlert(NULL,"is  2","ok"))->Go();
      for(int n=3;n<NUMBER_BLOCK;n++){
            BlockArray[n] = BlockArray[2];
      }
   }else{
   //(new BAlert(NULL,"lessthan 2","ok"))->Go();
      ManualDraw = true;
      (new BAlert("Hmm","Manual Draw Mode","By Hand?"))->Go();
      //Big Prob, there is one or less images!!
   }
   */
   
   theImage = BlockArray[EMPTY_BLOCK];         
}
   
/*******************************************************
*   Clean up
*******************************************************/   
BTetrisGameBoard::~BTetrisGameBoard(){
   if(DEBUG_ON){printf("Entering View Destructor\n");}
   delete theImage;
  // delete BlockArray;
   delete backImage;
}
   
/*******************************************************
*
*******************************************************/
void BTetrisGameBoard::MakeTheme(char* name){
   BString tmp;
   tmp.SetTo(name);
   MakeTheme(tmp);

}
void BTetrisGameBoard::MakeTheme(BString Tname){
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
   // Set up bitmaps
   int count = 0;
   //ThemeName = "BW";
   ThemeName.SetTo(Tname);
   
   //(new BAlert(NULL,Tname.String(),"OK"))->Go();
   
   BString Temp = "Themes/";
   
   Temp.Append(ThemeName);
   Temp.Append("/EmptyBlock");
   
   backImage = BTranslationUtils::GetBitmapFile(Temp.String());
   if(!backImage){
      Temp = "Themes/";
      Temp.Append(ThemeName);
      Temp.Append("/FullBackGround");
    //  (new BAlert(NULL,Temp.String(),"ok"))->Go();
      backImage = BTranslationUtils::GetBitmapFile(Temp.String());
      if(!backImage){
         (new BAlert("Error","I could not find the image \"EmptyBlock\" or \"FullBackGround\" in selected theme directory\n\nUseing manual draw mode!","Thats OK"))->Go();
         ManualDraw = true;
         FullBackground = false;
      }else{
         FullBackground = true;
      }
   }else{
      //IsThere[EMPTY_BLOCK] = true;
      //count++;
      FullBackground = false;
   }
      
   
   count = -1;
   BDirectory dir;
   BEntry entry;
   BPath path;
   char name[B_FILE_NAME_LENGTH];
   BString Name;
   
	
   //file_path.Append("./Themes/"); 
   BString CurrentDir = file_path.Path();
   CurrentDir.Append("/");
   //BString CurrentDir = "/boot/home/devel/BTetris/";
   Temp = CurrentDir;
   Temp.Append("Themes/");
   Temp.Append(ThemeName);
   dir.SetTo(Temp.String());
   Temp.Append("/");
   // loop until we get them all
   while (dir.GetNextEntry(&entry, true) == B_NO_ERROR) {
      if (entry.IsFile()){
         entry.GetName(name);
         Name = name;
         if((Name == "FullBackGround")||(Name == "EmptyBlock")){}else{
            Name.Prepend(Temp);
       
             BlockArray[count+1] = BTranslationUtils::GetBitmapFile(Name.String());
            if(BlockArray[count+1] == NULL){}else{
               //(new BAlert("Hmm","Added a image","Ok"))->Go();
               //IsThere[count+1] = true;
               count++;
            }
         }
      }
   }
 
   if(count < 0){
      ManualDraw = true;
      (new BAlert("Hmm","Manual Draw Mode","By Hand?"))->Go();
   }
 
   BlockCount = count;
   Invalidate();
}

/*******************************************************
*
*******************************************************/
void BTetrisGameBoard::AttachedToWindow(void){
}

  
/*******************************************************
*   Clock for the game basicly. Ever pulse we check to
*   see if its time to update - and then we also call
*   draw.  This allows up to change the speed the blocks
*   fall
*******************************************************/
void BTetrisGameBoard::Pulse(){
   if(!Paused){
      if(Time >= Speed){
         Time = 0;
         TE->Drop();
         Invalidate();
      }else{
         Time++;
      }
   }
}

/*******************************************************
*   Draw the blocks(if we have found them) if there are
*   no blocks to draw use FillRect and fake it.  Also
*   keep up with the size and all that snazy stuff
*******************************************************/
void BTetrisGameBoard::Draw(BRect){
   if(DEBUG_ON){printf("Entering View Draw\n");}
   int tmp;
   Window()->Lock();

  //Does not work right if((TE->GetScore() > 0) && ((TE->GetScore() % 10) == 0)){ Speed--;}
   
   //size = Bounds().Height() / height;
   for(int32 j = 0;j < size*height; j=j+size+1){
      for(int32 i = 0;i < size*width; i=i+size+1){
         if(DEBUG_ON){printf("%d ",TE->GetBlock(j/size,i/size));}
         if(!ManualDraw){
            if((TE->GetBlock(j/size,i/size) == EMPTY_BLOCK)){
               if(FullBackground){
                  DrawBitmapAsync(backImage,BRect(i,j,i+size,j+size) ,BRect(i,j,i+size,j+size));
               }else{
                  DrawBitmapAsync(backImage, backImage->Bounds(), BRect(i,j,i+size,j+size));
               }
            }else{  
               tmp = TE->GetBlock(j/size,i/size);
               if(BlockCount == 0){
                  theImage = BlockArray[0];
               }else{
                  tmp = tmp % (BlockCount+1);
                  theImage = BlockArray[tmp];
               }
               
               if(theImage){
                  DrawBitmapAsync(theImage, theImage->Bounds(), BRect(i,j,i+size,j+size));
               }else{
                  //There is not image ... hmmm error !!
                  //(new BAlert(NULL,"","Yo"))->Go();
               }
            }
         }else{
            int c = TE->GetBlock(j/size,i/size);
            if(c == EMPTY_BLOCK){ 
               SetHighColor(0,0,0);
            }else{ 
               SetHighColor((c*100+random())%255,255,255);
            }
            FillRect(BRect(i+Xoff,j,i+size,j+size));
         }
      }
      if(DEBUG_ON){printf("\n");}
   }
   Window()->Unlock();
}

/*******************************************************
*   Pass our controlls up to the View to handle
*   if for some reason we have focus. 
*******************************************************/
void BTetrisGameBoard::KeyDown(const char *bytes, int32 numBytes){
   if(numBytes==1){
      switch(bytes[0]){
      case B_SPACE:
      case B_RIGHT_ARROW: // more right
      case B_LEFT_ARROW: // more left
      case B_DOWN_ARROW:  // rotate ccw
      case B_UP_ARROW:  // rotate cw
         parentWindow->KeyDown(bytes,numBytes);
         Invalidate();
         break;
      default:
         BView::KeyDown(bytes, numBytes);
         break;
      }
   }
}


/*******************************************************
*   More genaric msg stuff. nothing here
*******************************************************/
void BTetrisGameBoard::MessageReceived(BMessage *msg){
   if(DEBUG_ON){printf("Message Received\n");}
   BString tmp;
   
   switch(msg->what){
   case NEW_THEME:
      tmp.SetTo(msg->FindString("ThemeName"));
     // name[0] = (char)p_name[0];
      
      MakeTheme(tmp);
      break;
   default:
      BView::MessageReceived(msg);
   }
}



