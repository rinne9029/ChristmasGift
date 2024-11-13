#pragma once
#include"../CharaBase/CharaBase.h"

class Camera;
class Switch;
class GimmickObject;
class ToolTips;
class Closet;

//�v���C���[�@�\�N���X

class Player : public CharaBase {
private:
	Switch* mp_switch;		//�X�C�b�`
	Camera* m_camera;		//�J����
	ToolTips* m_tooltips;	//�c�[���`�b�v
	NavNode* node;			//�m�[�h
	Closet* mp_closet;		//�N���[�[�b�g

	float key_ang;			//�����x�N�g���̊p�x
	
	//�f�o�b�O�p
	CVector3D m_lS;			//���C�̎n�_
	CVector3D m_lE;			//���C�̏I�_

	//�A�j���[�V����ID
	enum class AnimId
	{
		Idle,				//�ҋ@
		Walk,				//����
		Crouch,				//���Ⴊ��
		Stand,				//����
		Crouchidle,			//���Ⴊ�ݑҋ@
		Crouchwalk,			//���Ⴊ�ݕ���
	};

	//��ԕω�
	enum {
		eState_Idle,		//�ʏ���
		eState_Squat,		//���Ⴊ�ݏ��
		eState_ClosetIn,	//�N���[�[�b�g�ɓ��������
	};

	//�X�e�[�^�X�֐�
	//�ʏ���
	void StateIdle();
	//���Ⴊ�ݏ��
	void StateSquat();
	//�B�ꂽ���
	void StateClosetIn();

public:
	int m_state;			//�X�e�[�^�X���
	bool m_hide;			//�X�e���X��ԃt���O

	CVector3D m_Closet_rot; //�J�����p�̃N���[�[�b�g�̐��ʕ����x�N�g��
	CVector3D m_Closet_pos;	//�N���[�[�b�g�����̍��W
	CVector3D m_copy_pos;	//�^���X���猳�̈ʒu�ɏo�邽�߂̃v���C���[�̍��W
	//�R���X�g���N�^
	Player(const CVector3D& pos,const CVector3D& rot, const CVector3D& scale);

	//�f�X�g���N�^
	~Player();

	//�X�V����
	void Update();

	//�`�揈��
	void Render();

	//�Փˏ���
	void Collision(Task* t);

	//���e�B�N��
	void Shot();
};