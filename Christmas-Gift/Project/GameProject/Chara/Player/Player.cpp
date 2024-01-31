#include"Player.h"
#include"Chara/Enemy/Enemy.h"
#include"Field/Field.h"
#include"Field/FieldObject/Closet.h"
#include"Filta/Filta.h"
#include"Field/FieldObject/GimmickObjectBase.h"
#include"Camera/Camera.h"
#include"Light/Light.h"
#include"Field/FieldObject/Switch.h"
#include"UI/SleepLife.h"
#include"UI/ToolTips.h"
#include"GameScene/GameScene.h"
#include"Navigation/NavNode.h"
#include"Navigation/NavManager.h"
#include"GameScene/GameData.h"

//マクロ
#define JUMP 0.20f			//ジャンプ力
#define WALK_SPEED 0.04f	//通常スピード
#define DOWN_SPEED 0.02f	//しゃがみスピード
#define RUN_SPEED 0.07f		//走りスピード


//コンストラクタ
Player::Player(const CVector3D& pos, const CVector3D& scale)
	:CharaBase(ETaskTag::ePlayer, true)
	, mp_enemy(nullptr)
	, mp_camera(nullptr)
	, mp_filta(nullptr)
	, mp_sleeplife(nullptr)
	, mp_light(nullptr)
	, mp_switch(nullptr)
	, m_tooltips(nullptr)
	, key_ang(0.0f)				
	, m_hide(false)											
	, m_state(eState_Idle)		
{

	FILE* fp = NULL;

	//	データをテキストの読み込みでオープン
	fopen_s(&fp, "PlayerData.txt", "r");
	if (!fp) return;

	char buf[256] = "";

	while (!feof(fp))
	{
		fgets(buf, 256, fp);
		CVector3D Pos(0, 0, 0);
		CVector3D Size(0, 0, 0);

		sscanf_s(buf, "%f %f %f %f %f %f", &Pos.x, &Pos.y, &Pos.z, &Size.x, &Size.y, &Size.z);

		m_pos = Pos;
		m_scale = Size;
	}

	fclose(fp);

	m_tooltips = new ToolTips();

	m_height = 1.9f;			//高さ
	m_rad = 0.3f;				//半径

	//デバッグ用
	m_lS = CVector3D(0, 0, 0);	//レイの始点
	m_lE = CVector3D(0, 0, 0);	//レイの終点

	//プレイヤーモデル読み込み
	m_model = COPY_RESOURCE("Player", CModelA3M);

	//プレイヤー位置に経路探索用のノードを作成
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.5f, 0.0f),
		NavNode::NodeType::Destination
	);
	//ノードのカラー選択(赤)
	m_navNode->SetNodeColor(CVector3D(1.0f, 0.25f, 0.25f));
}

//デストラクタ
Player::~Player()
{
}

//通常状態
void Player::StateIdle()
{
	//歩き速度
	

	//シフトキー入力&&移動している
	if (HOLD(CInput::eButton6))
	{
		
	}

	//スペースボタン入力
	if (PUSH(CInput::eButton5) /*&& m_isGround*/)
	{
		//着地フラグOFF
		m_isGround = false;
		//ジャンプ
		m_vec.y += JUMP;
	}

	//コントロールボタン入力
	if (PUSH(CInput::eButton7))
	{
		//しゃがみアニメーション
		m_model.ChangeAnimation((int)AnimId::Crouch);
		//しゃがみ状態に移行
		m_state = eState_Squat;
	}

}

//しゃがみ状態
void Player::StateSquat()
{
	//しゃがみスピードを代入
	m_movespeed = DOWN_SPEED;

	//隠れ状態の切り替え
	//if (m_hide) m_hide = false;

	//コントロールボタン
	if (PUSH(CInput::eButton7))
	{
		//立ちアニメーション
		m_model.ChangeAnimation((int)AnimId::Stand);
		//通常状態へ移行
		m_state = eState_Idle;
	}
}

//ハイド状態
void Player::StateHide()
{
	//ボタン入力で即ハイドを解除しないように
	static int count;
	count++;
	//クローゼットの中心に移動
	m_pos = m_Closet_pos;
	if (PUSH(CInput::eMouseL) && count >1)
	{
		//カウントの初期化
		count = 0;
		//元の位置へ戻る
		m_pos = m_copy_pos;
		//ハイド解除
		m_hide = false;
		m_state = eState_Idle;
	}
}

//更新処理
void Player::Update()
{
	//敵
	if (!mp_enemy) mp_enemy = dynamic_cast<Enemy*>(TaskManager::FindObject(ETaskTag::eEnemy));
	//カメラ
	if (!mp_camera) mp_camera = dynamic_cast<Camera*>(TaskManager::FindObject(ETaskTag::eCamera));
	//フィルター
	if (!mp_filta) mp_filta = dynamic_cast<Filta*>(TaskManager::FindObject(ETaskTag::eFilta));
	//睡眠ゲージ
	if (!mp_sleeplife) mp_sleeplife = dynamic_cast<SleepLife*>(TaskManager::FindObject(ETaskTag::eUI));
	//ライト
	if (!mp_light)mp_light = dynamic_cast<Light*>(TaskManager::FindObject(ETaskTag::eFieldLight));
	//スイッチ
	if (!mp_switch)mp_switch = dynamic_cast<Switch*>(TaskManager::FindObject(ETaskTag::eFieldObject));
	//自身の光源
	CLight::SetType(1, CLight::eLight_Point);
	CLight::SetRange(1, 1.0f);
	CLight::SetColor(1, CVector3D(0, 0, 0), CVector3D(0.8, 0.8, 0.7));
	CLight::SetPos(1, m_pos + CVector3D(0, 1.0f, 0));

	Shot();

	//カメラ視点
	//見下ろし視点
	if (mp_camera->m_camera_mode == Camera::LookDownCamera)
	{
		//キー方向ベクトルの角度でキャラクターの角度が決まる
		m_rot.y = Utility::NormalizeAngle(key_ang);
	}
	//一人称視点or三人称視点
	else
	{
		//キー入力されてない時
		//カメラの角度でキャラクターの正面角度が決まる
		m_rot.y = Utility::NormalizeAngle(mp_camera->m_rot.y + key_ang);
	}


	//フェードイン・フェードアウト中はアップデート処理をしない
	if (GameData::StartFadeIn)return;
	if (GameData::StartFadeOut)return;

	//キー方向ベクトルをリセット
	CVector3D key_dir = CVector3D(0, 0, 0);

	//ハイド中はキー入力を受け付けない
	if (m_state != eState_Hide)
	{
		//入力したキー方向のベクトルを設定する
		if (HOLD(CInput::eUp)) key_dir.z = 1;
		if (HOLD(CInput::eDown))key_dir.z = -1;
		if (HOLD(CInput::eLeft))key_dir.x = 1;
		if (HOLD(CInput::eRight))key_dir.x = -1;
	

		//キー入力されたら
		if (key_dir.LengthSq() > 0.1)
		{

			//キーの方向ベクトルを角度に逆算する
			key_ang = atan2(key_dir.x, key_dir.z);

			//方向ベクトル
			CVector3D dir(sin(m_rot.y), 0, cos(m_rot.y));

			//移動
			m_pos += dir * m_movespeed;

			//通常状態なら
			if (m_state == eState_Idle)
			{
				//通常歩きモーション
				m_model.ChangeAnimation((int)AnimId::Walk);

				//シフトキー入力
				if (HOLD(CInput::eButton6))
				{
					//ダッシュ移動
					m_movespeed = RUN_SPEED;
					//レム睡眠状態なら
					if (SleepLife::m_REM) GameData::BlueSleepSize -= 2.0f;
					//ノンレム睡眠状態なら
					else GameData::BlueSleepSize -= 1.0f;

					//走り音が終了するまで
					if (SOUND("SE_Run")->CheckEnd())
					{
						//歩き音停止
						SOUND("SE_Walk")->Stop();
						//走り音2倍速
						SOUND("SE_Run")->Pitch(2);
						//走り音再生
						SOUND("SE_Run")->Play();
					}
				}
				else
				{
					m_movespeed = WALK_SPEED;

					//歩き音終了するまで
					if (SOUND("SE_Walk")->CheckEnd())
					{
						//走り音停止
						SOUND("SE_Run")->Stop();
						//歩き音2倍速
						SOUND("SE_Walk")->Pitch(2);
						//歩き音再生
						SOUND("SE_Walk")->Play();
					}
				}
				
			}
			else
			{
				//しゃがみ歩き
				m_model.ChangeAnimation((int)AnimId::Crouchwalk);
			}
		}
		else
		{
			//待機アニメーション
			if (m_state == eState_Idle)
			{
				//通常待機
				m_model.ChangeAnimation((int)AnimId::Idle);
				//歩き音停止
				SOUND("SE_Walk")->Stop();
				SOUND("SE_Run")->Stop();
			}	
			else
			{
				//しゃがみ待機
				m_model.ChangeAnimation((int)AnimId::Crouchidle);
				SOUND("SE_Walk")->Stop();
				SOUND("SE_Run")->Stop();
			}

		}
	}
	//ベースクラスの更新
	CharaBase::Update();

	//ステート状態による状態変化
	switch (m_state) {
		//通常モード
	case eState_Idle:
		StateIdle();
		break;
		//しゃがみモード
	case eState_Squat:
		StateSquat();
		break;
		//ハイドモード
	case eState_Hide:
		StateHide();
		break;
	}

	if (m_state != eState_Squat)
	{
		m_navNode->m_pos = m_pos + CVector3D(0, 1.5, 0);
	}
	else
	{
		m_navNode->m_pos = m_pos + CVector3D(0, 0.5, 0);
	}

	NavNode* node = NavManager::Instance()->GetNearNavNode(m_navNode);

	//アニメーション更新
	m_model.UpdateAnimation();


	//プレイヤーカプセルの表示
	//Utility::DrawCapsule(m_lineS, m_lineE, m_rad, CVector4D(1, 0, 0, 1));

	//デバッグ用:kボタンでプレイヤーの座標を表示（ノード配置調整用）
	if (PUSH(CInput::eButton12))
	{
		printf("CVector3D(%f,%f,%f),\n", m_pos.x,m_pos.y+1,m_pos.z);
	}
}

//描画処理
void Player::Render()
{
	m_model.SetPos(m_pos);		//座標
	m_model.SetRot(m_rot);		//回転値
	m_model.SetScale(m_scale);	//大きさ
	//簡易的処理
	//一人称時モデルを描画しない
	if (mp_camera->m_camera_mode != Camera::FirstPersonCamera) {
		m_model.Render();
	}

	//デバッグ用:レイの線を表示
	float a = 10000;
	Utility::DrawLine(m_lS, m_lE, CVector4D(1, 0, 0, 1), a);
}

//衝突処理
void Player::Collision(Task* t)
{
	CharaBase::Collision(t);

	switch (t->GetTag())
	{
	case ETaskTag::eEnemy:
	{
		//プレイヤーと敵の判定
		CVector3D c1, dir1, c2, dir2;
		float dist;
		if (CCollision::CollisionCapsule(m_lineS, m_lineE, m_rad,
			t->m_lineS, t->m_lineE, t->m_rad,
			&dist, &c1, &dir1, &c2, &dir2))
		{
				//ゲームオーバ
				GameData::GameOverCheck = true;
				//フェードアウト実行
				GameData::StartFadeOut = true;
		}
	}
	break;
	case ETaskTag::eFieldObject:
	{
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(t->m_obb1, m_lineS, m_lineE, m_rad, &axis, &dist))
		{
			if (axis.y > 0.5f)
			{
				//面が上向き->地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
			}
			//ハイド以外
			if (m_state != eState_Hide)
			{
				//押し戻し
				float s = m_rad - dist;
				m_pos += axis * s;
			}
		}
	}
	break;
	case ETaskTag::eDoor:
	{
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(t->m_obb1, m_lineS, m_lineE, m_rad, &axis, &dist))
		{
			if (axis.y > 0.5f)
			{
				//面が上向き->地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
			}
			//押し戻し
			float s = m_rad - dist;
			m_pos += axis * s;
		}
	}
	break;
	}
}

//レイの処理
void Player::Shot()
{
	//判定するレイの距離
	const float range = 1.5f;
	CVector3D dir = CMatrix::MRotation(mp_camera->m_rot).GetFront();
	//レイの始点
	CVector3D lineS = m_pos + mp_camera->m_pos;

	//レイの終点
	CVector3D lineE = m_pos + mp_camera->m_pos + dir * range;

	//最も近いオブジェクトへの距離
	float dist = FLT_MAX;
	//レイとの衝突点
	CVector3D hit_field_point;
	//衝突したステージオブジェクト
	Field* hit_field = nullptr;
	if (Field* f = dynamic_cast<Field*>(TaskManager::FindObject(ETaskTag::eField))) 
	{
		//接触面の法線（使わない）
		CVector3D n;
		if (f->GetColModel()->CollisionRay(&hit_field_point, &n, lineS, lineE))
		{
			//発射位置から接触面への距離
			dist = (hit_field_point - lineS).LengthSq();
			//接触したステージを更新
			hit_field = f;
		}
	}
	//衝突したオブジェクト
	GimmickObjectBase* hit_object = nullptr;
	//全オブジェクトを探索
	auto list = TaskManager::FindObjects(ETaskTag::eFieldObject);
	for (auto t : list) 
	{
		if (GimmickObjectBase* o = dynamic_cast<GimmickObjectBase*>(t))
		{
			//レイとの衝突地点
			CVector3D c;
			CVector3D n;
			//弾の線分でオブジェクトとの判定を行う
			if(o->CollisionRay(lineS,lineE,&c,&n))
			{
				//発射位置から最も近いオブジェクトを調べる
				float l = (c - lineS).LengthSq();
					if (dist > l) 
					{
						dist = l;
							hit_object = o;
					}
			}
		}
		//接触したオブジェクトが見つかれば探索をやめる
		if (hit_object != nullptr)	break;
			
	}
	//最も近いオブジェクトに当たる
	if (hit_object)
	{
		//ツールチップを表示
		m_tooltips->isDraw = true;
		
		//当たっているオブジェクトのナンバーに応じて処理を変更
		switch (hit_object->m_objectno)
		{
		case 0:
		{
			//左クリックで敵を集める
			if (PUSH(CInput::eMouseL))
			{
				
			}
		}
		break;
		case 1:
		{	
			//左クリックで電気を消す
			if (PUSH(CInput::eMouseL)) 
			{
				if (mp_switch = dynamic_cast<Switch*>(hit_object))
				{
					auto lightlist = TaskManager::FindObjects(ETaskTag::eFieldLight);
					for (auto t : lightlist)
					{
						if (Light* l = dynamic_cast<Light*>(t))
						{
							if (l->m_islight && mp_switch->m_SwitchNo == l->m_roomNo)
							{
								l->m_islight = false;
							}
							else if (!l->m_islight && mp_switch->m_SwitchNo == l->m_roomNo)
							{
								l->m_islight = true;
							}
						}
					}
				}
			}
			
		}
		break;
		case 2:
		{
			//左クリックで隠れる
			if (PUSH(CInput::eMouseL))
			{
				//触れたクローゼットの座標を保存
				m_Closet_pos = hit_object->m_pos;

				//触れたクローゼットの正面ベクトルを保存
				m_Closet_rot = hit_object->m_rot;

				if (PUSH(CInput::eMouseL))
				{
					//現在地保存
					m_copy_pos = m_pos;
					//ハイド状態へ移行
					m_hide = true;
					m_state = eState_Hide;

				}
			}
			
		}
		break;
		}
	}
	else
	{
		m_tooltips->isDraw = false;
	}
	m_lS = lineS;
	m_lE = lineE;
}