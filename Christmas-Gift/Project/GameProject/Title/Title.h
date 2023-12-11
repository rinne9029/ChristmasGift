#pragma once

class Filta;

class Title : public Task {
private:
	Filta* mp_filta;
	CImage m_StartText;
	CImage m_ManualText;
	CImage m_RankingText;
	int m_select;
	float m_fuwafuwa;
	void FuwaFuwa();
	void Erabu();
public:
	//コンストラクタ
	Title();
	//デストラクタ
	~Title();
	//更新処理
	void Update();
	//3D描画処理
	void Render();
	//2D描画処理
	void Draw();
};