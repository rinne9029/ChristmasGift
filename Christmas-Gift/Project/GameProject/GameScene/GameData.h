#pragma once

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
	static bool Start;
	static bool ChengeStage;
	static float BlueSleepSize;		//睡眠ゲージのメーター
	static bool isGift;				//プレゼント設置フラグ
	
	//フラグ
	static bool GameClearCheck;			//ゲームクリアフラグ
	static bool GameOverCheck;			//ゲームオーバフラグ

	//整数
	static int second;					//秒数

};
