/*******************************************************
*   BTetris© - TetrisEngine
*
*   This is the Engine to Tetris. It handles all the 
*   good stuff.  We can biuld and GUI/TextBased Tetris
*   game around this. Currently it is implemented as
*   a larg Matrix.  It is a little slow and to 
*   manipulate it it usualy take two runs through the 
*   matrix to get anything done.
*
*   @author  YNOP (ynop@acm.org)
*   @vertion beta
*   @date    Sept 18 1999
*******************************************************/
#include <AppKit.h> //included for system_time()
#include <Alert.h>

#include <stdlib.h>
#include <stdio.h>

#include "TetrisEngine.h"
/*******************************************************
*
*******************************************************/
TetrisEngine::TetrisEngine(int h,int w){
   Height = h;
   Width = w;
   InitBoard();
   srandom(system_time());
   NumberOfColors = NUM_BLOCK_TYPES; // Shold be at least this
   //GameBoard = new int[h][w];

}
/*******************************************************
*
*******************************************************/
TetrisEngine::~TetrisEngine(){
   //delete *GameBoard;
}
/*******************************************************
*
*******************************************************/
void TetrisEngine::InitBoard(){
   for(int i=0;i<Height;i++){
      for(int j=0;j<Width;j++){
         GameBoard[i][j].Type = EMPTY;
      } 
   }
   CurBlock.Type = EMPTY;
   NextBlock.Type = EMPTY;
   CurBlock = CreatBlock();
   NextBlock = CreatBlock();
   Score = 0;
   StillDroping = false;
   GoGo = false;
}
/*******************************************************
*
*******************************************************/
void TetrisEngine::Go(){
   CurBlock = CreatBlock();
   NextBlock = CreatBlock();
   GoGo = true;
}
/*******************************************************
*
*******************************************************/
void TetrisEngine::RotateCW(){
   if(GoGo){
      switch(CurBlock.Type){
      case SQUARE:
         //duuh .. ist a square .. rotate dont to too much!!
         break;
      case LINE:
         switch(CurRotation){ //Simetric
         case R_000_DEGREE:case R_180_DEGREE:
            if((BlockCenterX +2) >= Width){ break; }
            if((BlockCenterX -1) < 0){ break; }
            if((GameBoard[BlockCenterY][BlockCenterX-1].Type == EMPTY)&&
               (GameBoard[BlockCenterY][BlockCenterX+1].Type == EMPTY)&&
               (GameBoard[BlockCenterY][BlockCenterX+2].Type == EMPTY)){
            
               GameBoard[BlockCenterY-1][BlockCenterX].Type = EMPTY;
               GameBoard[BlockCenterY+1][BlockCenterX].Type = EMPTY;
               GameBoard[BlockCenterY+2][BlockCenterX].Type = EMPTY;
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY][BlockCenterX+2].Type = TEMP_BLOCK;
               CurRotation = R_090_DEGREE;
            }
            break; 
         case R_090_DEGREE:case R_270_DEGREE:
            if((BlockCenterY + 2) >= Height){ break; }
            if((GameBoard[BlockCenterY-1][BlockCenterX].Type == EMPTY)&&
               (GameBoard[BlockCenterY+1][BlockCenterX].Type == EMPTY)&&
               (GameBoard[BlockCenterY+2][BlockCenterX].Type == EMPTY)){
            
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = EMPTY;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = EMPTY;
               GameBoard[BlockCenterY][BlockCenterX+2].Type = EMPTY;
            
               GameBoard[BlockCenterY-1][BlockCenterX].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+1][BlockCenterX].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+2][BlockCenterX].Type = TEMP_BLOCK;
               CurRotation = R_000_DEGREE;
            }
            break;
         } 
         break;
      case TEE:
         switch(CurRotation){
         case R_000_DEGREE:
            if((BlockCenterX -1) < 0){ break; }
            if(GameBoard[BlockCenterY][BlockCenterX-1].Type == EMPTY){
               
               GameBoard[BlockCenterY-1][BlockCenterX].Type = EMPTY;
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = TEMP_BLOCK;
               CurRotation = R_090_DEGREE;
            }
            break; 
         case R_090_DEGREE:
            if(GameBoard[BlockCenterY-1][BlockCenterX].Type == EMPTY){
            
               GameBoard[BlockCenterY][BlockCenterX+1].Type = EMPTY;
            
               GameBoard[BlockCenterY-1][BlockCenterX].Type = TEMP_BLOCK;
               CurRotation = R_180_DEGREE;
            }
            break; 
         case R_180_DEGREE:
            if((BlockCenterX + 1) >= Width){ break; }
            if(GameBoard[BlockCenterY][BlockCenterX+1].Type == EMPTY){
            
               GameBoard[BlockCenterY+1][BlockCenterX].Type = EMPTY;
            
               GameBoard[BlockCenterY][BlockCenterX+1].Type = TEMP_BLOCK;
               CurRotation = R_270_DEGREE;
            }
            break; 
         case R_270_DEGREE:
            if((BlockCenterY + 1) >= Height){ break; }
            if(GameBoard[BlockCenterY+1][BlockCenterX].Type == EMPTY){
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = EMPTY;
            
               GameBoard[BlockCenterY+1][BlockCenterX].Type = TEMP_BLOCK;
               CurRotation = R_000_DEGREE;
            }
            break; 
         }
         break;
      case LONE:
         switch(CurRotation){
         case R_000_DEGREE:
            if((BlockCenterX -1) < 0){ break; }
            if((GameBoard[BlockCenterY][BlockCenterX-1].Type == EMPTY)&&
               (GameBoard[BlockCenterY][BlockCenterX+1].Type == EMPTY)&&
               (GameBoard[BlockCenterY+1][BlockCenterX-1].Type == EMPTY)){
            
               GameBoard[BlockCenterY-1][BlockCenterX].Type = EMPTY;
               GameBoard[BlockCenterY+1][BlockCenterX].Type = EMPTY;
               GameBoard[BlockCenterY+1][BlockCenterX+1].Type = EMPTY;
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+1][BlockCenterX-1].Type = TEMP_BLOCK;
               CurRotation = R_090_DEGREE;
            }
            break; 
         case R_090_DEGREE:
            if((GameBoard[BlockCenterY-1][BlockCenterX-1].Type == EMPTY)&&
               (GameBoard[BlockCenterY-1][BlockCenterX].Type == EMPTY)&&
               (GameBoard[BlockCenterY+1][BlockCenterX].Type == EMPTY)){
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = EMPTY;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = EMPTY;
               GameBoard[BlockCenterY+1][BlockCenterX-1].Type = EMPTY;
            
               GameBoard[BlockCenterY-1][BlockCenterX-1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY-1][BlockCenterX].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+1][BlockCenterX].Type = TEMP_BLOCK;
               CurRotation = R_180_DEGREE;
            }
            break; 
         case R_180_DEGREE:
            if((BlockCenterX +1 ) >= Width){ break; }
            if((GameBoard[BlockCenterY][BlockCenterX-1].Type == EMPTY)&&
               (GameBoard[BlockCenterY][BlockCenterX+1].Type == EMPTY)&&
               (GameBoard[BlockCenterY-1][BlockCenterX+1].Type == EMPTY)){
            
               GameBoard[BlockCenterY-1][BlockCenterX-1].Type = EMPTY;
               GameBoard[BlockCenterY-1][BlockCenterX].Type = EMPTY;
               GameBoard[BlockCenterY+1][BlockCenterX].Type = EMPTY;
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY-1][BlockCenterX+1].Type = TEMP_BLOCK;
               CurRotation = R_270_DEGREE;
            }
            break; 
         case R_270_DEGREE:
            if((BlockCenterY + 1) >= Height){ break; }
            if((GameBoard[BlockCenterY-1][BlockCenterX].Type == EMPTY)&&
               (GameBoard[BlockCenterY+1][BlockCenterX].Type == EMPTY)&&
               (GameBoard[BlockCenterY+1][BlockCenterX+1].Type == EMPTY)){
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = EMPTY;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = EMPTY;
               GameBoard[BlockCenterY-1][BlockCenterX+1].Type = EMPTY;
            
               GameBoard[BlockCenterY-1][BlockCenterX].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+1][BlockCenterX].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+1][BlockCenterX+1].Type = TEMP_BLOCK;
               CurRotation = R_000_DEGREE;
            }
            break; 
         }
         break;
      case LTWO:
         switch(CurRotation){
         case R_000_DEGREE:
            if((BlockCenterX + 1) >= Width){ break; }
            if((GameBoard[BlockCenterY-1][BlockCenterX-1].Type == EMPTY)&&
               (GameBoard[BlockCenterY][BlockCenterX-1].Type == EMPTY)&&
               (GameBoard[BlockCenterY][BlockCenterX+1].Type == EMPTY)){
            
               GameBoard[BlockCenterY-1][BlockCenterX].Type = EMPTY;
               GameBoard[BlockCenterY+1][BlockCenterX].Type = EMPTY;
               GameBoard[BlockCenterY+1][BlockCenterX-1].Type = EMPTY;
            
               GameBoard[BlockCenterY-1][BlockCenterX-1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY][BlockCenterX-1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = TEMP_BLOCK;
               CurRotation = R_090_DEGREE;
            }
            break; 
         case R_090_DEGREE:
            if((BlockCenterY + 1) >= Height){ break; }
            if((GameBoard[BlockCenterY-1][BlockCenterX].Type == EMPTY)&&
               (GameBoard[BlockCenterY-1][BlockCenterX+1].Type == EMPTY)&&
               (GameBoard[BlockCenterY+1][BlockCenterX].Type == EMPTY)){
            
               GameBoard[BlockCenterY-1][BlockCenterX-1].Type = EMPTY;
               GameBoard[BlockCenterY][BlockCenterX-1].Type = EMPTY;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = EMPTY;
            
               GameBoard[BlockCenterY-1][BlockCenterX].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY-1][BlockCenterX+1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+1][BlockCenterX].Type = TEMP_BLOCK;
               CurRotation = R_180_DEGREE;
            }
            break; 
         case R_180_DEGREE:
            if((BlockCenterX - 1) < 0){ break; }
            if((GameBoard[BlockCenterY][BlockCenterX-1].Type == EMPTY)&&
               (GameBoard[BlockCenterY][BlockCenterX+1].Type == EMPTY)&&
               (GameBoard[BlockCenterY+1][BlockCenterX+1].Type == EMPTY)){
               
               GameBoard[BlockCenterY-1][BlockCenterX].Type = EMPTY;
               GameBoard[BlockCenterY-1][BlockCenterX+1].Type = EMPTY;
               GameBoard[BlockCenterY+1][BlockCenterX].Type = EMPTY;
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+1][BlockCenterX+1].Type = TEMP_BLOCK;
               CurRotation = R_270_DEGREE;
            }
            break; 
         case R_270_DEGREE:
            if((GameBoard[BlockCenterY-1][BlockCenterX].Type == EMPTY)&&
               (GameBoard[BlockCenterY+1][BlockCenterX].Type == EMPTY)&&
               (GameBoard[BlockCenterY+1][BlockCenterX-1].Type == EMPTY)){
            
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = EMPTY;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = EMPTY;
               GameBoard[BlockCenterY+1][BlockCenterX+1].Type = EMPTY;
            
               GameBoard[BlockCenterY-1][BlockCenterX].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+1][BlockCenterX].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+1][BlockCenterX-1].Type = TEMP_BLOCK;
               CurRotation = R_000_DEGREE;
            }
            break; 
         }
         break;
      case ZIG: 
         switch(CurRotation){ //Simetric
         case R_000_DEGREE:case R_180_DEGREE:
            if((BlockCenterX - 1) < 0){ break; }
            if((GameBoard[BlockCenterY][BlockCenterX-1].Type == EMPTY)&&
               (GameBoard[BlockCenterY-1][BlockCenterX+1].Type == EMPTY)){
            
               GameBoard[BlockCenterY][BlockCenterX+1].Type = EMPTY;
               GameBoard[BlockCenterY+1][BlockCenterX+1].Type = EMPTY;
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY-1][BlockCenterX+1].Type = TEMP_BLOCK;
               CurRotation = R_090_DEGREE;
            }
            break; 
         case R_090_DEGREE:case R_270_DEGREE:
            if((BlockCenterY + 1) >= Height){ break; }
            if((GameBoard[BlockCenterY][BlockCenterX+1].Type == EMPTY)&&
              (GameBoard[BlockCenterY+1][BlockCenterX+1].Type == EMPTY)){
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = EMPTY;
               GameBoard[BlockCenterY-1][BlockCenterX+1].Type = EMPTY;
            
               GameBoard[BlockCenterY][BlockCenterX+1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+1][BlockCenterX+1].Type = TEMP_BLOCK;
               CurRotation = R_000_DEGREE;
            }
            break; 
         }
         break;
      case ZAG:
         switch(CurRotation){ //Simetric
         case R_000_DEGREE:case R_180_DEGREE:
            if((BlockCenterX + 1) >= Width){ break; }
            if((GameBoard[BlockCenterY-1][BlockCenterX-1].Type == EMPTY)&&
               (GameBoard[BlockCenterY][BlockCenterX+1].Type == EMPTY)){

               GameBoard[BlockCenterY][BlockCenterX-1].Type = EMPTY;
               GameBoard[BlockCenterY+1][BlockCenterX-1].Type = EMPTY;
            
               GameBoard[BlockCenterY-1][BlockCenterX-1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = TEMP_BLOCK;
               CurRotation = R_090_DEGREE;
            }
            break; 
         case R_090_DEGREE:case R_270_DEGREE:
            if((BlockCenterY + 1) >= Height){ break; }
            if((GameBoard[BlockCenterY][BlockCenterX-1].Type == EMPTY)&&
               (GameBoard[BlockCenterY+1][BlockCenterX-1].Type == EMPTY)){
              
               GameBoard[BlockCenterY-1][BlockCenterX-1].Type = EMPTY;
               GameBoard[BlockCenterY][BlockCenterX+1].Type = EMPTY;
            
               GameBoard[BlockCenterY][BlockCenterX-1].Type = TEMP_BLOCK;
               GameBoard[BlockCenterY+1][BlockCenterX-1].Type = TEMP_BLOCK;
               CurRotation = R_000_DEGREE;
            }
            break; 
         }
         break;
      }
   }
}
/*******************************************************
*
*******************************************************/
void TetrisEngine::RotateCCW(){
   RotateCW();  // Ok .. I know .. its REAL stupid way
   RotateCW();  // of doing this but come on do you
   RotateCW();  // realy want to rewrite all that code!!!
}
/*******************************************************
*
*******************************************************/
void TetrisEngine::MoveRight(){
   bool itsOK = true;
   if(GoGo){
      for(int i=Height;i>=0;i--){
         for(int j=Width;j>=0;j--){ 
            if(GameBoard[i][j].Type == TEMP_BLOCK){
               if(itsOK && (j+1 < Width) && ((GameBoard[i][j+1].Type == EMPTY) || (GameBoard[i][j+1].Type == TEMP_BLOCK))){
                  itsOK = true;
               }else{
                  itsOK = false;
               }
            }
         } 
      }
      if(itsOK){
         BlockCenterX = BlockCenterX + 1;
         for(int i=Height;i>=0;i--){
            for(int j=Width;j>=0;j--){ 
               if((GameBoard[i][j].Type == -1)){
                  GameBoard[i][j+1] = GameBoard[i][j];
                  GameBoard[i][j].Type = EMPTY;
               }
            } 
         }
      }
   }
}
/*******************************************************
*
*******************************************************/
void TetrisEngine::MoveLeft(){
   bool itsOK = true;
   if(GoGo){
      for(int i=Height;i>=0;i--){
         for(int j=0;j<Width;j++){ 
            if(GameBoard[i][j].Type == TEMP_BLOCK){
               if(itsOK && (j-1 >= 0) && ((GameBoard[i][j-1].Type == EMPTY) || (GameBoard[i][j-1].Type == TEMP_BLOCK))){
                  itsOK = true;
               }else{
                  itsOK = false;
               }
            }
         } 
      }
      if(itsOK){
         BlockCenterX = BlockCenterX - 1;
         for(int i=Height;i>=0;i--){
            for(int j=0;j<Width;j++){ 
               if((GameBoard[i][j].Type == -1)){
                  GameBoard[i][j-1] = GameBoard[i][j];
                  GameBoard[i][j].Type = EMPTY;
               }
            } 
         }
      }
   }
}
/*******************************************************
*
*******************************************************/
void TetrisEngine::Drop(){
   bool NextOpen = true;
   StillDroping = false;
   bool CompletRow = true;
   int i,j;
   
   if(GoGo){
      // Check to see if we can move the block down
      // also are we still in the droping process
      for(i=Height;i>=0;i--){
         for(j=0;j<Width;j++){ 
            if(GameBoard[i][j].Type == TEMP_BLOCK){
               if(NextOpen && (i+1 < Height) && ((GameBoard[i+1][j].Type == EMPTY)||(GameBoard[i+1][j].Type == TEMP_BLOCK))){
                  NextOpen = true;
                  StillDroping = true;      
               }else{
                  NextOpen = false;
                  StillDroping = false;
               } 
            }
         } 
      }
   
   
      if(StillDroping){ 
         for(i=Height;i>=0;i--){ // Now acutaly move them
            for(j=0;j<Width;j++){  
               if((GameBoard[i][j].Type == TEMP_BLOCK) && (i+1 < Height)){
                  GameBoard[i+1][j] = GameBoard[i][j];
                  GameBoard[i][j].Type = EMPTY;
               }
            } 
         }
         BlockCenterY = BlockCenterY+1; // drop center too.
      }else{ // No longer in the drop process
         // Loop through and get rid of our temp blocks
         for(i=Height;i>=0;i--){
            for(j=0;j<Width;j++){
               if(GameBoard[i][j].Type == TEMP_BLOCK){
                  GameBoard[i][j] = CurBlock;
               }
            }
         }
         //Check to see if we have finished any rows and get rid of them
         for(i=Height;i>=0;i--){
            CompletRow = true;
            for(j=0;j<Width;j++){
               if(CompletRow && (GameBoard[i][j].Type != EMPTY)){
                  //GameBoard[i][j].Type = EMPTY;
               }else{
                  CompletRow = false;
               }           
            }
            if(CompletRow){ 
               Score+=10;
               for(j=0;j<Width;j++){ // Delete the row
                  GameBoard[i][j].Type = EMPTY;
               }
               for(int x=i;x>0;x--){ // now move everything down
                  for(int y=0;y<Width;y++){
                     GameBoard[x][y] = GameBoard[x-1][y];
                  }
               }
               i++; // do this row again
            }
         }      
      
         // Set up he new Block and fetch another one
         CurBlock = NextBlock;
         NextBlock = CreatBlock();
         //Creat New Block and place in Matrix
         int center = Width/2;
         CurRotation = R_000_DEGREE;
         switch(CurBlock.Type){
         case SQUARE:
            if(!GameBoard[0][center].Type&&!GameBoard[0][center+1].Type&&
               !GameBoard[1][center].Type&&!GameBoard[1][center+1].Type){
               GameBoard[0][center].Type = 
               GameBoard[0][center+1].Type = 
               GameBoard[1][center].Type = 
               GameBoard[1][center+1].Type = TEMP_BLOCK;//CurBlock; 
               BlockCenterX = center;
               BlockCenterY = 1; 
            }else{
               //You lost!!
            }
            break;
         case LINE:
            if(!GameBoard[0][center].Type&&!GameBoard[1][center].Type&&!GameBoard[2][center].Type&&!GameBoard[3][center].Type){
               GameBoard[0][center].Type =
               GameBoard[1][center].Type = 
               GameBoard[2][center].Type =
               GameBoard[3][center].Type = TEMP_BLOCK;
               BlockCenterX = center;
               BlockCenterY = 1;
            }else{
               //You lost!!
            }
            break;
         case TEE:
            if(!GameBoard[0][center].Type&&!GameBoard[1][center].Type&&!GameBoard[2][center].Type&&!GameBoard[1][center+1].Type){
               GameBoard[0][center].Type=
               GameBoard[1][center].Type=
               GameBoard[2][center].Type=
               GameBoard[1][center+1].Type = TEMP_BLOCK;
               BlockCenterX = center;
               BlockCenterY = 1;
            }else{
               //You still lost!!
            }
            break;
         case LONE:
            if(!GameBoard[0][center].Type&&!GameBoard[1][center].Type&&!GameBoard[2][center].Type&&!GameBoard[2][center+1].Type){
               GameBoard[0][center].Type=
               GameBoard[1][center].Type=
               GameBoard[2][center].Type=
               GameBoard[2][center+1].Type = TEMP_BLOCK;
               BlockCenterX = center;
               BlockCenterY = 1;
            }else{
               //You still lost!!
            }
            break;
         case LTWO:
            if(!GameBoard[0][center].Type&&!GameBoard[1][center].Type&&!GameBoard[2][center].Type&&!GameBoard[2][center-1].Type){
               GameBoard[0][center].Type=
               GameBoard[1][center].Type=
               GameBoard[2][center].Type=
               GameBoard[2][center-1].Type = TEMP_BLOCK;
               BlockCenterX = center;
               BlockCenterY = 1;
            }else{
               //You still lost!!
            }
            break;
         case ZIG:
            if(!GameBoard[0][center].Type&&!GameBoard[1][center].Type&&!GameBoard[1][center+1].Type&&!GameBoard[2][center+1].Type){
               GameBoard[0][center].Type =
               GameBoard[1][center].Type =
               GameBoard[1][center+1].Type =
               GameBoard[2][center+1].Type = TEMP_BLOCK;
               BlockCenterX = center;
               BlockCenterY = 1;
            }else{
               //Lost Again .. man!
            }
            break;
         case ZAG:
            if(!GameBoard[0][center].Type&&!GameBoard[1][center].Type&&!GameBoard[1][center-1].Type&&!GameBoard[2][center-1].Type){
               GameBoard[0][center].Type =
               GameBoard[1][center].Type =
               GameBoard[1][center-1].Type =
               GameBoard[2][center-1].Type = TEMP_BLOCK;
               BlockCenterX = center;
               BlockCenterY = 1;
            }else{
               //Lost Again .. man!
            }
            break;
         }
      }
   }
   
}
/*******************************************************
*
*******************************************************/
int TetrisEngine::GetBlock(int h,int w){
   if(GameBoard[h][w].Type == TEMP_BLOCK){
      return CurBlock.Color;
   }
   if(GameBoard[h][w].Type == EMPTY){
      return EMPTY;
   }
   return GameBoard[h][w].Color;
}
/*******************************************************
*
*******************************************************/
int TetrisEngine::GetBlockType(int h,int w){
   if(GameBoard[h][w].Type == TEMP_BLOCK){
      return CurBlock.Type;
   }
   return GameBoard[h][w].Type;
}
/*******************************************************
*
*******************************************************/
int TetrisEngine::GetNextBlock(){
   return NextBlock.Type;
}
/*******************************************************
*
*******************************************************/
int TetrisEngine::GetScore(){
   return Score;
}
/*******************************************************
*
*******************************************************/
void TetrisEngine::SetBlock(int h,int w,int Block){
   GameBoard[h][w].Type = Block;
}
/*******************************************************
*   Creats a new random block with a random color 
*   withing the color range. We have to add one because
*   random includes zero. And we dont want that
*******************************************************/
Block TetrisEngine::CreatBlock(){
   Block tmp;
   tmp.Type = (random()%NUM_BLOCK_TYPES) + 1;
   tmp.Color = random()%9+1; // Max of 9 hmmm
   return tmp;
}
