#include"Player.h"
#include"Camera/Camera.h"
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

//マクロ
#define JUMP 0.2f			//ジャンプ力
#define WALK_SPEED 0.04f	//通常スピード
#define DOWN_SPEED 0.02f	//しゃがみスピード
#define RUN_SPEED 0.07f		//走りスピード
#define HEIGHT 1.9f			//高さ
#define RAD 0.4f			//半径


//コンストラクタ
Player::Player(const CVector3D& pos,const CVector3D& rot, const CVector3D& scale)
	:CharaBase(ETaskTag::ePlayer, true)
	, mp_switch(nullptr)
	, m_tooltips(nullptr)
	, key_ang(0.0f)				
	, m_hide(false)											
	, m_state(eState_Idle)		
{
	//プレイヤーモデル読み込み
	m_model = COPY_RESOURCE("Player", CModelA3M);

	m_pos = pos;
	m_scale = scale;

	//当たり判定の設定
	m_height = HEIGHT;
	m_rad = RAD;

	//プレイヤーと同じ方向のカメラ作成
	m_camera = new Camera(rot);

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
	m_camera->m_state = m_camera->eState_Idle;
	//スペースボタン入力
	if (PUSH(CInput::eButton1) && m_isGround)
	{
		//着地フラグOFF
		m_isGround = false;
		//ジャンプ
		m_vec.y += JUMP;
	}

	//コントロールボタン入力
	if (PUSH(CInput::eButton3))
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
	m_camera->m_state = m_camera->eState_Idle;
	//しゃがみスピードを代入
	m_movespeed = DOWN_SPEED;

	//コントロールボタン
	if (PUSH(CInput::eButton3))
	{
		//立ちアニメーション
		m_model.ChangeAnimation((int)AnimId::Stand);
		//通常状態へ移行
		m_state = eState_Idle;
	}
}

//ハイド状態
void Player::StateClosetIn()
{
	m_camera->m_state = m_camera->eState_ClosetIn;
	//ボタン入力で即ハイドを解除しないように
	static int count;
	count++;
	//クローゼットの中心に移動
	m_pos = m_Closet_pos;
	if (PUSH(CInput::eMouseL) && count >1)
	{
		SOUND("SE_DoorClose")->Volume(0.3);
		SOUND("SE_DoorClose")->Play();
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
	//スイッチ
	if (!mp_switch)mp_switch = dynamic_cast<Switch*>(TaskManager::FindObject(ETaskTag::eFieldObject));

	//カメラ視点
	//キー入力されてない時
	//カメラの角度でキャラクターの正面角度が決まる
	m_rot.y = Utility::NormalizeAngle(m_camera->m_rot.y + key_ang);


	//フェードイン・フェードアウト中は処理をしない
	if (GameData::StartFadeIn || GameData::StartFadeOut)return;

	//キー方向ベクトルをリセット
	CVector3D key_dir = CVector3D(0, 0, 0);

	//ハイド中はキー入力を受け付けない
	if (m_state != eState_ClosetIn)
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
				if (HOLD(CInput::eButton2))
				{
					//ダッシュ移動
					m_movespeed = RUN_SPEED;
					//レム睡眠状態なら
					if (SleepLife::m_REM) GameData::FacePosition -= 1.0f;
					//ノンレム睡眠状態なら
					else GameData::FacePosition -= 0.5f;

					//走り音が終了するまで
					if (SOUND("SE_Run")->CheckEnd())
					{
						//歩き音停止
						SOUND("SE_Walk")->Stop();
						//走り音2倍速
						SOUND("SE_Run")->Pitch(2);

						SOUND("SE_Run")->GetOffset();
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
		Shot();
	}
	//ベースクラスの更新
	if(m_state != eState_ClosetIn)
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
	case eState_ClosetIn:
		StateClosetIn();
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
}

//描画処理
void Player::Render()
{
	m_model.SetPos(m_pos);		//座標
	m_model.SetRot(m_rot);		//回転値
	m_model.SetScale(m_scale);	//大きさ
	//簡易的処理
	//一人称時モデルを描画しない
	//m_model.Render();

	if (m_state == eState_ClosetIn) return;
	//レイの描画（デバッグ）
	//Utility::DrawLine(m_lS, m_lE, CVector4D(1, 0, 0, 1), 10000);
	//オブジェクトを選択するカーソル
	Utility::DrawQuad(CVector2D(954, 510), CVector2D(12, 12), CVector4D(0, 1, 0, 1));
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
	case ETaskTag::eDoor:
	{
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(t->m_CollisionObb, m_lineS, m_lineE, m_rad, &axis, &dist))
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
	CVector3D dir = CMatrix::MRotation(m_camera->m_rot).GetFront();
	//レイの始点
	CVector3D lineS = m_pos + m_camera->m_pos;

	//レイの終点
	CVector3D lineE = m_pos + m_camera->m_pos + dir * range;

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
			if (o->CollisionRay(lineS, lineE, &c, &n))
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
		//ツールチップ作成
		if (m_tooltips == nullptr)m_tooltips = new ToolTips();
		//当たっているオブジェクトのナンバーに応じて処理を変更
		switch (hit_object->m_objectno)
		{
		case 0:
		{
			m_tooltips->m_Text = "";
			//左クリックで敵を集める
			if (PUSH(CInput::eMouseL))
			{

			}
		}
		break;
		case 1:
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
							m_tooltips->m_Text = "電気を消す";
							//左クリックで電気を消す
							if (PUSH(CInput::eMouseL))
							{
								l->m_islight = false;
							}
						}
						else if (!l->m_islight && mp_switch->m_SwitchNo == l->m_roomNo)
						{
							m_tooltips->m_Text = "電気をつける";
							if (PUSH(CInput::eMouseL))
							{
								l->m_islight = true;
							}
						}
					}
				}
			}
		}
		break;
		//クローゼット
		case 2:
		{
			if (m_state == eState_ClosetIn)
			{
				m_tooltips->m_Text = "出る";
			}
			else
			{
				m_tooltips->m_Text = "隠れる";
			}
			

			
			//左クリックで隠れる
			if (PUSH(CInput::eMouseL) && m_state == eState_Idle)
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
					m_state = eState_ClosetIn;
					//音声再生
					SOUND("SE_DoorOpen")->Volume(0.3);
					SOUND("SE_DoorOpen")->Play();
				}
			}

		}
		break;
		}
	}
	else
	{
		if (m_tooltips != nullptr)
		{
			m_tooltips->m_Iskill = true;
			m_tooltips = nullptr;
		}
	}
	m_lS = lineS;
	m_lE = lineE;
}
