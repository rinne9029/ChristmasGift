#pragma once
#include"ObjectBase/ObjectBase.h"

class PresentBox : public ObjectBase {
private:
	CModelObj m_model;
public:
	//コンストラクタ
	PresentBox(const CVector3D& pos, const CVector3D& rot);

	//描画処理
	void Render();
};