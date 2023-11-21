#include"Camera.h"
#include"../Chara/Player/Player.h"
#include"Field/Field.h"
#include"Field/Closet.h"

//コンストラクタ
Camera::Camera(const CVector3D& pos, const CVector3D& rot)
	:ObjectBase(ETaskTag::eCamera)
	, mp_player(nullptr)
	, mp_closet(nullptr)
	, m_state(eState_Idle)
	, m_camera_mode(0)
{
	m_remove = true;
	m_pos = pos;
	m_rot = rot;
}

//通常状態
void Camera::StateIdle()
{
	if (mp_player->m_hide)
		m_state = eState_Hide;

	//見下ろし視点中はカメラ固定
	if (m_camera_mode == LookDownCamera) return;

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
void Camera::StateHide()
{
	//カメラの向きをクローゼットの正面方向に変更
	m_rot = mp_player->m_Closet_rot;

	//通常状態のカメラに戻す
	if(!mp_player->m_hide)
		m_state = eState_Idle;
	
}

//更新処理
void Camera::Update()
{
	if (!mp_player)
	{
		mp_player = dynamic_cast<Player*>(TaskManager::FindObject(ETaskTag::ePlayer));
	}
	if (!mp_closet)
	{
		mp_closet = dynamic_cast<Closet*>(TaskManager::FindObject(ETaskTag::eHideBox));
	}

	switch (m_state) {
	case eState_Idle:
		StateIdle();
		break;
	case eState_Hide:
		StateHide();
		break;
	}

}

//描画処理
void Camera::Render()
{
	//カメラの座標
	CVector3D camera_pos[2] = {
		CVector3D(0,1.5,0),
		CVector3D(0,0.7,0),
	};

	static int idx;
	//ctrlボタンをおしたらしゃがみ視点
	if (PUSH(CInput::eButton7)) {
		idx = (idx + 1) % 2;
	}
	//割合補間
	CVector3D cam_pos_target = camera_pos[idx] + CVector3D(0, 0, 0);
	m_pos = m_pos * 0.90f + cam_pos_target * 0.10f;

	//カメラのモードに応じて視点変更

	switch (m_camera_mode)
	{
		//一人称
	case FirstPersonCamera:
	{
		CMatrix cam_matrix = CMatrix::MTranselate(mp_player->m_pos)					//character_matrix
			* CMatrix::MTranselate(m_pos) * CMatrix::MRotation(m_rot)				//branch_matrix
			* CMatrix::MTranselate(CVector3D(0, 0, -0.8));								//camera_matrix
			//カメラ位置を行列で設定
		CCamera::GetCurrent()->SetTranseRot(cam_matrix);
		
		if (PUSH(CInput::eButton8))			//Fキー入力
		{
			m_camera_mode = LookDownCamera;	//見下ろしに変更
		}
	}
	break;
	//見下ろし
	case LookDownCamera:
	{
		CMatrix cam_matrix = CMatrix::MTranselate(mp_player->m_pos)					//character_matrix
			* CMatrix::MTranselate(0, 20, 0) * CMatrix::MRotation(DtoR(90), 0, 0)	//branch_matrix
			* CMatrix::MTranselate(CVector3D(0, 0, 0));								//camera_matrix
			//カメラ位置を行列で設定
		CCamera::GetCurrent()->SetTranseRot(cam_matrix);

		if (PUSH(CInput::eButton8))				//Fキー入力
		{
			m_camera_mode = ThirdPersonCamera;	//三人称に変更
		}
	}
	break;
	//三人称視点
	//デバッグ用
	case ThirdPersonCamera:
	{
		CMatrix cam_matrix = CMatrix::MTranselate(mp_player->m_pos)
			* CMatrix::MTranselate(CVector3D(0, 1.5, 0)) * CMatrix::MRotation(m_rot)
			* CMatrix::MTranselate(CVector3D(-0.5, 0.2, -2));
		//カメラ位置を行列で設定
		CCamera::GetCurrent()->SetTranseRot(cam_matrix);

		if (PUSH(CInput::eButton8))				//Fキー入力
		{
			m_camera_mode = FirstPersonCamera;	//一人称に変更
		}
	}
	break;
	}


	
		

}