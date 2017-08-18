#ifndef _BTETRIS_H
#define _BTETRIS_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "BTetrisWindow.h"

extern const char *APP_SIGNATURE;
class BTetris : public BApplication {
   public:
      BTetris();
      ~BTetris();
      static int32 BTetris::threadRunner(void* obj){
         return ((BTetris*)obj)->PlaySound();
      }
      int32 PlaySound();
      virtual void AboutRequested();
      virtual void MessageReceived(BMessage*);
      virtual bool QuitRequested();
   private:

      BTetrisWindow *theWin;
};
#endif