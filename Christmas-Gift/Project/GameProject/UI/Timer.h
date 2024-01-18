#pragma once

class Timer : public Task
{
private:
	CVector2D a[2];
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