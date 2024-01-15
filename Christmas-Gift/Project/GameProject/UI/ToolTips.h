#pragma once

class ToolTips : public Task
{
private:
	CImage m_Image;
public:
	bool isDraw;
	//コンストラクタ
	ToolTips();
	//デストラクタ
	~ToolTips();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};