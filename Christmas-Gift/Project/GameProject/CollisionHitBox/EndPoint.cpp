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

	m_FlagObb = COBB(
		m_pos,
		m_rot,
		m_size
	);
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
