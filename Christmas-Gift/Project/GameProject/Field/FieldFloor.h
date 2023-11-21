#pragma once
#include"ObjectBase/ObjectBase.h"

class FieldFloor : public ObjectBase
{
private:
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