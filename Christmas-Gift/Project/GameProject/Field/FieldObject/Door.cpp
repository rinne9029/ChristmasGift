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

	m_CollisionObb = COBB(
		m_pos,
		m_rot,
		m_obbscale
	);
	
	m_FlagObb = COBB(
		m_pos,
		m_rot,
		CVector3D(0.7f, 2.1f, 0.4f)
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
	Utility::DrawOBB(m_FlagObb, CVector4D(1.0f, 0.0f, 0.0f, 1.0f));
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
		if (CCollision::CollisionOBBCapsule(m_FlagObb, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			//ドアが閉じているなら
			if (!m_isopen)
			{
				m_isopen = true;
				m_rot = m_rot + CVector3D(0.0f, DtoR(90), 0.0f);
				OpenCheck = 1;
			}
		}
		else if(OpenCheck == 1)
		{
			if (m_isopen)
			{
				m_isopen = false;
				m_rot = m_rot - CVector3D(0.0f, DtoR(90), 0.0f);
				OpenCheck = 0;
			}
		}
		break;
	}
	case ETaskTag::ePlayer:
	{
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_FlagObb, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			if (m_tooltips == nullptr)m_tooltips = new ToolTips();
			//ツールチップ生成
			if (m_isopen)
			{
				m_tooltips->m_Text = "開ける";
			}
			else
			{
				m_tooltips->m_Text = "閉じる";
			}
			
			if (PUSH(CInput::eMouseL))
			{
				m_isopen = !m_isopen;
				//ドアを開ける
				if (m_isopen)
				{
					SOUND("SE_DoorOpen")->Volume(GameData::volume);
					SOUND("SE_DoorOpen")->Play();
					m_rot = m_rot + CVector3D(0.0f, DtoR(90), 0.0f);
					//接触判定用の箱を削除
					m_CollisionObb = COBB(
						m_pos,
						m_rot,
						CVector3D(0.0f,0.0f,0.0f)
					);
				}
				//ドアを閉じる
				else
				{
					SOUND("SE_DoorClose")->Volume(GameData::volume);
					SOUND("SE_DoorClose")->Play();
					m_rot = m_rot - CVector3D(0.0f, DtoR(90), 0.0f);
					//接触判定用の箱を出す
					m_CollisionObb = COBB(
						m_pos,
						m_rot,
						m_obbscale
					);
				}
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
		break;
	}
	}
}
