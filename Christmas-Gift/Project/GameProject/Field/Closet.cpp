#include"Closet.h"

Closet::Closet(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	:ObjectBase(ETaskTag::eHideBox)
{
	m_model = COPY_RESOURCE("Item", CModelObj);
	m_remove = true;
	m_pos = pos;
	m_rot = rot;
	m_size = size;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_size);

}

void Closet::Update()
{

}

void Closet::Render()
{
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(1.2, 1.2, 2);
	m_model.Render();
	//Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
}

void Closet::Collision(Task* t)
{
}