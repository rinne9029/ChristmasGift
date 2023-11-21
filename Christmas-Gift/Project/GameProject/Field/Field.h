#pragma once
#include"GLLibrary/CModel.h"
#include"ObjectBase/ObjectBase.h"

class FieldFloor;
class FieldWall;
class NavNode;

//フィールド制御クラス
class Field : public ObjectBase {
private:
	//フィールドクラスのインスタンス
	static Field* ms_instance;
	//経路探索用ノードのテーブル
	static std::list<CVector3D>ms_nodes;

	CModel* m_colModel;		//フィールドのコリジョンモデル
	CModel* m_colWallModel;	//壁のコリジョンモデル
	FieldFloor* m_floor;		//床描画用のクラス
	FieldWall* m_wall;			//壁描画用のクラス
	NavNode* m_node;

public:
	//コンストラクタ
	Field();
	//デストラクタ
	~Field();

	//経路探索用のノードを作成
	void CreateNavNodes();

	//フィールドのコリジョンを取得
	CModel* GetColModel() const;

	//壁のコリジョンを取得
	CModel* GetColWallModel() const;

	//指定したレイと壁のコリジョンとのヒット判定
	static bool CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal);
};