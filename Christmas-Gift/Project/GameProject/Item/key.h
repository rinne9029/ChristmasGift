#pragma once

//子供部屋の鍵管理クラス

class key : public Task {
private:
	CModelObj m_model;
public:
	//コンストラクタ
	key(const CVector3D& pos,const CVector3D& rot, const CVector3D& scale);

	//描画処理
	void Render();
};