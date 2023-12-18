#include"Field/GimmickObject.h"

GimmickObject::GimmickObject(CVector3D& pos,CVector3D& rot,CVector3D& size,CVector3D& obbsize, int no)
	:ObjectBase(ETaskTag::eFieldObject,true)
	,mp_light(nullptr)
{
	m_pos = pos;	//座標
	m_rot = rot;	//回転値
	m_size = size;	//大きさ
	m_obbsize = obbsize;	
	m_obb = COBB
	(
		m_pos,
		m_rot,
		m_obbsize
	);
	//ナンバーに応じて描画するオブジェクト変更
	switch (no)
	{
	case 0:
		m_obj = GET_RESOURCE("Telephone", CModel);
		break;
	case 1:
		m_obj = GET_RESOURCE("Switch", CModel);
		break;
	case 2:
		m_obj = GET_RESOURCE("Closet", CModel);
		break;
	}

}

GimmickObject::~GimmickObject()
{

}

void GimmickObject::Update()
{
	
}

void GimmickObject::Render()
{
	m_obj->SetPos(m_pos);
	m_obj->SetRot(m_rot);
	m_obj->SetScale(m_size);
	m_obj->Render();
	//Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
}

bool GimmickObject::CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal)
{
	if (m_obj == nullptr) return false;

	return m_obj->CollisionRay(outHitPos, outHitNormal, start, end);
}

//int GimmickObject::CollisionLine(const CVector3D& lineS, const CVector3D& lineE,const CVector3D& dir,CVector3D* cross)
//{
//	float rad = 0.2f;
//	float dist;
//	CVector3D axis;
//	if (CCollision::CollisionOBBCapsule(m_obb, lineS, lineE, rad, &axis, &dist))
//	{
//		if (cross) {
//			*cross = axis*dist;
//		}
//		return 0;
//	}
//		return -1;
//	
//}




