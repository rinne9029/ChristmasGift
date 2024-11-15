#include "EndPoint.h"
#include"UI/ToolTips.h"
#include"Effect/Destination.h"

EndPoint::EndPoint(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	:CollisionBoxBase(ETaskTag::eCollisionBox,true)
{
	m_pos = pos;
	m_rot = rot;
	m_size = size;

	m_FlagObb = COBB(
		m_pos,
		m_rot,
		m_size
	);
}

void EndPoint::Update()
{
	if (m_time < 1.0f)
	{
		m_time += CFPS::GetDeltaTime();
	}
	else
	{
		//エフェクト生成
		new Destination
		(
			COPY_RESOURCE("Effect_Gool_Side", CModelObj),
			CVector3D(m_pos),
			CVector3D(1.0f, 1.0f, 1.0f)
		);
		m_time = 0.0f;
	}
}

//衝突処理
void EndPoint::Collision(Task* t)
{
	switch (t->GetTag())
	{
	case ETaskTag::ePlayer:
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_FlagObb, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			//プレゼントが設置できていなければ処理はスルー
			if (!GameData::isGift) return;

			//ツールチップ生成
			if(m_tooltips == nullptr)m_tooltips = new ToolTips();
			m_tooltips->m_Text = "脱出";
			
			//左クリックでゲームクリア
			if (PUSH(CInput::eMouseL))
			{
				GameData::GameClearCheck = true;
				GameData::StartFadeOut = true;
			}
		}
		else
		{
			if (m_tooltips != nullptr)
			{
				//ツールチップ削除
				m_tooltips->m_Iskill = true;
				m_tooltips = nullptr;
			}
		}
	}
}
