#include "key.h"

key::key(const CVector3D& pos,const CVector3D& rot, const CVector3D& scale)
	:Task(ETaskTag::eFieldObject,true)
{
	m_pos = pos;
	m_rot = rot;
	m_scale = scale;
	m_model = COPY_RESOURCE("Key", CModelObj);
}

void key::Render()
{
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(m_scale);
	m_model.Render();
}
