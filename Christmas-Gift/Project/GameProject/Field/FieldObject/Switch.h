#pragma once
#include"GimmickObjectBase.h"

class Switch : public GimmickObjectBase {
private:

public:
	int m_SwitchNo;
	//コンストラクタ
	Switch(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,int lightno);

	//更新処理
	void Update();
};