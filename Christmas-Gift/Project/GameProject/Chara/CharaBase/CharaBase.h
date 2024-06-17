#pragma once
#include"ObjectBase/ObjectBase.h"

//�L�����N�^�[�̃x�[�X�N���X

//�d�͉����x
#define GRAVITY (0.02f)

class NavNode;

class CharaBase : public ObjectBase
{
protected:
	CModelA3M m_model;		//���f���f�[�^
	NavNode* m_navNode;		//�����m�[�h

	CVector3D m_scale;		//�L�����N�^�[�`��T�C�Y

	bool m_isGround;		//���n�t���O

	float m_movespeed;		//�ړ����x

public:
	//�R���X�g���N�^
	CharaBase(ETaskTag tag,bool remove);

	//�f�X�g���N�^
	virtual ~CharaBase();

	//�Փ˔���
	void Collision(Task* t);

	NavNode* GetNavNode() const;

	//�X�V
	void Update();
};
