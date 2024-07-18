#include "Light.h"

//�R���X�g���N�^
Light::Light(CVector3D& pos,int no,int roomno,float range, int type,bool islight)
	:ObjectBase(ETaskTag::eFieldLight,true)
	,m_islight(islight)
	,m_roomNo(roomno)
	,m_no(no)
{
	m_pos = pos;
	m_range = range;
	m_type = type;
}

//�f�X�g���N�^
Light::~Light()
{
	
}

//�X�V����
void Light::Update()
{
	//�|�C���g���C�g(�S����)
	CLight::SetType(m_no, m_type);
	CLight::SetRange(m_no,m_range);
	CLight::SetPos(m_no, m_pos);
	//���C�g�����Ă��鎞
	if (m_islight)
	{
		CLight::SetColor(m_no, CVector3D(0.0f, 0.0f, 0.0f), CVector3D(1.0f, 1.0f, 0.9f));
	}
	//���C�g�������Ă��鎞
	else
	{
		CLight::SetColor(m_no, CVector3D(0.0f, 0.0f, 0.0f), CVector3D(0.1f, 0.1f, 0.1f));
	}
	
}