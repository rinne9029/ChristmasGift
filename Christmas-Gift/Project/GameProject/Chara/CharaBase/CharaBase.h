#pragma once
#include"ObjectBase/ObjectBase.h"

//キャラクターのベースクラス

//重力加速度
#define GRAVITY (0.02f)

class NavNode;

class CharaBase : public ObjectBase
{
protected:
	CModelA3M m_model;		//モデルデータ
	NavNode* m_navNode;		//検索ノード

	CVector3D m_scale;		//キャラクター描画サイズ

	bool m_isGround;		//着地フラグ

	float m_movespeed;		//移動速度

public:
	//コンストラクタ
	CharaBase(ETaskTag tag,bool remove);

	//デストラクタ
	virtual ~CharaBase();

	//衝突判定
	void Collision(Task* t);

	NavNode* GetNavNode() const;

	//更新
	void Update();
};
