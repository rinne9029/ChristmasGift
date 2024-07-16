#pragma once
#include"ObjectBase/ObjectBase.h"

class Player;

//�v���C���[�J�����Ǘ��N���X

class Camera : public ObjectBase {
private:
	Player* mp_player;		//�v���C���[

	int m_idx;
	int m_state;			//���݂̏��

	//��ԕω�
	enum {
		eState_Idle,		//�ʏ���
		eState_ClosetIn,	//�B�ꂽ���
	};			
public:
	//�ʏ���
	void StateIdle();
	//�B�ꂽ���
	void StateClosetIn();
	//�R���X�g���N�^
	Camera(const CVector3D& rot);
	//�X�V����
	void Update();
	//�`�揈��
	void Render();
};