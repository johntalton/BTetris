#ifndef _BTETRIS_VIEW_H
#define _BTETRIS_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "TetrisEngine.h"
#include "BTetrisGameBoard.h"
extern const char *APP_SIGNATURE;
class BTetrisView : public BView {
   public:
      BTetrisView(BWindow*,BRect);
      ~BTetrisView();
      virtual void KeyDown(const char *, int32);
      virtual void MessageReceived(BMessage*);
   private:
      BWindow *parentWindow;
      TetrisEngine *TE;
     // BBox *Bb;
      BView *StatusBar;
      BTetrisGameBoard *TGB;
      BMenuField *favmenu;
   //   BMenu *Fmenu;
      BMenuItem *item;
      BStatusBar *SpeedBar;
    
      int32 size;
      int32 width;
      int32 height;

};
#endif