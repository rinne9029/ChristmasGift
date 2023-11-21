#include "DebugPrint.h"
#include <stdarg.h>

DebugPrint* DebugPrint::ms_instance = nullptr;

// コンストラクタ
DebugPrint::DebugPrint()
	: Task(ETaskTag::eDebug)
{
	m_remove = false;

	m_font = FONT("DebugPrint");
}

// デストラクタ
DebugPrint::~DebugPrint()
{
}

// インスタンス取得
DebugPrint* DebugPrint::Instance()
{
	if (ms_instance == nullptr)
	{
		ms_instance = new DebugPrint();
	}
	return ms_instance;
}

// デバッグ文字列をプリント
void DebugPrint::Print(const char* format, ...)
{
	char buf[512];
	va_list ap;
	va_start(ap, format);
	vsprintf_s(buf, format, ap);
	va_end(ap);

	Instance()->AddStr(buf);
}

// 表示する文字列をリストに追加
void DebugPrint::AddStr(std::string str)
{
	m_printStrList.push_back(str);
}

// 更新
void DebugPrint::Update()
{
}

// 描画
void DebugPrint::Render()
{
	// 1行の幅
	float rowSize = m_font->GetFontSize();
	// Y軸開始位置
	float startPosY = rowSize * 2.0f;
	// リスト内の文字列を全て描画
	for (int i = 0; i < m_printStrList.size(); i++)
	{
		// 文字列描画
		m_font->Draw
		(
			0, (int)(startPosY + rowSize * i),
			1.0f, 0.0f, 0.0f,
			m_printStrList[i].c_str()
		);
	}
	// リストをクリア
	m_printStrList.clear();
}