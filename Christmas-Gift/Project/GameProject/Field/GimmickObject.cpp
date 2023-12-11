#include"Field/GimmickObject.h"

GimmickObject::GimmickObject(CVector3D& pos,CVector3D& rot,CVector3D& size,CVector3D& obbsize, int no)
	:ObjectBase(ETaskTag::eField,true)
{
	mp_light = dynamic_cast<Light*>(TaskManager::FindObject(ETaskTag::eFieldLight));
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
		m_model = COPY_RESOURCE("Telephone", CModelObj);
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
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(m_size);
	m_model.Render();
	Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
}