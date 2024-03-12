#include"Camera.h"
#include"../Chara/Player/Player.h"
#include"Field/Field.h"
#include"Field/FieldObject/Closet.h"

//コンストラクタ
Camera::Camera(const CVector3D& rot)
	:ObjectBase(ETaskTag::eCamera,true)
	, mp_player(nullptr)
	, m_idx(0)
	, m_state(eState_Idle)
{
	m_pos = CVector3D(0,1.5,0);
	m_rot = rot;
}

//通常状態
void Camera::StateIdle()
{
	//プレイヤーがクローゼットに隠れたなら
	if (mp_player->m_state == 2)	m_state = eState_ClosetIn;
		
	//カメラ回転速度
	float cam_speed = 0.002f;

	//マウス操作で回転
	CVector2D mouse_vec = CInput::GetMouseVec();

	//回転
	m_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;

	//上下角度制限
	m_rot.x = min(DtoR(40), max(DtoR(-40), m_rot.x));

	
}

//隠れた状態
void Camera::StateClosetIn()
{
	//カメラの向きをクローゼットの正面方向に変更
	m_rot = mp_player->m_Closet_rot - CVector3D(0,DtoR(90),0);

	//通常状態のカメラに戻す
	if(mp_player->m_state != 2)		m_state = eState_Idle;
}

//更新処理
void Camera::Update()
{
	//プレイヤー
	if (!mp_player)		mp_player = dynamic_cast<Player*>(TaskManager::FindObject(ETaskTag::ePlayer));
	
	switch (m_state) {
	case eState_Idle:
		StateIdle();
		break;
	case eState_ClosetIn:
		StateClosetIn();
		break;
	}

}

//描画処理
void Camera::Render()
{
	//カメラの座標
	CVector3D camera_pos[2] = {
		CVector3D(0,1.5,0),	//立ちの高さ
		CVector3D(0,0.7,0),	//しゃがみの高さ
	};

	//ctrlボタン
	if (PUSH(CInput::eButton7) && m_state == eState_Idle) {
		m_idx = (m_idx + 1) % 2;	//視点切り替え
	}

	//割合補間
	m_pos = m_pos * 0.90f + camera_pos[m_idx] * 0.10f;
	
	CMatrix cam_matrix = CMatrix::MTranselate(mp_player->m_pos)					//character_matrix
		* CMatrix::MTranselate(m_pos) * CMatrix::MRotation(m_rot)				//branch_matrix
		* CMatrix::MTranselate(CVector3D(0, 0, -0.8));							//camera_matrix
		//カメラ位置を行列で設定
	CCamera::GetCurrent()->SetTranseRot(cam_matrix);
}