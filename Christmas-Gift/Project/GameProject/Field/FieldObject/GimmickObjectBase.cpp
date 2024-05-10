#include"Field/FieldObject/GimmickObjectBase.h"

GimmickObjectBase::GimmickObjectBase(const CVector3D& pos,const CVector3D& rot,const CVector3D& size, ETaskTag tag,bool remove)
	:ObjectBase(tag,remove)
{
	m_pos = pos;	//���W
	m_rot = rot;	//��]�l
	m_size = size;	//�傫��
}

GimmickObjectBase::~GimmickObjectBase()
{
	if (m_model)	delete m_model;
}

void GimmickObjectBase::Render()
{
	m_model->SetPos(m_pos);
	m_model->SetRot(m_rot);
	m_model->SetScale(m_size);
	m_model->Render();

	//�f�o�b�O�p�FOBB�`��
	Utility::DrawOBB(m_CollisionObb, CVector4D(1, 0, 0, 1));
}

bool GimmickObjectBase::CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal)
{
	if (m_model == nullptr) return false;

	return m_model->CollisionRay(outHitPos, outHitNormal, start, end);
}