#ifndef _TETRISENGINE_H
#define _TETRISENGINE_H
//There are Seven Block TYPEs.
#define TEMP_BLOCK -1
#define EMPTY  0
#define SQUARE 1
//	11
//	11
#define LINE   2
//	1111
#define TEE    3
//	111
//	 1
#define LONE   4
//	111
//	  1
#define LTWO   5
//	  1
//	111
#define ZIG    6
//	11
//	 11
#define ZAG    7
//	 11
//	11
#define NUM_BLOCK_TYPES 7

#define R_000_DEGREE 0
#define R_090_DEGREE 1
#define R_180_DEGREE 2
#define R_270_DEGREE 3

struct Block{
   int Type;
   int Color;
};

class TetrisEngine{
   public:
      TetrisEngine(int w,int h);
      ~TetrisEngine();
      void Go();
      void InitBoard();
      void RotateCW();
      void RotateCCW();
      void MoveRight();
      void MoveLeft();
      void Drop();
      int GetBlock(int w,int h);
      int GetBlockType(int h,int w);
      int GetNextBlock();
      int GetScore();
      void SetBlock(int w,int h,int Block);
   private:
      Block CreatBlock();
      bool CheckFull();
      void CheckLines();
      int Width;
      int Height;
      int Score;
      Block CurBlock;
      Block NextBlock;
      Block GameBoard[20][20];
      bool StillDroping;
      int NumberOfColors;
      bool GoGo;
      int BlockCenterX;
      int BlockCenterY;
      int CurRotation;
};
#endif