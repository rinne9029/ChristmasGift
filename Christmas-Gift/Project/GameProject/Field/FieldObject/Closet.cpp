#include"Closet.h"

Closet::Closet(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,int no)
	:GimmickObjectBase(pos,rot,size,ETaskTag::eFieldObject,true)
{
	m_model = new CModelObj(*GET_RESOURCE("Closet", CModelObj));
	m_objectno = 2;
}

void Closet::Update()
{

}