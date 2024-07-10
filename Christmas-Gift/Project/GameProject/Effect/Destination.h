#pragma once
#include"EffectBase.h"

//目標地点管理クラス

class Destination : public EffectBase
{
private:
	CModelObj m_model;
public:
	//コンストラクタ
	Destination(CModelObj model,const CVector3D& pos,const CVector3D& scale);
	//更新処理
	void Update();
	//描画処理
	void Render();
};