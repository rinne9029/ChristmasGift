#include"Field.h"
#include"FieldFloor.h"
#include"FieldWall.h"
#include"../Navigation/NavManager.h"
#include"../Navigation/NavNode.h"
#include"../ObjectBase/ObjectBase.h"
#include"GameScene/GameData.h"
#include"Debug/DebugPrint.h"
#include"Debug/DebugProfiler.h"

Field* Field::ms_instance = nullptr;

// 経路探索用ノードのテーブル
std::list<CVector3D> Field::ms_nodes =
{
	CVector3D(-3.136362f, 0.0f, -27.636364f),
	CVector3D(-3.136362f, 0.0f, -15.272726f),
	CVector3D(-3.136362f, 0.0f, -2.909088f),
	CVector3D(-3.136362f, 0.0f, 3.272728f),
	CVector3D(-3.136362f, 0.0f, 9.454548f),
	CVector3D(-3.136362f, 0.0f, 15.636364f),
	CVector3D(-3.136362f, 0.0f, 21.818184f),

	CVector3D(-9.409090f, 0.0f, -33.818180f),
	CVector3D(-9.409090f, 0.0f, -21.454544f),
	CVector3D(-9.409090f, 0.0f, -9.090908f),

	CVector3D(-15.681818f, 0.0f, -40.000000f),
	CVector3D(-15.681818f, 0.0f, -27.636364f),
	CVector3D(-15.681818f, 0.0f, -9.090908f),
	CVector3D(-15.681818f, 0.0f, -2.909088f),
	CVector3D(-15.681818f, 0.0f, 3.272728f),

	CVector3D(-21.954544f, 0.0f, -40.000000f),
	CVector3D(-21.954544f, 0.0f, -27.636364f),
	CVector3D(-21.954544f, 0.0f, -21.454544f),
	CVector3D(-21.954544f, 0.0f, -9.090908f),
	CVector3D(-21.954544f, 0.0f, -2.909088f),
	CVector3D(-21.954544f, 0.0f, 9.454548f),
	CVector3D(-21.954544f, 0.0f, 15.636364f),

	CVector3D(-28.227272f, 0.0f, -33.818180f),
	CVector3D(-28.227272f, 0.0f, -27.636364f),
	CVector3D(-28.227272f, 0.0f, -15.272726f),
	CVector3D(-28.227272f, 0.0f, -9.090908f),
	CVector3D(-28.227272f, 0.0f, 3.272728f),
	CVector3D(-28.227272f, 0.0f, 21.818184f),
	CVector3D(-28.227272f, 0.0f, 28.000000f),

	CVector3D(-34.500000f, 0.0f, -40.000000f),
	CVector3D(-34.500000f, 0.0f, -21.454544f),
	CVector3D(-34.500000f, 0.0f, -9.090908f),
	CVector3D(-34.500000f, 0.0f, -2.909088f),
	CVector3D(-34.500000f, 0.0f, 28.00000f),

	CVector3D(3.136364f, 0.0f, -33.818180f),
	CVector3D(3.136364f, 0.0f, -27.636364f),
	CVector3D(3.136364f, 0.0f, -21.454544f),
	CVector3D(3.136364f, 0.0f, -15.272726f),
	CVector3D(3.136364f, 0.0f, -9.090908f),
	CVector3D(3.136364f, 0.0f, -2.909088f),
	CVector3D(3.136364f, 0.0f, 3.272728f),
	CVector3D(3.136364f, 0.0f, 9.454548f),
	CVector3D(3.136364f, 0.0f, 21.818184f),
	CVector3D(3.136364f, 0.0f, 28.000000f),

	CVector3D(9.409092f, 0.0f, -27.636364f),
	CVector3D(9.409092f, 0.0f, -15.272726f),
	CVector3D(9.409092f, 0.0f, 3.272728f),
	CVector3D(9.409092f, 0.0f, 15.636364f),
	CVector3D(9.409092f, 0.0f, 21.818184f),
	CVector3D(9.409092f, 0.0f, 28.000000f),

	CVector3D(15.681820f, 0.0f, -40.000000f),
	CVector3D(15.681820f, 0.0f, -33.818180f),
	CVector3D(15.681820f, 0.0f, -15.272726f),
	CVector3D(15.681820f, 0.0f, -2.909088f),
	CVector3D(15.681820f, 0.0f, 3.272728f),
	CVector3D(15.681820f, 0.0f, 15.636364f),

	CVector3D(21.954544f, 0.0f, -40.000000f),
	CVector3D(21.954544f, 0.0f, -21.454544f),
	CVector3D(21.954544f, 0.0f, -15.272726f),
	CVector3D(21.954544f, 0.0f, -9.090908f),

	CVector3D(28.227276f, 0.0f, -40.000000f),
	CVector3D(28.227276f, 0.0f, -33.818180f),
	CVector3D(28.227276f, 0.0f, -21.454544f),

	CVector3D(34.500000f, 0.0f, -40.000000f),
	CVector3D(34.500000f, 0.0f, -33.818180f),
	CVector3D(34.500000f, 0.0f, -15.272726f),
	CVector3D(34.500000f, 0.0f, -9.090908f),
	CVector3D(34.500000f, 0.0f, -2.909088f),
	CVector3D(34.500000f, 0.0f, 3.272728f),
};

//コンストラクタ
Field::Field()
	:ObjectBase(ETaskTag::eField)
{
	m_remove = true;
	ms_instance = this;

	//コリジョンモデルの取得
	switch (GameData::Stage)
	{
	case GameData::A_1:
		m_colModel = GET_RESOURCE("FieldCol", CModel);
		m_colWallModel = GET_RESOURCE("FieldWallCol", CModel);
		break;
	case GameData::B_1:
		m_colModel = GET_RESOURCE("FieldCol", CModel);
		m_colWallModel = GET_RESOURCE("FieldWallCol", CModel);
		break;
	case GameData::B_2:
		m_colModel = GET_RESOURCE("FieldCol", CModel);
		m_colWallModel = GET_RESOURCE("FieldWallCol", CModel);
		break;
	}
	

	//床と壁のクラスを作成
	m_floor = new FieldFloor();
	m_wall = new FieldWall();

	//経路探索用のノードを作成
	CreateNavNodes();
}

Field::~Field()
{
	if (ms_instance == this)
	{
		ms_instance = nullptr;
	}
}

//経路探索用のノードを作成
void Field::CreateNavNodes()
{
	//テーブル内の座標に経路探索用のノードを作成
	for (CVector3D nodePos : ms_nodes)
	{
		new NavNode(nodePos);
	}
}

//フィールドのコリジョンを取得
CModel* Field::GetColModel() const
{
	return m_colModel;
}

CModel* Field::GetColWallModel() const
{
	return m_colWallModel;
}

// 指定したレイと壁のコリジョンとのヒット判定
bool Field::CollisionRay(const CVector3D& start, const CVector3D& end, CVector3D* outHitPos, CVector3D* outHitNormal)
{
	//DebugProfiler::StartTimer("FieldCollision");
	// フィールドが作成されていなければ、スルー
	Field* f = ms_instance;
	if (f == nullptr) return false;

	// コリジョンモデルが作成されていなければ、スルー
	CModel* col = f->GetColWallModel();
	if (col == nullptr) return false;

	//DebugProfiler::EndTimer("FieldCollision");
	// コリジョンが存在すれば、レイとのヒット判定の結果を返す
	return col->CollisionRay(outHitPos, outHitNormal, start, end);
	
}
