#pragma once
#include"CollisionBoxBase.h"

//脱出用フラグCollision

class EndPoint : public CollisionBoxBase {
private:
	CVector3D m_size;
public:
	//コンストラクタ
	EndPoint(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);
	//更新処理
	void Update();
	//衝突処理
	void Collision(Task* t);
};