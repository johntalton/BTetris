/*******************************************************
*   BTetris©
*
*   Ok - its tetris - what more do you want.   
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    Sept 23 1999
*******************************************************/
#include "BTetris.h"

// Application's signature
const char *APP_SIGNATURE = "application/x-vnd.Abstract-BTetris";

/*******************************************************
*   Main.. What else do you want from me.
*******************************************************/
int main(int argc, char* argv[]){
   BTetris *app = new BTetris();
     
   app->Run();
   delete app;
   return B_NO_ERROR;
}