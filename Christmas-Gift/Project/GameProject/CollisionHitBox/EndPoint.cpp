#include "EndPoint.h"
#include"UI/ToolTips.h"
#include"GameScene/GameData.h"

EndPoint::EndPoint(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	:CollisionBoxBase(ETaskTag::eCollisionBox,true)
{
	m_pos = pos;
	m_rot = rot;
	m_size = size;

	m_IsRender = true;

	m_obb2 = COBB(
		m_pos,
		m_rot,
		m_size
	);
	m_tooltips = new ToolTips();
}

//衝突処理
void EndPoint::Collision(Task* t)
{
	switch (t->GetTag())
	{
	case ETaskTag::ePlayer:
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb2, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			//プレゼントが設置できていなければ処理はスルー
			if (!GameData::isGift) return;

			//ツールチップ表示
			m_tooltips->isDraw = true;
			//左クリックでゲームクリア
			if (PUSH(CInput::eMouseL))
			{
				GameData::GameClearCheck = true;
				GameData::StartFadeOut = true;
			}
		}
		else
		{
			//ツールチップ非表示
			m_tooltips->isDraw = false;
		}
	}
}
