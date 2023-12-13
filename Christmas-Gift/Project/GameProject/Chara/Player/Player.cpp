#include"Player.h"
#include"Field/Field.h"
#include"Field/Closet.h"
#include"Filta/Filta.h"
#include"Field/GimmickObject.h"
#include"Camera/Camera.h"
#include"Light/Light.h"
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
	:CharaBase(ETaskTag::ePlayer,true)
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
	m_pos = pos;				//プレイヤー初期座標
	m_scale = scale;			//プレイヤー大きさ
	m_height = 1.9f;			//高さ
	m_rad = 0.3f;				//半径
	m_lS = CVector3D(0, 0, 0);
	m_lE = CVector3D(0, 0, 0);

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
	
	{
		//ライト設置テスト
		//0番　環境光源 1番自身の光源 2～部屋の光源
		//static int idx = 2;
		//if (PUSH(CInput::eMouseL) || PUSH(CInput::eMouseR)) {
		//	if (PUSH(CInput::eMouseL)) {
		//		//スポットライト
		//		CLight::SetType(idx, CLight::eLight_Spot);
		//		//下向き
		//		CLight::SetDir(idx, CVector3D(0, -1, 0));
		//		//範囲は15°
		//		CLight::SetRadiationAngle(idx, DtoR(80.0f));
		//	}else {
		//		//ポイントライト(全方位)
		//		CLight::SetType(idx, CLight::eLight_Point);
		//	}
		//	//減衰率（低いと光の届く範囲が広い。壁を貫通しないように調整）
		//	CLight::SetRange(idx, 4.0f);
		//	//光源の色（アンビエント、ディフューズ）
		//	CLight::SetColor(idx, CVector3D(0, 0, 0), CVector3D(1.0, 1.0, 0.9));
		//	//光源の位置
		//	CLight::SetPos(idx, m_pos + CVector3D(0, 3.0f, 0));
		//	idx++;
		//}

		//自身の光源
		CLight::SetType(1, CLight::eLight_Point);
		CLight::SetRange(1, 1.0f);
		CLight::SetColor(1, CVector3D(0, 0, 0), CVector3D(0.8, 0.8, 0.7));
		CLight::SetPos(1, m_pos + CVector3D(0, 1.0f, 0));
	}

	//歩き速度
	m_Speed = 0.10f;

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
	//ライト
	if (!mp_light)mp_light = dynamic_cast<Light*>(TaskManager::FindObject(ETaskTag::eFieldLight));
	
	if (PUSH(CInput::eMouseL))
	{
		Shot();
	}
	
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

	//ノード配置確認用
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

	float a = 10000;
	Utility::DrawLine(m_lS, m_lE, CVector4D(1, 0, 0, 1), a);
}

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
}


void Player::Shot()
{
	//判定するレイの距離
	const float range = 1.5f;
	CVector3D dir = CMatrix::MRotation(mp_camera->m_rot).GetFront();
	//始点
	CVector3D lineS = m_pos + mp_camera->m_pos;

	//終点
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
	GimmickObject* hit_object = nullptr;
	//全オブジェクトを探索
	auto list = TaskManager::FindObjects(ETaskTag::eFieldObject);
	for (auto t : list) 
	{
		if (GimmickObject* o = dynamic_cast<GimmickObject*>(t))
		{
			//レイとの衝突地点
			CVector3D c;
			//弾の線分でオブジェクトとの判定を行う
			if (o->CollisionLine(lineS, lineE,mp_camera->m_dir,&c) >= 0)
			{
				//発射位置から最も近いオブジェクトを調べる
				float l = (c - lineS).LengthSq();
					if (dist > l) {
						dist = l;
							hit_object = o;
					}
			}
		}
	}
	//最も近いオブジェクトに当たる
	if (hit_object)
	{
		//処理を書く
		printf("オブジェクトに当たった\n");
		
	}
	else if (hit_field)
	{
		//何もしない
		printf("壁に当たった\n");
	}
	else
	{
		printf("何も当たってない\n");
	}
	m_lS = lineS;
	m_lE = lineE;
}