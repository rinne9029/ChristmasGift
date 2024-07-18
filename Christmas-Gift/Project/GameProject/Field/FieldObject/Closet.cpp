#include"Closet.h"

Closet::Closet(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,int no)
	:GimmickObjectBase(pos,rot,size,ETaskTag::eFieldObject,true)
{
	m_model = new CModelObj(*GET_RESOURCE("Closet", CModelObj));
	m_objectno = 2;
	CVector3D obbsize = CVector3D(0.7f, 1.0f, 1.0f);			//ObbÇÃëÂÇ´Ç≥
	CVector3D obbpos = m_pos + CVector3D(0.0f, 1.5f, 0.0f);	//ObbÇÃç¿ïW
	m_CollisionObb = COBB(
		obbpos,
		rot,
		obbsize
	);
}

void Closet::Update()
{
	
}