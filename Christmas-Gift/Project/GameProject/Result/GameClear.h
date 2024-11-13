#pragma once

//ゲームクリア画面管理クラス

class GameClear : public Task {
private:
	CImage m_BuckGraund;		//背景
	CImage m_TitleText;			//タイトル文字
	int m_select;				//セレクトフラグ
	float m_High;				//強調表示用実数
	void HighLight();			//強調表示処理
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
