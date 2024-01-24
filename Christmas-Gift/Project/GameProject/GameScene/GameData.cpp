#include"GameData.h"

int GameData::Stage = GameData::A_1;		//初期スタートの家
bool GameData::Start = false;				//ゲームスタートフラグ
bool GameData::ChengeStage = false;			//マップ移動フラグ
float GameData::BlueSleepSize = 300;
bool GameData::isGift = false;				//プレゼント設置フラグ
int GameData::second = 0;					//タイマー