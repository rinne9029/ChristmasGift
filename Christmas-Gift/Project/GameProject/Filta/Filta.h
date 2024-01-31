#pragma once
#include"ObjectBase/ObjectBase.h"

class Filta : public Task
{
private:
	CImage m_filta;		//フィルター
	float m_alpha;		//不透明度
	float m_time;
	
	void Fadein();
	void Fadeout();
public:
	//コンストラクタ
	Filta();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};