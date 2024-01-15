#pragma once
#include"GimmickObjectBase.h"

class Telephone : public GimmickObjectBase {
private:

public:
	//コンストラクタ
	Telephone(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);

	//更新処理
	void Update();
};