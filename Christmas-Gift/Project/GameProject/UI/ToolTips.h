#pragma once

//ツールチップ管理クラス

class ToolTips : public Task
{
private:
	CImage m_Image;
	
public:
	char* m_Text;
	bool m_Iskill;
	//コンストラクタ
	ToolTips();
	//2D描画処理
	void Draw();
};