#include"GameScene.h"
#include"Chara/Player/Player.h"
#include"Chara/Enemy/Enemy.h"
//#include"Chara/Enemy/EnemyManager.h"
#include"Camera/Camera.h"
#include"Field/Field.h"
#include"Field/FieldObject/Closet.h"
#include"Field/FieldObject/Telephone.h"
#include"Light/Light.h"
#include"CollisionHitBox/CollisionHitBox.h"
#include"Title/Title.h"
#include"Filta/Filta.h"
#include"CollisionHitBox/MidPoint.h"
#include"CollisionHitBox/EndPoint.h"
#include"Navigation/NavManager.h"
#include"UI/SleepLife.h"
#include"UI/ToolTips.h"
#include"GameData.h"
#include"Item/PresentBox.h"
#include"Field/FieldObject/Switch.h"

GameScene::GameScene()
	:Task(ETaskTag::eScene,true)
	,GameCount(0)
	,mp_filta(nullptr)
	,mp_player(nullptr)
{
	GameData::isGift = false;

	FILE* fp = NULL;
	S_STATUS  character[] = { CVector3D(0,0,0) };
	int i;

	//	セーブデータをバイナリの読み込みでオープン
	fopen_s(&fp, "SaveData.bin", "rb");

	//	キャラクターのステータスをファイルから復元(キャラクター:3人)
	fread(character, sizeof(S_STATUS), sizeof(character) / sizeof(character[0]), fp);

	


	//ゲームスタート時はA_1ステージ
	switch (GameData::Stage)
	{
	//マップA_1のシーン
	case GameData::A_1:
		//ステージ
		new Field();

		new Switch
		(
			CVector3D(1.5, 1.5, 7),
			CVector3D(0, 0, 0),
			CVector3D(1, 1, 1),
			0
		);

		new Switch
		(
			CVector3D(3, 1.5, 7),
			CVector3D(0, 0, 0),
			CVector3D(1, 1, 1),
			0
		);

		//カメラ
		new Camera
		(
			CVector3D(0, 1.5, 0),							//座標
			CVector3D(0, DtoR(180), 0)						//回転値
		);

		//サンタ
		new Player
		(
			CVector3D(0, 0, 0),
			character[0].size//モデルサイズ
		);

		//エネミー
		/*new Enemy
		(
			CVector3D(2.369029,0.1,-0.000122),
			CVector3D(0,DtoR(180),0),
			CVector3D(0.01,0.01,0.01)
		);*/

		new MidPoint
		(
			CVector3D(5.998150, 10.053699, 0.179571),
			CVector3D(0, 0, 0),
			CVector3D(1, 1, 1)
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
			CVector3D(0, DtoR(180), 0),		//回転値
			CVector3D(1.2, 1.2, 2),				//オブジェクトの大きさ
			2
		);

		new Closet
		(
			CVector3D(6.261635, 9.053699, 8.970999),			//座標
			CVector3D(0, DtoR(180), 0),		//回転値
			CVector3D(1.2, 1.2, 2),				//オブジェクトの大きさ
			3
		);

		new Telephone
		(
			CVector3D(0, 0, 0),	//座標
			CVector3D(0, DtoR(270), 0),					//回転値
			CVector3D(3, 3, 3)							//オブジェクトの大きさ
		);

		new Telephone
		(
			CVector3D(3, 0, 0),	//座標
			CVector3D(0, DtoR(270), 0),					//回転値
			CVector3D(3, 3, 3)							//オブジェクトの大きさ
		);

		new SleepLife();			//睡眠ゲージ

		break;

	//マップA_子供部屋のシーン
	//case GameData::A_Child:
	//			//ステージ
	//			//new ChildRoom();

	//			//カメラ
	//			new Camera
	//			(
	//				CVector3D(0, 1.5, 0),
	//				CVector3D(0,DtoR(180),0)
	//			);

	//			//サンタ
	//			new Player
	//			(
	//				CVector3D(-30, 0, -30),			//座標
	//				CVector3D(0, DtoR(0), 0),		//回転値
	//				CVector3D(0.01, 0.01, 0.01)		//モデルサイズ
	//			);

	//	break;

	//マップB_1のシーン
	case GameData::B_1:
				//ステージ
				new Field();

				//カメラ
				new Camera
				(
					CVector3D(0, 1.5, 0),
					CVector3D(0,DtoR(-125),0)
				);

				//サンタ
				new Player
				(
					CVector3D(34.5, 0, 30),			//座標
					CVector3D(0.01, 0.01, 0.01)		//モデルサイズ
				);

				//エネミー
				/*new Enemy
				(
					CVector3D(-30, 0, 30),
					CVector3D(0.01, 0.01, 0.01)
				);*/

				/*for (int i = 0; i < 4; i++)
				{
					new Closet
					(
						CVector3D(31 - 2 * i, -0.1, 31),
						CVector3D(0, DtoR(180), 0),
						CVector3D(0.9, 3, 0.9)
					);
				}
				

				for (int i = 0; i < 4; i++)
				{
					new Closet
					(
						CVector3D(31 - 2 * i, -0.1, 7),
						CVector3D(0, DtoR(0), 0),
						CVector3D(0.9, 3, 0.9)
					);
				}*/

				//ワープ用ボックス
				/*new WarpRoomBox
				(
					CVector3D(34.5, 0, 30),
					CVector3D(0, 0, 0),
					CVector3D(1, 0.5, 0.5)
				);*/

				new SleepLife();			//睡眠ゲージ
		break;
	
	//マップB_２のシーン
	case GameData::B_2:
		//ステージ
		new Field();

		//カメラ
		new Camera
		(
			CVector3D(0, 1.5, 0),
			CVector3D(0, DtoR(0), 0)
		);

		//サンタ
		new Player
		(
			CVector3D(34.5, 0, -42),			//座標
			CVector3D(0.01, 0.01, 0.01)		//モデルサイズ
		);

		//エネミー
		/*new Enemy
		(
			CVector3D(-30, 0, 30),
			CVector3D(0.01, 0.01, 0.01)
		);*/

		//クローゼット
		//new Closet
		//(
		//	CVector3D(19, -0.1, 5.6),			//座標
		//	CVector3D(0, DtoR(180), 0),		//回転値
		//	CVector3D(0.9, 3, 0.9)			//当たり判定サイズ
		//);

		//new Closet
		//(
		//	CVector3D(-18, -0.1, -19),
		//	CVector3D(0, DtoR(90), 0),
		//	CVector3D(0.9, 3, 0.9)
		//);

		//new Closet
		//(
		//	CVector3D(-18, -0.1, -19),
		//	CVector3D(0, DtoR(90), 0),
		//	CVector3D(0.9, 3, 0.9)
		//);

		//ワープ用ボックス
		/*new WarpRoomBox
		(
			CVector3D(34.5, 0, -42),
			CVector3D(0, 0, 0),
			CVector3D(1, 0.5, 0.5)
		);*/

		new SleepLife();			//睡眠ゲージ
		break;
	}
	mp_player = dynamic_cast<Player*>(TaskManager::FindObject(ETaskTag::ePlayer));
	mp_filta = dynamic_cast<Filta*>(TaskManager::FindObject(ETaskTag::eFilta));
	//フェードイン実行
	mp_filta->m_FadeinCheck = true;
	
	fclose(fp);
}

GameScene::~GameScene()
{
	//プレイヤーが死亡or勝利状態なら
	if (mp_player->m_CheckKill == true || GameData::isGift == true)
	{
		new Title();
	}
	else
	{
		new GameScene();
	}
	
}

void GameScene::Update()
{
	GameCount++;
	//５分経過でタイトル画面へ
	if (GameCount > 18000)
	{
		mp_player->m_CheckKill = true;
		TaskManager::KillALL();
	}
}