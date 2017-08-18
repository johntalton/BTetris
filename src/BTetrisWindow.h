#ifndef _BTETRIS_WINDOW_H
#define _BTETRIS_WINDOW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "BTetrisView.h"

class BTetrisWindow : public BWindow {
   public:
      BTetrisWindow(BRect);
      virtual void MessageReceived(BMessage*);
      virtual bool QuitRequested();
      virtual void FrameResized(float,float);
      virtual void WindowActivated(bool active);
   private:
      BMenuBar *menubar;
      BTetrisView *View;
};
#endif