#include"Field.h"
#include"FieldFloor.h"
#include"FieldWall.h"
#include"FieldObject/Door.h"
#include"Light/Light.h"
#include"../Navigation/NavManager.h"
#include"../Navigation/NavNode.h"
#include"../ObjectBase/ObjectBase.h"
#include"GameScene/GameData.h"

Field* Field::ms_instance = nullptr;

// 経路探索用ノードのテーブル
std::list<CVector3D> Field::ms_nodes =
{
	CVector3D(-1.119953,1,5.792867),		//1階廊下
	CVector3D(-7.946468,1,6.155985),
	CVector3D(-7.979040,1,3.100000),
	CVector3D(-10.848549,1,3.523031),		//洗面所
	CVector3D(2.369029,1,-0.000122),		//ソファー前
	CVector3D(5.275981,1,1.853063),
	CVector3D(6.315304,1,5.931384),
	CVector3D(-0.969914,1,16.858345),		//玄関前
	CVector3D(-0.936397,1,12.954982),		//1階階段前
	CVector3D(-7.932617,5.860001,12.954982),//階段
	CVector3D(-5.252072,4.460001,12.954982),
	CVector3D(-7.932617,8.660000,9.414995),
	CVector3D(-7.932617,10.053699,6.816346),//2階階段前
	CVector3D(1.205202,10.053699,7.116930),	//2階廊下
	CVector3D(6.316210,10.053699,7.119658),	
	CVector3D(-10.846934,10.053699,6.613060),
	CVector3D(-10.756325,10.053700,13.386012),
	CVector3D(0.943921,10.053699,1.877687),	//両親部屋
};

//ライト用のテーブル
std::list<CVector3D> ms_lights =
{
	CVector3D(2.8,1.5,2.1),		//リビング
	CVector3D(1,1.5,12.1),		//1階廊下①
	CVector3D(-5,1.5,5.3),		//1階廊下②
	CVector3D(-10.5,1.5,2.5),	//洗面所
	CVector3D(-4.6,3.7,12.9),	//階段
	CVector3D(-7.0,6.8,13.0),
	CVector3D(-7.0,11.8,13.0),
	CVector3D(-7.9,8.1,10.0),	
	CVector3D(-7.7,9.8,6.6),//2階廊下
	CVector3D(-4.2,9.8,6.6),
	CVector3D(0.4,9.8,6.7),
	CVector3D(5.0,9.8,6.8),
	CVector3D(-10.6,9.8,12.5),
	CVector3D(-10.6,9.8,8.5),
	CVector3D(-2.0,10.5,0.4),//両親部屋
	CVector3D(5.8,9.8,0.6),//子供部屋
	
};

//コンストラクタ
Field::Field(const char* Doorfile)
	:ObjectBase(ETaskTag::eField,true)
{
	ms_instance = this;

	//コリジョンモデルの取得
	switch (GameData::Stage)
	{
	case GameData::A_1:
		m_colModel = GET_RESOURCE("MapCol", CModel);
		m_colWallModel = GET_RESOURCE("WallMapCol", CModel);
		break;
	case GameData::B_1:
		m_colModel = GET_RESOURCE("MapCol", CModel);
		m_colWallModel = GET_RESOURCE("WallMapCol", CModel);
		break;
	case GameData::B_2:
		m_colModel = GET_RESOURCE("MapCol", CModel);
		m_colWallModel = GET_RESOURCE("WallMapCol", CModel);
		break;
	}
	

	//床と壁のクラスを作成
	m_floor = new FieldFloor();
	m_wall = new FieldWall();

	m_lightNo = 2;
	//経路探索用のノードを作成
	CreateNavNodes();
	//ライト作成
	CreateLights();
	//ドア作成
	CreateDoors(Doorfile);
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

//ステージにライトを作成
void Field::CreateLights()
{
	//テーブル内の座標にライトを作成
	for (CVector3D lightsPos : ms_lights)
	{
		//リビング
		if (m_lightNo <= 2)	
		{
			//2～番は部屋用ライト番号
			new Light(lightsPos, m_lightNo, 0, 8.0f, CLight::eLight_Point, true);
			m_lightNo++;
		}
		else if (m_lightNo <= 3)
		{
			//2～番は部屋用ライト番号
			new Light(lightsPos, m_lightNo, 1, 7.0f, CLight::eLight_Point, false);
			m_lightNo++;
		}
		else if (m_lightNo <= 4)
		{
			//2～番は部屋用ライト番号
			new Light(lightsPos, m_lightNo, 2, 5.0f, CLight::eLight_Point, false);
			m_lightNo++;
		}
		else if (m_lightNo <= 5)
		{
			//2～番は部屋用ライト番号
			new Light(lightsPos, m_lightNo, 3, 3.5f, CLight::eLight_Point, false);
			m_lightNo++;
		}
		else if (m_lightNo <= 9)
		{
			//2～番は部屋用ライト番号
			new Light(lightsPos, m_lightNo, 4, 5.0f, CLight::eLight_Point, false);
			m_lightNo++;
		}
		else if (m_lightNo <= 15)
		{
			//2～番は部屋用ライト番号
			new Light(lightsPos, m_lightNo, 5, 4.0f, CLight::eLight_Point, false);
			m_lightNo++;
		}
		else if (m_lightNo <= 16)
		{
			//2～番は部屋用ライト番号
			new Light(lightsPos, m_lightNo, 6, 6.0f, CLight::eLight_Point, false);
			m_lightNo++;
		}
		else if (m_lightNo <= 17)
		{
			//2～番は部屋用ライト番号
			new Light(lightsPos, m_lightNo, 7, 5.0f, CLight::eLight_Point, false);
			m_lightNo++;
		}
		
	}
}

//ドア生成処理
void Field::CreateDoors(const char* file)
{
	FILE* fp = NULL;

	//データをテキスト読み込みでオープン
	fopen_s(&fp, file, "r");
	//開くのに失敗
	if (!fp)return;
	
	char buf[256] = "";

	//ファイルの末尾まで繰り返す
	while (!feof(fp))
	{
		//一行づつbufに格納
		fgets(buf, 256, fp);

		//ドアの座標
		CVector3D Pos(0, 0, 0);
		//ドアの向き
		float Rot;

		sscanf_s(buf,"%f %f %f %f", &Pos.x, &Pos.y, &Pos.z, &Rot);

		new Door(Pos,CVector3D(0,DtoR(Rot),0), CVector3D(1, 1, 1), CVector3D(1, 2, 0.3));
	}
	fclose(fp);
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
	// フィールドが作成されていなければ、スルー
	Field* f = ms_instance;
	if (f == nullptr) return false;

	// コリジョンモデルが作成されていなければ、スルー
	CModel* col = f->GetColWallModel();
	if (col == nullptr) return false;

	// コリジョンが存在すれば、レイとのヒット判定の結果を返す
	return col->CollisionRay(outHitPos, outHitNormal, start, end);
	
}
