#pragma once
#include"GLLibrary/CModel.h"
#include"../../ObjectBase/ObjectBase.h"

//全オブジェクトのベースクラス

class GimmickObjectBase : public ObjectBase
{
private:
	CVector3D m_size;		//オブジェクトの大きさ
public:
	CModel* m_model;
	int m_objectno;				//オブジェクト別番号
	GimmickObjectBase(const CVector3D& pos,const CVector3D& rot,const CVector3D& size,ETaskTag tag,bool remove);
	~GimmickObjectBase();
	void Render();
	bool CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal);
};