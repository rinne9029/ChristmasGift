#pragma once

//アクション用当たり判定のベース

class CollisionBoxBase : public Task
{
public:
	//コンストラクタ
	CollisionBoxBase(ETaskTag tag,bool remove);

	virtual ~CollisionBoxBase();
	//描画処理
	void Render();
};