#pragma once

class Player;

//�v���C���[�J�����Ǘ��N���X

class Camera : public Task {
private:
	Player* mp_player;		//�v���C���[

	int m_idx;	
public:
	//��ԕω�
	enum {
		eState_Idle,		//�ʏ���
		eState_ClosetIn,	//�B�ꂽ���
	};
	int m_state;			//���݂̏��
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