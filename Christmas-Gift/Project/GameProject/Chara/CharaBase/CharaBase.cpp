#include"CharaBase.h"
#include"Field/Field.h"
#include"Field/FieldObject/GimmickObjectBase.h"
#include"Navigation/NavNode.h"
#include"Navigation/NavManager.h"


//コンストラクタ
CharaBase::CharaBase(ETaskTag tag,bool remove)
	:ObjectBase(tag,remove)
	, m_isGround(false)
{
	m_remove = true;
}

//デストラクタ
CharaBase::~CharaBase()
{
	NavManager::Instance()->RemoveNode(m_navNode);
}

//衝突判定
void CharaBase::Collision(Task* t)
{
	switch (t->GetTag())
	{
	case ETaskTag::eField:
		if (Field* f = dynamic_cast<Field*>(t))
		{
			//押し戻し量
			CVector3D v(0, 0, 0);
			//カプセルとモデルの衝突
			auto tri = f->GetColModel()->CollisionCupsel(m_lineS, m_lineE, m_rad);
			//接触した面の数繰り返す
			for (auto& t : tri)
			{
				if (t.m_normal.y < -0.5f)
				{
					//面が下向き→天井に当たった	
					//上昇速度を0に戻す
					if (m_vec.y > 0)
						m_vec.y = 0;
				}
				else if (t.m_normal.y > 0.5f)
				{
					//面が上向き→地面に当たった
					//重力落下速度を0に戻す
					if (m_vec.y < 0) {
						m_vec.y = 0;
						m_isGround = true;
					}
					
				}
				float max_y = max(t.m_vertex[0].y, max(t.m_vertex[1].y, t.m_vertex[2].y));
				//接触した面の方向へ、めり込んだ分押し戻す
				CVector3D nv = t.m_normal * (m_rad - t.m_dist);
				//最も大きな移動量を求める
				v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
				if (max_y > m_pos.y + 1)
				{
					v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
					v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
				}
				
			}
			//押し戻す
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
				//面が上向き->地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
			}
			//押し戻し
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

	//楕円形当たり判定
	m_lineS = m_pos + CVector3D(0, m_height - m_rad, 0);
	m_lineE = m_pos + CVector3D(0, m_rad, 0);

	m_vec.y -= GRAVITY;
	m_pos.y += m_vec.y;
}