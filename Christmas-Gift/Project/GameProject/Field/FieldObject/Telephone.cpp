#include"Telephone.h"

Telephone::Telephone(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	:GimmickObjectBase(pos, rot, size, ETaskTag::eFieldObject, true)
{
	m_model = new CModelObj(*GET_RESOURCE("Telephone", CModelObj));
	m_objectno = 0;
}

void Telephone::Update()
{
}
