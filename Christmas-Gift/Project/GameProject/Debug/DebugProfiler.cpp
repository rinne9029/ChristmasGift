#include "DebugProfiler.h"
#include "DebugPrint.h"

// DebugProfilerのインスタンス
DebugProfiler* DebugProfiler::ms_instance = nullptr;

// コンストラクタ
DebugProfiler::DebugProfiler()
	: Task(ETaskTag::eDebug)
{
	m_remove = false;
}

// デストラクタ
DebugProfiler::~DebugProfiler()
{
}

// インスタンス取得
DebugProfiler* DebugProfiler::Instance()
{
	if (ms_instance == nullptr)
	{
		ms_instance = new DebugProfiler();
	}
	return ms_instance;
}

// 時間計測開始
void DebugProfiler::StartTimer(std::string name)
{
	DebugProfiler* inst = Instance();
	auto& timers = inst->m_timers;

	DebugTimer* timer = timers[name];
	if (timer == nullptr)
	{
		timer = new DebugTimer();
		timers[name] = timer;
	}

	timer->Start();
}

// 時間計測終了
void DebugProfiler::EndTimer(std::string name)
{
	DebugProfiler* inst = Instance();
	auto& timers = inst->m_timers;

	DebugTimer* timer = timers[name];
	if (timer == nullptr)
	{
		timer = new DebugTimer();
		timers[name] = timer;
	}

	timer->End();
}

// 計測結果を描画
void DebugProfiler::Print()
{
	DebugProfiler* inst = Instance();
	auto& timers = inst->m_timers;

	if (timers.size() > 0)
	{
		DebugPrint::Print("■■■■■■■ Profiler ■■■■■■■");
		DebugPrint::Print("Delta:%f", CFPS::GetDeltaTime());
		for (auto& timer : timers)
		{
			DebugTimer* dt = timer.second;
			if (dt->IsEnd())
			{
				DebugPrint::Print
				(
					"%s:経過時間[%f]:割合[%f]",
					timer.first.c_str(),
					dt->Get(),
					(dt->Get() / CFPS::GetDeltaTime()) * 100.0f
				);
			}
			else
			{
				DebugPrint::Print
				(
					"%s:E[%f]",
					timer.first.c_str(),
					dt->Get()
				);
			}
		}
		DebugPrint::Print("■■■■■■■■■■■■■■■■■■■");
	}
}

DebugTimer::DebugTimer()
	: freq({ 0 })
	, startCount({ 0 })
	, endCount({ 0 })
	, isEnd(false)
{
}

void DebugTimer::Start()
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&startCount);
	isEnd = false;
}

void DebugTimer::End()
{
	QueryPerformanceCounter(&endCount);
	isEnd = true;
}

float DebugTimer::Get() const
{
	if (isEnd)
	{
		return (float)(endCount.QuadPart - startCount.QuadPart) / freq.QuadPart;
	}
	else
	{
		LARGE_INTEGER tempCount;
		QueryPerformanceCounter(&tempCount);
		return (float)(tempCount.QuadPart - startCount.QuadPart) / freq.QuadPart;
	}
}

bool DebugTimer::IsEnd() const
{
	return isEnd;
}