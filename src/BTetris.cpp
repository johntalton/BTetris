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
#include <Alert.h>
#include <Application.h>
#include <MediaDefs.h> 
#include <Sound.h> 
#include <SoundPlayer.h> 
#include <Entry.h> 
#include <Path.h> 

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "BTetris.h"
#include "BTetrisWindow.h"

/*******************************************************
*
*******************************************************/
BTetris::BTetris() : BApplication(APP_SIGNATURE){
   BRect wind_pos;
   BPath path;
   int ref;
   BRect defaultSize(100,50,568,589);

   if(find_directory(B_USER_SETTINGS_DIRECTORY, &path) == B_OK) {
      path.Append("BTetris_settings");
      ref = open(path.Path(),O_RDONLY);
      if(ref >= 0){
         read(ref, (char *)&wind_pos, sizeof(wind_pos));
         close(ref);
         if(!wind_pos.Intersects(BScreen().Frame())){
            theWin = new BTetrisWindow(defaultSize);
         }else{
            theWin = new BTetrisWindow(wind_pos);
         }
      }else{ // hmm that was not in there
         theWin = new BTetrisWindow(defaultSize);
      }
   }else{ // gess there are no defaults ..first time?
      theWin = new BTetrisWindow(defaultSize);
   }
   
   
   
   /*
   BSound *sound;
   BSoundPlayer player;
   entry_ref eref;
   BEntry entry("/boot/home/devel/BTetris/Sounds/xmas.wav", true);
   BSoundPlayer::play_id id;
 //  if(entry.InitCheck()) == B_OK){
      if (entry.GetRef(&eref) == B_OK){
         sound = new BSound(&eref);
         if (sound->InitCheck() == B_OK){
            player.Start();
            player.SetVolume(0.2);
            id = player.StartPlaying(sound);
            sound->ReleaseRef();
            player.WaitForSound(id);
            
         }
      }
   //} 
   */
   
   /*
   thread_id folgeThread;
   folgeThread = spawn_thread(threadRunner, "BackGroundMusic", B_NORMAL_PRIORITY, this);
   resume_thread(folgeThread);
   */
   
}

/*******************************************************
*
*******************************************************/
BTetris::~BTetris(){

}

/*******************************************************
*   Thread that plays background music
*******************************************************/
int32 BTetris::PlaySound(){
   BSoundPlayer *player;
   BSound *sound; 
   BSoundPlayer::play_id id;
   BEntry entry("/boot/home/devel/BTetris/Sounds/deadloop.au", true);
   entry_ref eref;
   entry.GetRef(&eref);
   sound = new BSound(&eref);
   media_raw_audio_format fmt = sound->Format();
   player = new BSoundPlayer(&fmt, eref.name);
   player->SetVolume(0.1);
   while(true){
      snooze(50000);
      player->Start();
  
      id = player->StartPlaying(sound);

      player->WaitForSound(id);
   }
   sound->ReleaseRef();
   return B_OK;
}


/*******************************************************
*   Our lovely about box with hidden box
*******************************************************/
void BTetris::AboutRequested(){
   uint32 mask;
   mask = modifiers();
   if(B_SHIFT_KEY & mask){
      (new BAlert("About BTetris",
      "BTetris ©\n\nIf you spent the time to find this:\n\tA. You need a hobbie\n\tB. You're a hacker, code cracker, slacker.\n\tC. You really want to register this app!",
      " A "," B ","  C  ",B_WIDTH_AS_USUAL,B_IDEA_ALERT))->Go();
   }else{
      (new BAlert("About BTetris","BTetris ©1999\n\nTheAbstractCompany\nynop@acm.org\n\nVersion: 1.0 Beta 1.5\nTetrisEngine Version: 1.8","Thats Nice"))->Go();
   }
}

/*******************************************************
*   Ah .. do nothing .. just defalut pass off.
*******************************************************/
void BTetris::MessageReceived(BMessage *msg){
   switch(msg->what){
   default:
      BApplication::MessageReceived(msg);
      break; 
   }  
}

/*******************************************************
*   Make shure all preffs are saved and kill it all
*******************************************************/
bool BTetris::QuitRequested(){
   if(theWin->QuitRequested()){ 
      return true; // Ok .. fine .. leave then
   }else{
      (new BAlert("Oh","Someone Does not want to quit","WHAT!"))->Go();
      return false;
   }
}






