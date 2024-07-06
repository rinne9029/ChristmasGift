#include"Camera.h"
#include"../Chara/Player/Player.h"
#include"Field/Field.h"
#include"Field/FieldObject/Closet.h"
#include"Field/FieldWall.h"

//�R���X�g���N�^
Camera::Camera(const CVector3D& rot)
	:ObjectBase(ETaskTag::eCamera, true)
	, mp_player(nullptr)
	, m_idx(0)
	, m_state(eState_Idle)
{
	m_pos = CVector3D(0, 1.5, 0);
	m_rot = rot;
}

//�ʏ���
void Camera::StateIdle()
{
	//�v���C���[���N���[�[�b�g�ɉB�ꂽ�Ȃ�
	if (mp_player->m_state == 2)	m_state = eState_ClosetIn;

	//�J������]���x
	float cam_speed = 0.002f;

	//�}�E�X����ŉ�]
	CVector2D mouse_vec = CInput::GetMouseVec();

	//��]
	m_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;

	//�㉺�p�x����
	m_rot.x = min(DtoR(40), max(DtoR(-40), m_rot.x));


}

//�B�ꂽ���
void Camera::StateClosetIn()
{
	//�J�����̌������N���[�[�b�g�̐��ʕ����ɕύX
	m_rot = mp_player->m_Closet_rot - CVector3D(0, DtoR(90), 0);

	//�ʏ��Ԃ̃J�����ɖ߂�
	if (mp_player->m_state != 2)		m_state = eState_Idle;
}

//�X�V����
void Camera::Update()
{
	//�t�F�[�h�C���E�t�F�[�h�A�E�g���͏��������Ȃ�
	if (GameData::StartFadeOut)return;

	//�v���C���[
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

//�`�揈��
void Camera::Render()
{
	//�J�����̍��W
	CVector3D camera_pos[2] = {
		CVector3D(0,1.5,0),	//�����̍���
		CVector3D(0,0.7,0),	//���Ⴊ�݂̍���
	};

	//ctrl�{�^��
	if (PUSH(CInput::eButton3) && m_state == eState_Idle) {
		m_idx = (m_idx + 1) % 2;	//���_�؂�ւ�
	}

	//�������
	m_pos = m_pos * 0.90f + camera_pos[m_idx] * 0.10f;

	CMatrix cam_matrix = CMatrix::MTranselate(mp_player->m_pos)					//character_matrix
		* CMatrix::MTranselate(m_pos) * CMatrix::MRotation(m_rot)				//branch_matrix
		* CMatrix::MTranselate(CVector3D(0, 0, -0.8));							//camera_matrix
		//�J�����ʒu���s��Őݒ�
	CCamera::GetCurrent()->SetTranseRot(cam_matrix);
}