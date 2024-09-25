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
#include"UI/DangerFilta.h"
#include"Effect/Destination.h"
#include"Result/GameClear.h"
#include"Result/GameOver.h"

#define DANGER_TIME 240.0f

//コンストラクタ
GameScene::GameScene(int stage)
	:Task(ETaskTag::eScene,true)
	,m_dangerfilta(NULL)
{
	//ゲーム状況のリセット
	GameData::isGift = false;
	GameData::FacePosition = FACE_POSITION;
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
			CVector3D(4.55f, -0.2f, 6.18f),
			CVector3D(0.0f, DtoR(180), 0.0f),
			CVector3D(0.01f, 0.01f, 0.01f)
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

		//プレゼント設置ポイント
		new MidPoint
		(
			CVector3D(5.998150f, 9.0f, -1.6f),
			CVector3D(0.0f, 0.0f, 0.0f),
			CVector3D(1.0f, 0.5f, 1.0f)
		);

		
		new SleepLife();			//睡眠ゲージ
		new Timer();				//制限時間
		new TargetText();			//現在の目標

	}
	break;
	//ステージ２生成
	case 2:
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
			CVector3D(4.55f, -0.2f, 6.18f),
			CVector3D(0.0f, DtoR(180), 0.0f),
			CVector3D(0.01f, 0.01f, 0.01f)
		);

		//プレゼント設置ポイント
		new MidPoint
		(
			CVector3D(5.998150f, 9.0f, -1.6f),
			CVector3D(0.0f, 0.0f, 0.0f),
			CVector3D(1.0f, 0.5f, 1.0f)
		);

		new SleepLife();			//睡眠ゲージ
		new Timer();				//制限時間
		new TargetText();			//現在の目標

		//敵：父親生成
		new Enemy
		(
			CVector3D(2.9, 0, 0.351914),
			CVector3D(-1, 0, 0),
			CVector3D(0.01, 0.01, 0.01),
			0
		);

		////敵：母親生成
		new Enemy
		(
			CVector3D(-2.073762, 9.891998, 1.103694),
			CVector3D(-1, 0, 0),
			CVector3D(0.01, 0.01, 0.01),
			1
		);
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
	if (GameData::second < DANGER_TIME && m_dangerfilta == NULL)
	{
		m_dangerfilta = new DangerFilta();		//危険状態を伝えるフィルター
	}
	//5分経過でゲームオーバー
	if (GameData::second == 0.0f)
	{
		//ゲームオーバーフラグOn
		GameData::GameOverCheck = true;
		//フェードアウト実行
		GameData::StartFadeOut = true;
	}
}
