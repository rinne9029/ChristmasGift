#pragma once

class Timer : public Task
{
private:
	CImage m_Image;
	int count;
	int a;
public:
	//コンストラクタ
	Timer();
	//デストラクタ
	~Timer();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};