#pragma once

//制限時間管理クラス

class Timer : public Task
{
private:
	CImage m_Image;
	int m_count;
public:
	//コンストラクタ
	Timer();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};