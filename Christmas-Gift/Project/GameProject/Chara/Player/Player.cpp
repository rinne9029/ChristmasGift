#include"Player.h"
#include"Field/Field.h"
#include"Field/Closet.h"
#include"Filta/Filta.h"
#include"Camera/Camera.h"
#include"UI/UI.h"
#include"GameScene/GameScene.h"
#include"Navigation/NavNode.h"
#include"Navigation/NavManager.h"
#include"GameScene/GameData.h"
#include"Debug/DebugPrint.h"
#include"Debug/DebugProfiler.h"

//ジャンプ力
#define JUMP 0.30f

#define DownSpeed 0.20f

Player::Player(const CVector3D& pos, const CVector3D& scale)
	:CharaBase(ETaskTag::ePlayer)
	, mp_camera(nullptr)
	, mp_filta(nullptr)
	, mp_sleeplife(nullptr)
	, m_hide(false)
	, m_isFootFall(false)
	, m_CheckKill(false)
	, m_Speed(0)
	, key_ang(0.0f)
	, m_copy_pos(0, 0, 0)
	, m_HideCount(0)
	, m_HideAnim(false)
	, m_state(eState_Idle)
{
	m_remove = true;
	m_pos = pos;				//プレイヤー初期座標
	m_scale = scale;			//プレイヤー大きさ
	m_height = 1.9f;			//高さ
	m_rad = 0.3f;				//半径

	//プレイヤーモデル読み込み
	m_model = COPY_RESOURCE("Player", CModelA3M);

	//プレイヤー位置に経路探索用のノードを作成
	m_navNode = new NavNode
	(
		m_pos + CVector3D(0.0f, 1.0f, 0.0f),
		NavNode::NodeType::Destination
	);
	//ノードのカラー選択(赤)
	m_navNode->SetNodeColor(CVector3D(1.0f, 0.25f, 0.25f));

}

Player::~Player()
{
}

void Player::StateIdle()
{
	

	//歩き速度
	m_Speed = 0.15f;

	//シフトキー入力&&移動している
	if (HOLD(CInput::eButton6) && m_isFootFall)
	{
		//ダッシュ移動
		m_Speed = 0.3f;
		GameData::BlueSleepSize -= 1.0f;
	}

	//スペースボタン入力
	if (PUSH(CInput::eButton5) /*&& m_isGround*/)
	{
		//ジャンプ中のフラグ
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

	//隠れ状態の切り替え
	if (m_hide) m_hide = false;

}

void Player::StateSquat()
{
	//しゃがみスピードを代入
	m_Speed = 0.02f;

	//隠れ状態の切り替え
	if (m_hide) m_hide = false;

	//コントロールボタン
	if (PUSH(CInput::eButton7))
	{
		//通常状態へ移行
		m_model.ChangeAnimation((int)AnimId::Stand);
		m_state = eState_Idle;
	}
}

void Player::StateHide()
{
	//隠れている
	m_hide = true;

	m_pos = m_Closet_pos;

}

//更新処理
void Player::Update()
{
	//カメラ
	if (!mp_camera) mp_camera = dynamic_cast<Camera*>(TaskManager::FindObject(ETaskTag::eCamera));
	//フィルター
	if (!mp_filta) mp_filta = dynamic_cast<Filta*>(TaskManager::FindObject(ETaskTag::eFilta));
	//睡眠ゲージ
	if (!mp_sleeplife) mp_sleeplife = dynamic_cast<SleepLife*>(TaskManager::FindObject(ETaskTag::eUI));

	//当たり判定
	m_lineS = m_pos + CVector3D(0, m_height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);

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
	if (mp_filta->m_FadeinCheck)return;
	if (mp_filta->m_FadeoutCheck)return;

	//足音がでてない
	m_isFootFall = false;

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
			//足音が出ている
			m_isFootFall = true;

			//キーの方向ベクトルを角度に逆算する
			key_ang = atan2(key_dir.x, key_dir.z);

			//方向ベクトル
			CVector3D dir(sin(m_rot.y), 0, cos(m_rot.y));

			//移動
			m_pos += dir * m_Speed;

			//移動アニメーション
			if (m_state == eState_Idle)
			{
				//通常歩き
				m_model.ChangeAnimation((int)AnimId::Walk);
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
			}	
			else
			{
				//しゃがみ待機
				m_model.ChangeAnimation((int)AnimId::Crouchidle);
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
	if (mp_camera->m_camera_mode != Camera::FirstPersonCamera) {
		m_model.Render();
	}

}

void Player::Collision(Task* t)
{
	//std::string name = "Collision";
	//DebugProfiler::StartTimer(name);
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

			//フェードアウトが終了した時にゲーム終了
			if (!mp_filta->m_FadeoutCheck && m_CheckKill)
			{
				GameData::BlueSleepSize = 300;
				TaskManager::KillALL();
			}

			//接触したらプレイヤー死亡
			m_CheckKill = true;

			//フェードアウトを実行
			mp_filta->m_FadeoutCheck = true;
		}
	}
	break;
	case ETaskTag::eHideBox:
	{
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(t->m_obb, m_lineS, m_lineE, m_rad, &axis, &dist))
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
			//触れたクローゼットの座標を保存
			m_Closet_pos = t->m_pos;

			//触れたクローゼットの正面ベクトルを保存
			m_Closet_rot = t->m_rot;

			if (PUSH(CInput::eMouseL) && !m_hide)
			{
				m_copy_pos = m_pos;
				//ハイド状態
				m_HideAnim = !m_HideAnim;
				m_state = eState_Hide;
			}
			else if (PUSH(CInput::eMouseL) && m_hide)
			{
				m_pos = m_copy_pos;
				m_state = eState_Idle;
			}
		}
	}
	break;
	}
	//DebugProfiler::EndTimer(name);
}