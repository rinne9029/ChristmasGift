#include"Camera.h"
#include"../Chara/Player/Player.h"

#define CAMERA_SPEED 0.002f	//カメラの回転スピード

//コンストラクタ
Camera::Camera(const CVector3D& rot)
	:Task(ETaskTag::eCamera, true)
	, mp_player(nullptr)
	, m_idx(0)
	, m_state(eState_Idle)
{
	mp_player = dynamic_cast<Player*>(TaskManager::FindObject(ETaskTag::ePlayer));
	m_pos = CVector3D(0, 1.5, 0);
	m_rot = rot;
}

//通常状態
void Camera::StateIdle()
{
	//マウス操作で回転
	CVector2D mouse_vec = CInput::GetMouseVec();
	m_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * CAMERA_SPEED;

	//カメラの上下角度制限
	m_rot.x = min(DtoR(40), max(DtoR(-40), m_rot.x));
}

//隠れた状態
void Camera::StateClosetIn()
{
	//カメラの向きをクローゼットの正面方向に変更
	m_rot = mp_player->m_Closet_rot - CVector3D(0, DtoR(90), 0);
}

//更新処理
void Camera::Update()
{
	//フェードイン・フェードアウト中は処理をしない
	if (GameData::StartFadeOut || GameData::StartFadeIn)return;

	switch (m_state) {
	case eState_Idle:
		StateIdle();
		break;
	case eState_ClosetIn:
		StateClosetIn();
		break;
	}

	//カメラの座標
	CVector3D camera_pos[2] = {
		CVector3D(0,1.5,0),	//立ちの高さ
		CVector3D(0,0.7,0),	//しゃがみの高さ
	};

	//ctrlボタン
	if (PUSH(CInput::eButton3) && m_state == eState_Idle) {
		m_idx = (m_idx + 1) % 2;	//立ちとしゃがみの切り替え
	}

	//割合補間
	m_pos = m_pos * 0.9f + camera_pos[m_idx] * 0.1f;
	
}

//描画処理
void Camera::Render()
{
	CMatrix cam_matrix = CMatrix::MTranselate(mp_player->m_pos)					//character_matrix
		* CMatrix::MTranselate(m_pos) * CMatrix::MRotation(m_rot)				//branch_matrix
		* CMatrix::MTranselate(CVector3D(0, 0, 0));							//camera_matrix
		//カメラ位置を行列で設定
	CCamera::GetCurrent()->SetTranseRot(cam_matrix);
}