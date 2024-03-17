#include "MidPoint.h"
#include"EndPoint.h"
#include"Item/PresentBox.h"
#include"UI/ToolTips.h"
#include"GameScene/GameData.h"

MidPoint::MidPoint(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	:CollisionBoxBase(ETaskTag::eCollisionBox,true)
{
	m_pos = pos;
	m_rot = rot;
	m_size = size;

	m_IsRender = true;
	//ツールチップ
	m_tooltips = new ToolTips();

	m_obb2 = COBB(
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
		if (CCollision::CollisionOBBCapsule(m_obb2, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			if (GameData::isGift) return;
			m_tooltips->isDraw = true;
			//Eボタン入力でプレゼント配置
			if (PUSH(CInput::eMouseL))
			{
				GameData::isGift = true;
				new PresentBox
				(
					CVector3D(m_pos),
					CVector3D(0, 0, 0)
				);
				//判定描画終了
				m_IsRender = false;
				//脱出ポイント
				new EndPoint
				(
					CVector3D(4.555758, 0, 6.188184),
					CVector3D(0, 0, 0),
					CVector3D(0.5, 1, 0.5)
				);
			}
		}
		else
		{
			m_tooltips->isDraw = false;
		}
	}
}

