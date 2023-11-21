#include"EnemyManager.h"
#include"Enemy.h"

//敵の管理クラスのインスタンス
EnemyManager* EnemyManager::ms_instance = nullptr;
//敵の探索位置ノードのリスト
std::list<SearchNode> EnemyManager::ms_searchNodes =
{
	SearchNode(CVector3D(-28.227272f, 0.0f, -33.818180f)),
	SearchNode(CVector3D(-28.227272f, 0.0f, -15.272726f)),
	SearchNode(CVector3D(-9.409090f, 0.0f, -33.818180f)),
	SearchNode(CVector3D(-3.136362f, 0.0f, -27.636364f)),
	SearchNode(CVector3D(3.136364f, 0.0f, 3.272728f)),
	SearchNode(CVector3D(3.136364f, 0.0f, 9.454548f)),
	SearchNode(CVector3D(25.090910f, 0.0f, 18.727274f)),
	SearchNode(CVector3D(34.500000f, 0.0f, -40.000000f)),
};

//コンストラクタ
EnemyManager::EnemyManager()
	:CharaBase(ETaskTag::eEnemyManager)
{
	m_remove = true;
}

//デストラクタ
EnemyManager::~EnemyManager()
{
	ms_instance = nullptr;
}

//インスタンスを取得
EnemyManager* EnemyManager::Instance()
{
	//インスタンスがなければ
	if (ms_instance == nullptr)
	{
		//インスタンスを生成する
		ms_instance = new EnemyManager();
	}
	return ms_instance;
}

//敵をリストに追加
void EnemyManager::Add(Enemy* enemy)
{
	m_enemies.push_back(enemy);
}

//敵をリストから取り除く
void EnemyManager::Remove(Enemy* enemy)
{
	m_enemies.remove(enemy);
}

//次に探索するノードを取得
SearchNode* EnemyManager::GetNextSearchNode() const
{
	//□探索ノードの選択条件
	//①他の敵が探索中のノードは選択しない
	//②存在確率が一番高いノードを選択する
	
	SearchNode* ret = nullptr;
	//リストの先頭から順番に調べて、
	//一番条件に近い一致するノードを取得する
	for (SearchNode& node : ms_searchNodes)
	{
		//既に他の敵が探索中のノードは対象外
		if (node.enemy != nullptr) continue;
		//戻り値のノードがまだ設定されていないもしくは、
		//存在確率が検索中のノードの法が高いのであれば、
		//検索中のノードを優先する
		if (ret == nullptr || node.prob > ret->prob)
		{
			ret = &node;
		}
	}
	return ret;
}

//更新
void EnemyManager::Update()
{
	for (SearchNode& node : ms_searchNodes)
	{
		node.prob = Utility::Clamp01(node.prob + 0.0005f);
	}
}

//描画
void EnemyManager::Render()
{
	if (!g_isRenderDebug) return;

	for (SearchNode& node : ms_searchNodes)
	{
		float per = node.prob;
		CVector3D color;
		if (per > 0.5f)
		{
			color = CVector3D::Leap
			(
				CVector3D(1.0f, 1.0f, 0.0f),
				CVector3D(1.0f, 0.0f, 0.0f),
				(per - 0.5f) / 0.5f
			);
		}
		else
		{
			color = CVector3D::Leap
			(
				CVector3D(0.0f, 1.0f, 0.0f),
				CVector3D(1.0f, 1.0f, 0.0f),
				per / 0.5f
			);
		}
		Utility::DrawCube(node.pos, 2.0f, CVector4D(color, 1.0f));
	}
}