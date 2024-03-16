#pragma once

//睡眠ゲージ管理クラス

class SleepLife : public Task
{
private:
	CImage m_SleepMeter;	//睡眠値
	CImage m_FaceRelief;	//大丈夫
	CImage m_FaceUsually;	//普通
	CImage m_FaceWorry;		//やばい
	CImage m_REMText;		//レム睡眠文字
	CImage m_NREMText;		//ノンレム睡眠文字
	int m_ChengeSleep;		//睡眠状態切り替え時間
	int m_ChengeFace;		//顔切り替え変数
public:
	static bool m_REM;		//睡眠の状態フラグ
	//コンストラクタ
	SleepLife();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();

};