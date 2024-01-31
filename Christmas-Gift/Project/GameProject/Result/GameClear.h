#pragma once

class GameClear : public Task {
private:
	CImage m_BuckGraund;		//背景
	CImage m_TitleText;			//タイトル文字
	CImage m_RankingText;		//ランキング文字
	static const int m_max_size = 256;
	static const int m_max_line = 32;
	//char m_text[m_max_line][m_max_size];
	//char m_text[256];

	int m_Rank[5];
	int m_line_size;
	int m_select;				//セレクトフラグ
	float m_High;				//強調表示用実数
	void HighLight();			//強調表示処理
	void ChengeRanking();		//ランキング更新処理
public:
	//コンストラクタ
	GameClear();
	//デストラクタ
	~GameClear();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};
