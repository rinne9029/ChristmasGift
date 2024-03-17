#include"Field.h"
#include"FieldFloor.h"
#include"FieldWall.h"
#include"FieldObject/Door.h"
#include"Field/FieldObject/Switch.h"
#include"Field/FieldObject/Closet.h"
#include"Light/Light.h"
#include"../Navigation/NavManager.h"
#include"../Navigation/NavNode.h"
#include"../ObjectBase/ObjectBase.h"
#include"GameScene/GameData.h"

Field* Field::ms_instance = nullptr;

//コンストラクタ
Field::Field(const char* Nodefile,const char* Lightfile,const char* Doorfile,const char* Switchfile,const char* Closetfile)
	:ObjectBase(ETaskTag::eField,true)
{
	ms_instance = this;

	//コリジョンモデルの取得
	m_colModel = GET_RESOURCE("MapCol", CModel);
	m_colWallModel = GET_RESOURCE("WallMapCol", CModel);
	
	//床と壁のクラスを作成
	m_floor = new FieldFloor();
	m_wall = new FieldWall();

	m_lightNo = 2;
	//経路探索用のノードを作成
	CreateNavNodes(Nodefile);
	//ライト作成
	CreateLights(Lightfile);
	//ドア作成
	CreateDoors(Doorfile);
	//スイッチ作成
	CreateSwitchs(Switchfile);
	//クローゼット作成
	CreateCloset(Closetfile);
}

Field::~Field()
{
	if (ms_instance == this)
	{
		ms_instance = nullptr;
	}
}

//経路探索用のノードを作成
void Field::CreateNavNodes(const char* file)
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

		//ノードの座標
		CVector3D Pos(0, 0, 0);

		sscanf_s(buf, "%f %f %f", &Pos.x, &Pos.y, &Pos.z);

		//ノード生成
		new NavNode(Pos);
	}
	fclose(fp);
}

//ステージにライトを作成
void Field::CreateLights(const char* file)
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

		CVector3D Pos(0, 0, 0);			//ライトの座標
		int No = 0;						//ライトナンバー
		float Range = 0;				//光の大きさ
		int IsShine = 0;				//光っているかの判断
		bool Shine = true;				

		sscanf_s(buf, "%f %f %f %d %f %d", &Pos.x, &Pos.y, &Pos.z, &No, &Range, &IsShine);

		//値が0なら電気はついていない
		if (IsShine == 0) Shine = false;

		//ライト生成
		new Light(Pos,m_lightNo,No,Range, CLight::eLight_Point,Shine);

		m_lightNo++;
	}
	fclose(fp);

}

//ドアを作成
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

		//ドア生成
		new Door(Pos,CVector3D(0,DtoR(Rot),0), CVector3D(1, 1, 1), CVector3D(1, 2, 0.3));
	}
	fclose(fp);
}

//スイッチを作成
void Field::CreateSwitchs(const char* file)
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

		//スイッチの座標
		CVector3D Pos(0, 0, 0);
		//スイッチの向き
		float Rot;
		//スイッチの番号
		float No;

		sscanf_s(buf, "%f %f %f %f %f", &Pos.x, &Pos.y, &Pos.z, &Rot, &No);

		//スイッチ生成
		new Switch(Pos, CVector3D(0, DtoR(Rot), 0), CVector3D(1, 1, 1),No);
	}
	fclose(fp);
}

void Field::CreateCloset(const char* file)
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

		//クローゼットの座標
		CVector3D Pos(0, 0, 0);
		//クローゼットの向き
		float Rot;

		sscanf_s(buf, "%f %f %f %f", &Pos.x, &Pos.y, &Pos.z, &Rot);

		//クローゼット生成
		new Closet(Pos, CVector3D(0, DtoR(Rot), 0), CVector3D(1, 0.7, 1),2);
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
