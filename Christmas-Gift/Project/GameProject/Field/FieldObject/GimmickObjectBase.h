#pragma once
#include"GLLibrary/CModel.h"
#include"../../ObjectBase/ObjectBase.h"

//�S�I�u�W�F�N�g�̃x�[�X�N���X

class GimmickObjectBase : public ObjectBase
{
private:
	CVector3D m_size;		//�I�u�W�F�N�g�̑傫��
public:
	CModel* m_model;
	int m_objectno;				//�I�u�W�F�N�g�ʔԍ�
	GimmickObjectBase(const CVector3D& pos,const CVector3D& rot,const CVector3D& size,ETaskTag tag,bool remove);
	~GimmickObjectBase();
	void Render();
	bool CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal);
};