#pragma once

//タイトル画面管理クラス

class Title : public Task {
private:
	CImage m_TitleText;		//タイトル名テキスト
	CImage m_StartText;		//スタートテキスト
	CImage m_ManualText;	//説明テキスト
	CImage m_RankingText;	//ランキングテキスト
	int m_select;			//セレクトフラグ
	float m_fuwafuwa;		//ふわふわ表示用実数
	void FuwaFuwa();		//ふわふわ表示処理
public:
	//コンストラクタ
	Title();
	//デストラクタ
	~Title();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};