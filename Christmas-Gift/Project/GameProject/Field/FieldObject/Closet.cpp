#include"Closet.h"

Closet::Closet(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,int no)
	:GimmickObjectBase(pos,rot,size,no,ETaskTag::eHideBox,true)
{
	m_model = GET_RESOURCE("Closet", CModel);
}

void Closet::Update()
{

}