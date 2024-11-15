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
#include"Item/key.h"

Field* Field::ms_instance = nullptr;

//コンストラクタ
Field::Field(const char* Nodefile,const char* Lightfile,const char* Doorfile,const char* Switchfile,const char* Closetfile)
	:ObjectBase(ETaskTag::eField,true)
{
	ms_instance = this;
	 
	//コリジョンモデルの取得
	m_colModel = GET_RESOURCE("MapCol", CModel);
	m_colWallModel = GET_RESOURCE("WallMapCol", CModel);
	
	m_keypos[0] = CVector3D(-11.5, 0.93, 3.58);
	m_keypos[1] = CVector3D(4.1,1.01,4.22);
	m_keypos[2] = CVector3D(-5.54,1.09,1.02);

	m_keyrot[0] = CVector3D(0, DtoR(90), DtoR(90));
	m_keyrot[1] = CVector3D(0, 0, 0);
	m_keyrot[2] = CVector3D(0, 0, 0);

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
	//鍵作成
	CreateKey();
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
		CVector3D Pos(0.0f, 0.0f, 0.0f);

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

		CVector3D Pos(0.0f, 0.0f, 0.0f);		//ライトの座標
		int No = 0;								//ライトナンバー
		float Range = 0.0f;						//光の大きさ
		int IsShine = 0;						//光っているかの判断
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
		CVector3D Pos(0.0f, 0.0f, 0.0f);
		//ドアの向き
		float Rot;

		sscanf_s(buf,"%f %f %f %f", &Pos.x, &Pos.y, &Pos.z, &Rot);

		//ドア生成
		new Door(Pos,CVector3D(0.0f,DtoR(Rot),0.0f), CVector3D(1.0f, 1.0f, 1.0f), CVector3D(1.0f, 2.0f, 0.3f));
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
		CVector3D Pos(0.0f, 0.0f, 0.0f);
		//スイッチの向き
		float Rot;
		//スイッチの番号
		float No;

		sscanf_s(buf, "%f %f %f %f %f", &Pos.x, &Pos.y, &Pos.z, &Rot, &No);

		//スイッチ生成
		new Switch(Pos, CVector3D(0.0f, DtoR(Rot), 0.0f), CVector3D(1.0f, 1.0f, 1.0f),No);
	}
	fclose(fp);
}

//クローゼットを作成
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
		CVector3D Pos(0.0f, 0.0f, 0.0f);
		//クローゼットの向き
		float Rot;

		sscanf_s(buf, "%f %f %f %f", &Pos.x, &Pos.y, &Pos.z, &Rot);

		//クローゼット生成
		new Closet(Pos, CVector3D(0.0f, DtoR(Rot), 0.0f), CVector3D(1.0f, 0.7f, 1.0f),2);
	}
	fclose(fp);
}

//鍵を作成
void Field::CreateKey()
{
	new key(CVector3D(m_keypos[0]), CVector3D(m_keyrot[0]), CVector3D(0.3, 0.3, 0.3));
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
