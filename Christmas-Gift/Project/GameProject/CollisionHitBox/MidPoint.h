#pragma once
#include"CollisionBoxBase.h"

class ToolTips;

//�v���[���g�ݒu�p�t���OCollision

class MidPoint : public CollisionBoxBase {
private:
	CVector3D m_size;
	ToolTips* mp_tooltips;
public:
	//�R���X�g���N�^
	MidPoint(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	//�X�V����
	void Update();
	//�Փˏ���
	void Collision(Task* t);
};