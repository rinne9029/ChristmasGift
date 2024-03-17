#pragma once
#include"ObjectBase/ObjectBase.h"

//ステージの床管理クラス

class FieldFloor : public ObjectBase
{
private:
	CModelObj m_model;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FieldFloor();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	CModel* GetModel() {
		return &m_model;
	}
};