#include"GameScene.h"
#include"Chara/Player/Player.h"
#include"Chara/Enemy/Enemy.h"
//#include"Chara/Enemy/EnemyManager.h"
#include"Camera/Camera.h"
#include"Field/Field.h"
#include"Field/Closet.h"
#include"Field/GimmickObject.h"
#include"CollisionHitBox/CollisionHitBox.h"
#include"Title/Title.h"
#include"Filta/Filta.h"
#include"Navigation/NavManager.h"
#include"UI/UI.h"
#include"GameData.h"

GameScene::GameScene()
	:Task(ETaskTag::eScene,true)
	,GameCount(0)
	,mp_filta(nullptr)
	,mp_player(nullptr)
{
	//m_remove = true;

	//ゲームスタート時はA_1ステージ
	switch (GameData::Stage)
	{
	//マップA_1のシーン
	case GameData::A_1:
		//ステージ
		new Field();

		//カメラ
		new Camera
		(
			CVector3D(0, 1.5, 0),							//座標
			CVector3D(0, DtoR(90), 0)						//回転値
		);

		//サンタ
		new Player
		(
			CVector3D(-3.702046f, -0.589298f, 10.902628f),			//座標
			CVector3D(0.01, 0.01, 0.01)						//モデルサイズ
		);

		new GimmickObject
		(
			CVector3D(0.342562, 0.231580, 10.509188),	//座標
			CVector3D(0, DtoR(270), 0),					//回転値
			CVector3D(3, 3, 3),							//オブジェクトの大きさ
			CVector3D(0.25,0.1,0.25),							//obbの大きさ
			0
		);
		//エネミー
		/*new Enemy
		(
			CVector3D(15.687225, 0.420055, 4.950093),
			CVector3D(0.01,0.01,0.01)
		);*/

		//クローゼット
		new Closet
		(
			CVector3D(19, -0.1, 5.6),			//座標
			CVector3D(0, DtoR(180), 0),		//回転値
			CVector3D(0.9, 3, 0.9)			//当たり判定サイズ
		);

		new Closet
		(
			CVector3D(0.8, -0.1, 19),
			CVector3D(0, DtoR(90), 0),
			CVector3D(0.9, 3, 0.9)
		);

		new Closet
		(
			CVector3D(-18, -0.1, -19),
			CVector3D(0, DtoR(90), 0),
			CVector3D(0.9, 3, 0.9)
		);

		new WarpRoomBox
		(
			CVector3D(34.5, 0, -42),
			CVector3D(0, 0, 0),
			CVector3D(1, 0.5, 0.5)
		);

		/*new WarpChildRoomBox
		(
			CVector3D(10, 0, 10),
			CVector3D(0, 0, 0),
			CVector3D(1, 2, 1)
		);*/

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
				new Enemy
				(
					CVector3D(-30, 0, 30),
					CVector3D(0.01, 0.01, 0.01)
				);

				for (int i = 0; i < 4; i++)
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
				}

				//ワープ用ボックス
				new WarpRoomBox
				(
					CVector3D(34.5, 0, 30),
					CVector3D(0, 0, 0),
					CVector3D(1, 0.5, 0.5)
				);

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
		new Enemy
		(
			CVector3D(-30, 0, 30),
			CVector3D(0.01, 0.01, 0.01)
		);

		//クローゼット
		new Closet
		(
			CVector3D(19, -0.1, 5.6),			//座標
			CVector3D(0, DtoR(180), 0),		//回転値
			CVector3D(0.9, 3, 0.9)			//当たり判定サイズ
		);

		new Closet
		(
			CVector3D(-18, -0.1, -19),
			CVector3D(0, DtoR(90), 0),
			CVector3D(0.9, 3, 0.9)
		);

		new Closet
		(
			CVector3D(-18, -0.1, -19),
			CVector3D(0, DtoR(90), 0),
			CVector3D(0.9, 3, 0.9)
		);

		//ワープ用ボックス
		new WarpRoomBox
		(
			CVector3D(34.5, 0, -42),
			CVector3D(0, 0, 0),
			CVector3D(1, 0.5, 0.5)
		);

		new SleepLife();			//睡眠ゲージ
		break;
	}
	mp_player = dynamic_cast<Player*>(TaskManager::FindObject(ETaskTag::ePlayer));
	mp_filta = dynamic_cast<Filta*>(TaskManager::FindObject(ETaskTag::eFilta));
	//フェードイン実行
	mp_filta->m_FadeinCheck = true;
	
}

GameScene::~GameScene()
{
	//プレイヤーが死亡or勝利状態なら
	if (mp_player->m_CheckKill == true)
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
		TaskManager::KillALL();
	}
}