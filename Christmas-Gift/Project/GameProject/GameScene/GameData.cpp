#include"GameData.h"

int GameData::Stage = GameData::A_1;		//初期スタートの家
bool GameData::ChengeStage = false;			//マップ移動フラグ
bool GameData::isGift = false;				//プレゼント設置フラグ

bool GameData::GameClearCheck = false;
bool GameData::GameOverCheck = false;
bool GameData::StartFadeIn = false;
bool GameData::StartFadeOut = false;
bool GameData::GameStart = false;
bool GameData::isPauseGame = false;

float GameData::second = 0;					//タイマー

float GameData::FacePosition = 340;			//顔UI配置