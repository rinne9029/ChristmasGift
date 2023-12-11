#pragma once

class Player;
class Filta;

//睡眠ゲージのクラス
class SleepLife : public Task
{
private:
	Player* mp_player;
	Filta* mp_filta;
public:
	//コンストラクタ
	SleepLife();
	//デストラクタ
	~SleepLife();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();

};
