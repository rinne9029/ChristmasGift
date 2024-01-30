#pragma once
#include"GimmickObjectBase.h"
#include"UI/ToolTips.h"

class Door : public Task {
private:
	CModelObj m_model;
	bool m_isopen;
	CVector3D m_scale;
	CVector3D m_obbscale;
	ToolTips* m_tooltips;
public:
	//コンストラクタ
	Door(const CVector3D& pos,const CVector3D& rot,const CVector3D& scale,const CVector3D& obbscale);

	//更新処理
	void Update();

	//描画処理
	void Render();

	//衝突処理
	void Collision(Task* t);
};