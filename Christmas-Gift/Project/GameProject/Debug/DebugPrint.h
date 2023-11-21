#pragma once
#include <vector>
#include <string>
#include"Task/Task.h"

// テキストのデバッグ表示クラス
class DebugPrint : public Task
{
private:
	// インスタンス
	static DebugPrint* ms_instance;
	// テキスト描画用のフォント
	CFont* m_font;
	// 表示する文字列のリスト
	std::vector<std::string> m_printStrList;

	// コンストラクタ
	DebugPrint();
	// インスタンス取得
	static DebugPrint* Instance();
	// 表示する文字列をリストに追加
	void AddStr(std::string str);

public:
	// デストラクタ
	~DebugPrint();

	// デバッグ文字列をプリント
	static void Print(const char* format, ...);

	// 更新
	void Update();
	// 描画
	void Render();
};