#ifndef _BTETRIS_GAMEBOARD_H
#define _BTETRIS_GAMEBOARD_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>
#include <String.h>

#include "TetrisEngine.h"
#include "BTetrisView.h"

#define MAX_BITMAPS  20

class BTetrisGameBoard : public BView {
   public:
      BTetrisGameBoard(BRect,BView *p,TetrisEngine*);
      ~BTetrisGameBoard();
      virtual void Draw(BRect);
      virtual void Pulse();
      virtual void AttachedToWindow(void);
      virtual void KeyDown(const char *, int32);
      virtual void MessageReceived(BMessage*);
      void MakeTheme(char*);
      void MakeTheme(BString);
      bool Paused;
      int32 Speed;
   private:
      BView *parentWindow;
      TetrisEngine *TE;
      BBox *Bb;
      BView *StatusBar;
      BBitmap *theImage;
      BBitmap *backImage;
      BBitmap* BlockArray[MAX_BITMAPS];
//      int GameBoard[16][9];
      int32 size;
      int32 width;
      int32 height;
      bool ManualDraw;
      int Xoff,Yoff;
      
      int32 Time;
      bool FullBackground;
      int BlockCount;
      BString ThemeName;
      
};
#endif