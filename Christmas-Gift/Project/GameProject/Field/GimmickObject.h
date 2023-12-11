#pragma once
#include"ObjectBase/ObjectBase.h"
#include"Light/Light.h"

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
};