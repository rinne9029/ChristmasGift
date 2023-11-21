#pragma once
#include <map>
#include"Task/Task.h"
class DebugTimer;

// 処理時間計測用のデバッグクラス
class DebugProfiler : public Task
{
private:
	static DebugProfiler* ms_instance;
	std::map<std::string, DebugTimer*> m_timers;

	// コンストラクタ
	DebugProfiler();
	// インスタンス取得
	static DebugProfiler* Instance();

public:
	// デストラクタ
	~DebugProfiler();

	// 時間計測開始
	static void StartTimer(std::string name);
	// 時間計測終了
	static void EndTimer(std::string name);

	// 計測結果を描画
	static void Print();
};

class DebugTimer
{
private:
	LARGE_INTEGER freq;
	LARGE_INTEGER startCount;
	LARGE_INTEGER endCount;
	bool isEnd;

public:
	DebugTimer();

	void Start();
	void End();
	float Get() const;
	bool IsEnd() const;
};