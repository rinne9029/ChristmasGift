#pragma once
#include"CollisionBoxBase.h"

//�E�o�p�t���OCollision

class EndPoint : public CollisionBoxBase {
private:
	CVector3D m_size;
public:
	//�R���X�g���N�^
	EndPoint(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	//�X�V����
	void Update();
	//�Փˏ���
	void Collision(Task* t);
};