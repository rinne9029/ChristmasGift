#pragma once

//当たり判定ベース

class ToolTips;

class CollisionBoxBase : public Task
{
public:
	ToolTips* m_tooltips;
	
	bool m_IsRender;
	//コンストラクタ
	CollisionBoxBase(ETaskTag tag,bool remove);

	virtual ~CollisionBoxBase();
	//描画処理
	void Render();
};