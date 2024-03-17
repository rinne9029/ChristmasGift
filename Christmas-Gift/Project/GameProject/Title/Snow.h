#pragma once

//タイトル用の雪管理クラス

class Snow : public Task {
private:
	CImage m_snow;			//雪
	CVector2D m_snowPos;	//雪の座標
	float m_snowAng;		//雪の回転値
public:
	//コンストラクタ
	Snow();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};