#pragma once
#include<string>

//�^�X�N�̎��
enum class ETaskTag
{
	eScene,						//�Q�[���V�[��
	eField,						//�X�e�[�W
	eFieldFloor,				//��
	eFieldWall,					//��
	eDoor,						//�h�A
	eFieldLight,				//�X�e�[�W�ɐݒu�������C�g
	eFieldObject,				//�X�e�[�W�ɂ���M�~�b�N�I�u�W�F�N�g
	eObject,					//�����̃I�u�W�F�N�g
	eCamera,					//�J����
	ePlayer,					//�v���C���[
	eEnemy,						//�G
	eNavNode,					//�m�[�h
	eHideBox,					//�B����锠
	eCollisionBox,				//�A�N�V�����p�����蔻��
	eWarpRoomBox,				//�}�b�v�ړ��p�����ȓ����蔻��
	eEnemyManager,				//�G�l�~�[�}�l�[�W���[
	eNavManager,				//�m�[�h�}�l�[�W���[
	eUI,						//���[�U�[�C���^�[�t�F�[�X
	eResult,					//���U���g
	eEffect,					//�G�t�F�N�g
	eFilta,						//�t�B���^�[
	eDebug,						//�f�o�b�O�p
};

//�^�X�N�N���X
class Task
{
public:
	ETaskTag m_tag;			//�I�u�W�F�N�g���Ƃ̎�ޖ�
	bool m_remove;			//�I�u�W�F�N�g���Ƃ̍폜�t���O

	CVector3D m_pos;		//�I�u�W�F�N�g���W
	CVector3D m_rot;		//�I�u�W�F�N�g��]�l
	CVector3D m_at;			//�I�u�W�F�N�g�����_
	CVector3D m_vec;		//�I�u�W�F�N�g�ړ��x�N�g��
	CVector3D m_dir;		//�I�u�W�F�N�g�O�����x�N�g��
	CVector3D m_scale;		//�I�u�W�F�N�g�傫��

	//�����蔻��p
	//���A�J�v�Z���p
	float m_rad;			//�I�u�W�F�N�g���a
	float m_height;			//�I�u�W�F�N�g����
	CVector3D m_lineS;		//�I�u�W�F�N�g�����n�_
	CVector3D m_lineE;		//�I�u�W�F�N�g�����I�_

	//���`�p
	COBB m_CollisionObb;	//�ڐG����p��
	COBB m_FlagObb;			//�t���O����p��

	//int m_kill;			//�폜�t���O

private:
	//ETaskTag m_tag;			//�^�X�N�̎��
	bool m_isKill;			//�폜�t���O

public:
	//�R���X�g���N�^
	Task(ETaskTag tag ,bool Remove);

	//�f�X�g���N�^
	virtual~Task();

	//�폜�t���OON
	//void SetKill();

	//�^�X�N�̎�ނ��擾
	ETaskTag GetTag() const;

	//�I�u�W�F�N�g�̎����f���̎擾
	virtual CModel* GetModel() {
		return nullptr;
	}

	//�I�u�W�F�N�g���m�̓����蔻�茟��
	virtual void Collision(Task* t);

	//�폜�t���O���擾
	bool IsKill() const;

	//�^�X�N���폜
	void Kill();

	//�X�V����
	virtual void Update();

	//3D�`�揈��
	virtual void Render();

	//2D�`�揈��
	virtual void Draw();

	friend class TaskManager;
};