#include "Light.h"

//コンストラクタ
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

//デストラクタ
Light::~Light()
{
	
}

//更新処理
void Light::Update()
{
	//ポイントライト(全方位)
	CLight::SetType(m_no, m_type);
	CLight::SetRange(m_no,m_range);
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