#include"Camera.h"
#include"../Chara/Player/Player.h"

#define CAMERA_SPEED 0.002f	//�J�����̉�]�X�s�[�h

//�R���X�g���N�^
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

//�ʏ���
void Camera::StateIdle()
{
	//�}�E�X����ŉ�]
	CVector2D mouse_vec = CInput::GetMouseVec();
	m_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * CAMERA_SPEED;

	//�J�����̏㉺�p�x����
	m_rot.x = min(DtoR(40), max(DtoR(-40), m_rot.x));
}

//�B�ꂽ���
void Camera::StateClosetIn()
{
	//�J�����̌������N���[�[�b�g�̐��ʕ����ɕύX
	m_rot = mp_player->m_Closet_rot - CVector3D(0, DtoR(90), 0);
}

//�X�V����
void Camera::Update()
{
	//�t�F�[�h�C���E�t�F�[�h�A�E�g���͏��������Ȃ�
	if (GameData::StartFadeOut || GameData::StartFadeIn)return;

	switch (m_state) {
	case eState_Idle:
		StateIdle();
		break;
	case eState_ClosetIn:
		StateClosetIn();
		break;
	}

	//�J�����̍��W
	CVector3D camera_pos[2] = {
		CVector3D(0,1.5,0),	//�����̍���
		CVector3D(0,0.7,0),	//���Ⴊ�݂̍���
	};

	//ctrl�{�^��
	if (PUSH(CInput::eButton3) && m_state == eState_Idle) {
		m_idx = (m_idx + 1) % 2;	//�����Ƃ��Ⴊ�݂̐؂�ւ�
	}

	//�������
	m_pos = m_pos * 0.9f + camera_pos[m_idx] * 0.1f;
	
}

//�`�揈��
void Camera::Render()
{
	CMatrix cam_matrix = CMatrix::MTranselate(mp_player->m_pos)					//character_matrix
		* CMatrix::MTranselate(m_pos) * CMatrix::MRotation(m_rot)				//branch_matrix
		* CMatrix::MTranselate(CVector3D(0, 0, 0));							//camera_matrix
		//�J�����ʒu���s��Őݒ�
	CCamera::GetCurrent()->SetTranseRot(cam_matrix);
}