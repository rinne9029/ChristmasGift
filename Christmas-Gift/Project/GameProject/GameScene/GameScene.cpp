#include"GameScene.h"
#include"Chara/Player/Player.h"
#include"Chara/Enemy/Enemy.h"
#include"Camera/Camera.h"
#include"Field/Field.h"
#include"Field/FieldObject/Closet.h"
#include"Field/FieldObject/Telephone.h"
#include"CollisionHitBox/MidPoint.h"
#include"UI/SleepLife.h"
#include"UI/Timer.h"
#include"UI/Pause.h"
#include"Title/Title.h"
#include"UI/TargetText.h"
#include"Effect/Destination.h"
#include"Result/GameClear.h"
#include"Result/GameOver.h"

//コンストラクタ
GameScene::GameScene(int stage)
	:Task(ETaskTag::eScene,true)
{
	//ゲーム状況のリセット
	GameData::isGift = false;
	GameData::FacePosition = 340;
	GameData::GameClearCheck = false;
	GameData::GameOverCheck = false;
	GameData::second = GAME_TIME;
	GameData::GameStart = true;

	m_stage = stage;

	//フェードイン実行
	GameData::StartFadeIn = true;

	//ステージ生成
	switch (stage)
	{
	//ステージ１生成
	case 1:
	{
		//テキストデータによってステージ内オブジェクトが変化
		new Field
		(
			"TextData/StageNode1.txt",		//ノード
			"TextData/LightData1.txt",		//ライト
			"TextData/DoorData1.txt",		//ドア
			"TextData/SwitchData1.txt",		//スイッチ
			"TextData/ClosetData1.txt"		//クローゼット
		);

		//プレイヤー生成
		new Player
		(
			CVector3D(4.55, -0.2, 6.18),
			CVector3D(0, DtoR(180), 0),
			CVector3D(0.01, 0.01, 0.01)
		);

		

		//エフェクト生成
		/*new Destination
		(
			COPY_RESOURCE("Star", CModelObj),
			CVector3D(4.55, 1, 6.18),
			CVector3D(0.1, 0.1, 0.1)
		);*/

		
		

		//敵：父親生成
		/*new Enemy
		(
			CVector3D(2.9, 0, 0.351914),
			CVector3D(-1, 0, 0),
			CVector3D(0.01, 0.01, 0.01),
			0
		);*/

		////敵：母親生成
		//new Enemy
		//(
		//	CVector3D(-2.073762, 9.891998, 1.103694),
		//	CVector3D(-1, 0, 0),
		//	CVector3D(0.01, 0.01, 0.01),
		//	1
		//);

		//プレゼント設置ポイント
		new MidPoint
		(
			CVector3D(5.998150, 9.0, -1.6),
			CVector3D(0, 0, 0),
			CVector3D(1, 0.5, 1)
		);

		new SleepLife();			//睡眠ゲージ
		new Timer();				//制限時間
		new TargetText();			//現在の目標
	}
	break;
	//ステージ２生成
	case 2:
	{

	}
	break;
	//ステージ３生成
	case 3:
	{

	}
	break;
	}
	new Pause();		//ポーズ画面
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
		new GameOver(m_stage);
	}
	//それ以外
	else
	{
		SOUND("SE_Walk")->Stop();
		SOUND("SE_Run")->Stop();
		//ゲームオーバーシーン移行
		new Title();
	}
}

//更新処理
void GameScene::Update()
{
	//5分経過でゲームオーバー
	if (GameData::second == 0)
	{
		//ゲームオーバーフラグOn
		GameData::GameOverCheck = true;
		//フェードアウト実行
		GameData::StartFadeOut = true;
	}
}
