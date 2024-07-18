#pragma once

//危険信号管理クラス

class DangerFilta : public Task
{
private:
	CImage m_filta;			//フィルター
	float m_alpha;			//透明度
	float m_time;			//時間
	bool m_isalphachenge;	//透明度の加算と減算を変更
public:
	//コンストラクタ
	DangerFilta();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();

};
