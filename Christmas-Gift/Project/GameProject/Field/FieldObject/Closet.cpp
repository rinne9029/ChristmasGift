#include"Closet.h"

Closet::Closet(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,int no)
	:GimmickObjectBase(pos,rot,size,ETaskTag::eFieldObject,true)
{
	m_model = new CModelObj(*GET_RESOURCE("Closet", CModelObj));
	m_objectno = 2;
	CVector3D obbsize = CVector3D(0.7, 1, 1);			//ObbÇÃëÂÇ´Ç≥
	CVector3D obbpos = m_pos + CVector3D(0, 1.5, 0);	//ObbÇÃç¿ïW
	m_CollisionObb = COBB(
		obbpos,
		rot,
		obbsize
	);
}

void Closet::Update()
{
	
}