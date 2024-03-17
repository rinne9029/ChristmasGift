#pragma once
#include"GimmickObjectBase.h"

//クローゼット管理クラス

class Closet : public GimmickObjectBase {
private:
	CVector3D m_size;		//箱形の大きさ
public:
	//コンストラクタ
	Closet(const CVector3D& pos, const CVector3D& rot, const CVector3D& size,int no);

	//更新処理
	void Update();
};