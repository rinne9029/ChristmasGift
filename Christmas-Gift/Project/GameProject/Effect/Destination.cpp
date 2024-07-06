#include "Destination.h"

Destination::Destination(CModelObj model, const CVector3D& pos, const CVector3D& scale)
	:Task(ETaskTag::eEffect,true)
{
	m_model = model;
	m_pos = pos;
	m_scale = scale;
}

void Destination::Update()
{
}

void Destination::Render()
{
	//CLight::SetLighting(true);
	m_model.SetPos(m_pos);
	m_model.SetScale(m_scale);
	m_model.RenderBuilbord();
}
