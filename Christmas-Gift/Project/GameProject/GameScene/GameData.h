#pragma once
#define GAME_TIME 10.0f
#define FACE_POSITION 340.0f

class GameData
{
public:
	static enum StageId
	{
		A_1,		//Aの家の部屋１
		A_Child,	//Aの家の子供部屋
		B_1,		//Bの家の部屋１
		B_2,		//Bの家の部屋２
	};

	static int Stage;
	static bool ChengeStage;
	static bool isGift;				//プレゼント設置フラグ
	
	//フラグ
	static bool GameClearCheck;			//ゲームクリアフラグ
	static bool GameOverCheck;			//ゲームオーバフラグ
	static bool StartFadeIn;			//フェードインスタート
	static bool StartFadeOut;			//フェードアウトスタート
	static bool GameStart;				//ゲームスタートフラグ
	static bool isPauseGame;			//一時停止

	//整数
	static float second;					//秒数

	//実数
	static float FacePosition;			//UI顔の位置

};
