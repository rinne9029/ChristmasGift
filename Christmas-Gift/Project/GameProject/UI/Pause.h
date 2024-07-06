#pragma once

class Pause : public Task {
private:
	int select;
	int step;
	void PauseMenu1();	//ポーズ画面選択
	void PauseMenu2();	//操作方法
public:
	//コンストラクタ
	Pause();
	//2D描画処理
	void Draw();
};
