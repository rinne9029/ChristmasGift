#include "CollisionBoxBase.h"

CollisionBoxBase::CollisionBoxBase(ETaskTag tag, bool remove)
	:Task(tag,remove)
{
}

CollisionBoxBase::~CollisionBoxBase()
{
}

void CollisionBoxBase::Render()
{
	//デバッグ用
	Utility::DrawOBB(m_obb2, CVector4D(1, 0, 0, 1));
}
