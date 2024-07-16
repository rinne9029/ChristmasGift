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
		//�G�t�F�N�g����
		new Destination
		(
			COPY_RESOURCE("Effect_Gool_Side", CModelObj),
			CVector3D(m_pos),
			CVector3D(1.0f, 1.0f, 1.0f)
		);
		m_time = 0.0f;
	}
}

//�Փˏ���
void EndPoint::Collision(Task* t)
{
	switch (t->GetTag())
	{
	case ETaskTag::ePlayer:
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_FlagObb, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			//�v���[���g���ݒu�ł��Ă��Ȃ���Ώ����̓X���[
			if (!GameData::isGift) return;

			//�c�[���`�b�v����
			if(m_tooltips == nullptr)m_tooltips = new ToolTips();
			m_tooltips->m_Text = "�E�o";
			
			//���N���b�N�ŃQ�[���N���A
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
				//�c�[���`�b�v�폜
				m_tooltips->m_Iskill = true;
				m_tooltips = nullptr;
			}
		}
	}
}
