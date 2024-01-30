#pragma once

class Result : public Task {
private:
	CImage m_BuckGrangd;		//背景
	CImage m_TitleText;			//タイトル文字
	CImage m_RankingText;		//ランキング文字
	int m_select;				//セレクトフラグ
	float m_High;				//強調表示用変数
	void HighLight();			//強調表示処理
public:
	//コンストラクタ
	Result();
	//デストラクタ
	~Result();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};
