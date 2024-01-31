#pragma once

//睡眠ゲージ管理クラス

class SleepLife : public Task
{
private:
	CImage m_REMText;		//レム睡眠文字
	CImage m_NREMText;		//ノンレム睡眠文字
	int m_ChengeSleep;		//睡眠状態切り替え時間
public:
	static bool m_REM;
	//コンストラクタ
	SleepLife();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();

};