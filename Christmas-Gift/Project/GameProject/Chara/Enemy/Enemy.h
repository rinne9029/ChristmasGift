#pragma once
#include"../CharaBase/CharaBase.h"

class Player;
class NavNode;
class SearchNode;

//�G����N���X

//�Ƒ�
class Enemy : public CharaBase {
private:
	Player* mp_player;			//�v���C���[
	CVector3D m_moveDir;		//�ړ�����
	CVector3D m_rot_target;		//�v���C���[�����̉�]�l
	CVector3D m_movePos;		//�ړ���̍��W
	NavNode* m_moveNode;		//�ړ��ړI�n�̃m�[�h
	float m_elapsedTime;		//�o�ߎ��Ԍv���p
	bool m_isvigilance;			//�x���t���O
	int m_modelno;				//���f���O��
	float m_warning;			//�x���l
	
	//��
	float eye_length;			//���싗��
	float eye_ang;				//����p�x

	int walk_se_idx;			//�������ʉ��̃t���O 0:�E�� 1:����
	int walk_se_frame[2];		//�������ʉ������t���[�� 0:�E�� 1:����

	NavNode* m_nextNode;		//���Ɉړ�����m�[�h�i�[�p
	NavNode* m_lostNode;		//�v���C���[�����������ꏊ�ɔz�u
	SearchNode* m_searchNode;	//�T�����̃m�[�h

	CVector4D color;			//����͈͂̐F
	COBB m_WarningGauge;		//�x���l�̃Q�[�W

	//�A�j���[�V�����ԍ�
	enum class AnimId
	{
		Idle,		//�ҋ@
		Walk,		//����
		Run,		//����
		Sit,		//����
		SitIdle,	//����ҋ@
		Up,			//����
	};
	
	//��ԕω�
	enum {
		eState_Idle,			//�ҋ@���
		eState_Move,			//�ړ����
		eState_Look,			//�x�����
		eState_Chase,			//�ǐՏ��
		eState_Lost,			//�v���C���[�������������
	};

	int m_state;	//���݂̏��

	//�ҋ@���
	void StateIdle();	
	//�x�����
	void StateLook();
	//�ړ����
	void StateMove();		
	//�ǐՏ��
	void StateChase();	
	//�v���C���[�������������
	void StateLost();

	//��Ԃ��Ƃ̎���͈͂̐F
	void View_Color();

	//�w����W�ւ̈ړ�����
	bool MoveTo(const CVector3D& target);

	//�T�m�t���O
	bool IsEyeFoundPlayer();

	//���݈ʒu����v���C���[�������邩�ǂ����̃t���O
	bool IsLookPlayer() const;
public:
	//�R���X�g���N�^
	Enemy(const CVector3D& pos,const CVector3D& dir, const CVector3D& scale, int model);
	//�f�X�g���N�^
	~Enemy();

	//�T���m�[�h��ݒ�
	void SetSearchNode(SearchNode* node);
	//�T�����̃m�[�h���擾
	SearchNode* GetSearchNode() const;
	
	//�X�V����
	void Update();
	//�`�揈��
	void Render();
	//�ڐG����
	void Collision(Task* t);

};