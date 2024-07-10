#include "MidPoint.h"
#include"EndPoint.h"
#include"Item/PresentBox.h"
#include"UI/ToolTips.h"
#include"Effect/Destination.h"

MidPoint::MidPoint(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
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

void MidPoint::Update()
{
	if (m_time < 60)
	{
		m_time++;
	}
	else
	{
		new Destination
		(
			COPY_RESOURCE("Effect_Gool_Side", CModelObj),
			CVector3D(m_pos),
			CVector3D(1, 1, 1)
		);
		m_time = 0;
	}
}



void MidPoint::Collision(Task* t)
{
	switch (t->GetTag())
	{
	case ETaskTag::ePlayer:
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_FlagObb, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			if (GameData::isGift) return;

			//�c�[���`�b�v����
			if(m_tooltips == nullptr)m_tooltips = new ToolTips();
			m_tooltips->m_Text = "�v���[���g��u��";
			//�}�E�X���N���b�N
			if (PUSH(CInput::eMouseL))
			{
				GameData::isGift = true;
				//�v���[���g����
				new PresentBox
				(
					CVector3D(m_pos+CVector3D(0,0.8,0)),
					CVector3D(0, 0, 0)
				);
				
				//�E�o�|�C���g����
				new EndPoint
				(
					CVector3D(4.555758, 0, 6.188184),
					CVector3D(0, 0, 0),
					CVector3D(0.5, 1, 0.5)
				);
				if (m_tooltips != nullptr)
				{
					//�c�[���`�b�v�폜
					m_tooltips->m_Iskill = true;
					m_tooltips = nullptr;
				}
				
				Kill();
			}
		}
		//�����蔻�肩��o����
		else
		{
			if (m_tooltips != nullptr)
			{
				//�c�[���`�b�v�폜
				m_tooltips->m_Iskill = true;
				m_tooltips = nullptr;
			}
		}
	}
}

