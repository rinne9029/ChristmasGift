#include"CharaBase.h"
#include"Field/Field.h"
#include"Field/FieldObject/GimmickObjectBase.h"
#include"Navigation/NavNode.h"
#include"Navigation/NavManager.h"


//�R���X�g���N�^
CharaBase::CharaBase(ETaskTag tag,bool remove)
	:ObjectBase(tag,remove)
	, m_isGround(false)
{
	m_remove = true;
}

//�f�X�g���N�^
CharaBase::~CharaBase()
{
	NavManager::Instance()->RemoveNode(m_navNode);
}

//�Փ˔���
void CharaBase::Collision(Task* t)
{
	switch (t->GetTag())
	{
	case ETaskTag::eField:
		if (Field* f = dynamic_cast<Field*>(t))
		{
			//�����߂���
			CVector3D v(0, 0, 0);
			//�J�v�Z���ƃ��f���̏Փ�
			auto tri = f->GetColModel()->CollisionCupsel(m_lineS, m_lineE, m_rad);
			//�ڐG�����ʂ̐��J��Ԃ�
			for (auto& t : tri)
			{
				if (t.m_normal.y < -0.5f)
				{
					//�ʂ����������V��ɓ�������	
					//�㏸���x��0�ɖ߂�
					if (m_vec.y > 0)
						m_vec.y = 0;
				}
				else if (t.m_normal.y > 0.5f)
				{
					//�ʂ���������n�ʂɓ�������
					//�d�͗������x��0�ɖ߂�
					if (m_vec.y < 0) {
						m_vec.y = 0;
						m_isGround = true;
					}
					
				}
				float max_y = max(t.m_vertex[0].y, max(t.m_vertex[1].y, t.m_vertex[2].y));
				//�ڐG�����ʂ̕����ցA�߂荞�񂾕������߂�
				CVector3D nv = t.m_normal * (m_rad - t.m_dist);
				//�ł��傫�Ȉړ��ʂ����߂�
				v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
				if (max_y > m_pos.y + 1)
				{
					v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
					v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
				}
				
			}
			//�����߂�
			m_pos += v;
			m_lineS += v;
			m_lineE += v;
		}
		break;
	case ETaskTag::eFieldObject:
	{
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(t->m_CollisionObb, m_lineS, m_lineE, m_rad, &axis, &dist)) {
			if (axis.y > 0.5f)
			{
				//�ʂ������->�n�ʂɓ�������
				//�d�͗������x��0�ɖ߂�
				if (m_vec.y < 0)
					m_vec.y = 0;
			}
			//�����߂�
			float s = m_rad - dist;
			m_pos += axis * s;
		}
	}
	break;
	}
}

NavNode* CharaBase::GetNavNode() const
{
	return m_navNode;
}



void CharaBase::Update()
{

	if (m_navNode != nullptr)
	{
		m_navNode->SetPos(m_pos + CVector3D(0.0f, 1.5f, 0.0f));
	}

	//�ȉ~�`�����蔻��
	m_lineS = m_pos + CVector3D(0, m_height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);

	m_vec.y -= GRAVITY;
	m_pos.y += m_vec.y;
}