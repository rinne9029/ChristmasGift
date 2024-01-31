#pragma once

//ランキング管理クラス

class Ranking : public Task {
private:
	static const int m_max_size = 256;
	static const int m_max_line = 32;
	char m_text[m_max_line][m_max_size];
	int m_line_size;
	int m_select;
	CImage m_TitleText;		//タイトルテキスト
	float m_fuwafuwa;
	void FuwaFuwa();
public:
	//コンストラクタ
	Ranking(const char* file);
	//デストラクタ
	~Ranking();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};