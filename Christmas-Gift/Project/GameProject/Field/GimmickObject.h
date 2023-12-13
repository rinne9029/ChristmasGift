#pragma once
#include"ObjectBase/ObjectBase.h"

class Light;

class GimmickObject : public ObjectBase
{
private:
	Light* mp_light;
	CVector3D m_size;		//オブジェクトの大きさ
	CVector3D m_obbsize;	//obbの大きさ
public:
	GimmickObject(CVector3D& pos,CVector3D& rot,CVector3D& size,CVector3D& obbsize,int no);
	~GimmickObject();
	void Update();
	void Render();
	int CollisionLine(const CVector3D& lineS, const CVector3D& lineE,const CVector3D& dir,CVector3D* cross);
};