#pragma once
#include"ObjectBase/ObjectBase.h"

//ステージの壁管理クラス

class FieldWall : public ObjectBase
{
private:
	CModelObj m_model;
public:
	///<summary>
	///コンストラクタ
	///</summary>
	FieldWall();
	///<summary>
	///更新処理
	///</summary>
	void Update();
	///<summary>
	///描画処理
	///</summary>
	void Render();

	CModel* GetModel()
	{
		return &m_model;
	}
};