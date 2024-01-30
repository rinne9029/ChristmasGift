#include "Door.h"

Door::Door(const CVector3D& pos,const CVector3D& rot,const CVector3D& scale,const CVector3D& obbscale)
	:Task(ETaskTag::eDoor,true)
	,m_isopen(false)
{
	m_model = COPY_RESOURCE("Door", CModelObj);
	m_pos = pos;
	m_rot = rot;
	m_scale = scale;
	m_obbscale = obbscale;

	m_tooltips = new ToolTips();

	m_obb1 = COBB(
		m_pos,
		m_rot,
		m_obbscale
	);
	
	m_obb2 = COBB(
		m_pos,
		m_rot,
		m_obbscale + CVector3D(0.1, 0.1, 0.1)
	);

}

void Door::Update()
{
}

void Door::Render()
{
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(m_scale);
	m_model.Render();

	//デバッグ用
	//Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
}

void Door::Collision(Task* t)
{
	switch (t->GetTag())
	{
	case ETaskTag::eEnemy:
	{
		float dist;
		CVector3D axis;
		static int OpenCheck = 0;	//開閉を制限する変数
		if (CCollision::CollisionOBBCapsule(m_obb2, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			//ドアが閉じているなら
			if (!m_isopen)
			{
				m_isopen = true;
				m_rot = m_rot + CVector3D(0, DtoR(90), 0);
				OpenCheck = 1;
			}
		}
		else if(OpenCheck == 1)
		{
			if (m_isopen)
			{
				m_isopen = false;
				m_rot = m_rot - CVector3D(0, DtoR(90), 0);
				OpenCheck = 0;
			}
		}
		break;
	}
	case ETaskTag::ePlayer:
	{
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb2, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			m_tooltips->isDraw = true;
			if (PUSH(CInput::eMouseL))
			{
				m_isopen = !m_isopen;
				//ドアを開ける
				if (m_isopen)
				{
					m_rot = m_rot + CVector3D(0, DtoR(90), 0);
					//接触判定用の箱を削除
					m_obb1 = COBB(
						m_pos,
						m_rot,
						CVector3D(0,0,0)
					);
				}
				//ドアを閉じる
				else
				{
					m_rot = m_rot - CVector3D(0, DtoR(90), 0);
					//接触判定用の箱を出す
					m_obb1 = COBB(
						m_pos,
						m_rot,
						m_obbscale
					);
				}
			}
		}
		else
		{
			m_tooltips->isDraw = false;
		}
		break;
	}
	}
}
