#pragma once

//フェードイン・フェードアウト管理クラス

class Filta : public Task
{
private:
	CImage m_filta;		//フィルター
	float m_alpha;		//不透明度
	float m_time;		//時間
	void Fadein();		//フェードイン処理
	void Fadeout();		//フェードアウト処理
public:
	//コンストラクタ
	Filta();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};