#include"Switch.h"

Switch::Switch(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,int switchno)
	:GimmickObjectBase(pos, rot, size, ETaskTag::eFieldObject, true)
{
	m_model = new CModelObj(*GET_RESOURCE("Switch", CModelObj));
	m_objectno = 1;
	m_SwitchNo = switchno;
}

void Switch::Update()
{
}
