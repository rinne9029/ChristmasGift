#include "EndPoint.h"
#include"GameScene/GameData.h"

EndPoint::EndPoint(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	:CollisionBoxBase(ETaskTag::eCollisionBox,true)
{
	m_pos = pos;
	m_rot = rot;
	m_size = size;

	m_obb = COBB(
		m_pos,
		m_rot,
		m_size
	);
}

void EndPoint::Collision(Task* t)
{
	switch (t->GetTag())
	{
	case ETaskTag::ePlayer:
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			if (PUSH(CInput::eMouseL) && GameData::isGift == true)
			{
				TaskManager::KillALL();
			}
		}
	}
}
