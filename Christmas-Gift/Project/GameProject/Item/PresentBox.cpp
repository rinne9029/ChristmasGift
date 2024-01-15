#include "PresentBox.h"

PresentBox::PresentBox(const CVector3D& pos, const CVector3D& rot)
	:ObjectBase(ETaskTag::eObject,true)
{
	m_pos = pos;
	m_rot = rot;
	m_model = COPY_RESOURCE("PresentBox", CModelObj);
}

void PresentBox::Render()
{
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.Render();
}
