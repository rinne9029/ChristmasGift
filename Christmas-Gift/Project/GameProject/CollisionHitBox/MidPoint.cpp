#include "MidPoint.h"
#include"Item/PresentBox.h"
#include"UI/ToolTips.h"
#include"GameScene/GameData.h"

MidPoint::MidPoint(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	:CollisionBoxBase(ETaskTag::eCollisionBox,true)
	,mp_tooltips(nullptr)
{
	m_pos = pos;
	m_rot = rot;
	m_size = size;

	//ツールチップ
	if (!mp_tooltips)
		mp_tooltips = dynamic_cast<ToolTips*>(TaskManager::FindObject(ETaskTag::eUI));

	m_obb = COBB(
		m_pos,
		m_rot,
		m_size
	);

}



void MidPoint::Collision(Task* t)
{
	switch (t->GetTag())
	{
	case ETaskTag::ePlayer:
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			//Eボタン入力でプレゼント配置
			if (PUSH(CInput::eMouseL) && !GameData::isGift)
			{
				GameData::isGift = true;
				new PresentBox
				(
					CVector3D(m_pos),
					CVector3D(0, 0, 0)
				);
			}
		}
	}
}

