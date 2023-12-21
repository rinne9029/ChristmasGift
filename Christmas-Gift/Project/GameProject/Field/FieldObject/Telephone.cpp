#include"Telephone.h"

Telephone::Telephone(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,int no)
	:GimmickObjectBase(pos, rot, size, no, ETaskTag::eFieldObject, true)
{
	m_model = GET_RESOURCE("Telephone", CModel);
}

void Telephone::Update()
{
}
