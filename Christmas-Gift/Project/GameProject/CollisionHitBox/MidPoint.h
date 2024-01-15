#pragma once
#include"CollisionBoxBase.h"

class ToolTips;

//プレゼント設置用フラグCollision

class MidPoint : public CollisionBoxBase {
private:
	CVector3D m_size;
	ToolTips* mp_tooltips;
public:
	//コンストラクタ
	MidPoint(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);

	//衝突処理
	void Collision(Task* t);
};