#include "Light.h"

//コンストラクタ
Light::Light(CVector3D& pos,int no,int roomno)
	:ObjectBase(ETaskTag::eFieldLight,true)
	,m_islight(true)
	,m_roomNo(roomno)
	,m_no(no)
{
	m_pos = pos;
}

//デストラクタ
Light::~Light()
{
	
}

//更新処理
void Light::Update()
{
	//ポイントライト(全方位)
	CLight::SetType(m_no, CLight::eLight_Point);
	CLight::SetRange(m_no, 5.0f);
	CLight::SetPos(m_no, m_pos);
	//ライトがついている時
	if (m_islight)
	{
		CLight::SetColor(m_no, CVector3D(0, 0, 0), CVector3D(1.0, 1.0, 0.9));
	}
	//ライトが消えている時
	else
	{
		CLight::SetColor(m_no, CVector3D(0, 0, 0), CVector3D(0.1, 0.1, 0.1));
	}
	
}