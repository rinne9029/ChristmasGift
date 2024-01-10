#pragma once
#include"CollisionBoxBase.h"

//プレゼント設置用フラグCollision

class MidPoint : public CollisionBoxBase {
private:
	CVector3D m_size;
public:
	//コンストラクタ
	MidPoint(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);

	//衝突処理
	void Collision(Task* t);
};