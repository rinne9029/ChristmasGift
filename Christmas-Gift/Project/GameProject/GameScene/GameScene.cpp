#include"GameScene.h"
#include"Chara/Player/Player.h"
#include"Chara/Enemy/Enemy.h"
#include"Camera/Camera.h"
#include"Field/Field.h"
#include"Field/FieldObject/Closet.h"
#include"Field/FieldObject/Telephone.h"
#include"CollisionHitBox/MidPoint.h"
#include"CollisionHitBox/EndPoint.h"
#include"UI/SleepLife.h"
#include"UI/Timer.h"
#include"Result/GameClear.h"
#include"Result/GameOver.h"
#include"GameData.h"

//コンストラクタ
GameScene::GameScene()
	:Task(ETaskTag::eScene,true)
{
	//ゲーム状況のリセット
	GameData::isGift = false;
	GameData::FacePosition = 340;
	GameData::GameClearCheck = false;
	GameData::GameOverCheck = false;
	GameData::second = 300;
	GameData::GameStart = true;

	//フェードイン実行
	GameData::StartFadeIn = true;

	//ステージ生成
	//テキストデータによってステージ内オブジェクトが変化
	new Field
	(
		"TextData/StageNode1.txt",		//ノード
		"TextData/LightData1.txt",		//ライト
		"TextData/DoorData1.txt",		//ドア
		"TextData/SwitchData1.txt"		//スイッチ
	);

	//プレイヤー生成
	new Player
	(
		CVector3D(0, 0, 0),
		CVector3D(0, DtoR(180), 0),	
		CVector3D(0, 0, 0)
	);

	//敵：父親生成
	new Enemy
	(
		CVector3D(2.9, 0, 0.351914),
		CVector3D(-1, 0, 0),
		CVector3D(0.01, 0.01, 0.01),
		0
	);

	//敵：母親生成
	new Enemy
	(
		CVector3D(-2.073762, 9.891998, 1.103694),
		CVector3D(-1, 0, 0),
		CVector3D(0.01, 0.01, 0.01),
		1
	);


	new MidPoint
	(
		CVector3D(5.998150, 9.8, -1.6),
		CVector3D(0, 0, 0),
		CVector3D(1, 0.5, 1)
	);

	new EndPoint
	(
		CVector3D(4.555758, 0, 6.188184),
		CVector3D(0, 0, 0),
		CVector3D(1, 1, 1)
	);

	new Closet
	(
		CVector3D(3.261635, 9.053699, 4.970999),			//座標
		CVector3D(0, DtoR(270), 0),		//回転値
		CVector3D(1, 0.7, 1),				//オブジェクトの大きさ
		2
	);

	new Closet
	(
		CVector3D(0, 0, 0),			//座標
		CVector3D(0, DtoR(270), 0),		//回転値
		CVector3D(1, 0.7, 1),				//オブジェクトの大きさ
		2
	);

	//new Closet
	//(
	//	CVector3D(0.261635, 9.053699, 2.970999),			//座標
	//	CVector3D(0, DtoR(180), 0),		//回転値
	//	CVector3D(1.2, 1.2, 2),				//オブジェクトの大きさ
	//	2
	//);

	new SleepLife();			//睡眠ゲージ
	new Timer();				//制限時間	

}

//デストラクタ
GameScene::~GameScene()
{
	//ゲームクリアフラグOn
	if (GameData::GameClearCheck)
	{
		SOUND("SE_Walk")->Stop();
		SOUND("SE_Run")->Stop();
		//クリアシーン移行
		new GameClear();
	}
	//ゲームオーバーフラグOn
	else if (GameData::GameOverCheck)
	{
		SOUND("SE_Walk")->Stop();
		SOUND("SE_Run")->Stop();
		//ゲームオーバーシーン移行
		new GameOver();
	}
}

//更新処理
void GameScene::Update()
{
	//5分経過でゲームオーバー
	if (GameData::second < 0)
	{
		//ゲームオーバーフラグOn
		GameData::GameOverCheck = true;
		//フェードアウト実行
		GameData::StartFadeOut = true;
	}
}