#include"Effect/EffectBase.h"

EffectBase::EffectBase(int Effect_Mode)
	:Task(ETaskTag::eEffect,true)
	,m_alpha(1.0f)
{
	m_Effect_Mode = Effect_Mode;
}

//�ړI�n�G�t�F�N�g
void EffectBase::DestinationEffect()
{
	m_pos.y += 0.01;
	//3�b�œ����ɂȂ�
	m_alpha -= 1.0f/180;
}

void EffectBase::Update()
{
	switch (m_Effect_Mode)
	{
	case 1:
		DestinationEffect();
		break;
	}
}
