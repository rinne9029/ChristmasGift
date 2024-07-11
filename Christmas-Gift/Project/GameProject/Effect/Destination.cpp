#include "Destination.h"

Destination::Destination(CModelObj model, const CVector3D& pos, const CVector3D& scale)
	:EffectBase(eDestinationEffect_Effect)
{
	m_model = model;
	m_pos = pos;
	m_scale = scale;
}

void Destination::Update()
{
	EffectBase::Update();

	if (m_alpha < 0)	Kill();
	
}

void Destination::Render()
{
	m_model.SetPos(m_pos);
	m_model.SetScale(m_scale);
	m_model.GetMaterial(0)->m_alpha = m_alpha;
	m_model.Render();
	
	
}
