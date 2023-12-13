#pragma once
#include<string>

//タスクの種類
enum class ETaskTag
{
	eScene,						//ゲームシーン
	eField,						//ステージ
	eFieldFloor,				//床
	eFieldWall,					//壁
	eFieldLight,				//ステージに設置したライト
	eFieldObject,				//ステージにあるギミックオブジェクト
	eCamera,					//カメラ
	ePlayer,					//プレイヤー
	eEnemy,						//敵
	eNavNode,					//ノード
	eHideBox,					//隠れられる箱
	eWarpRoomBox,				//マップ移動用透明な当たり判定
	eWarpChildRoomBox,			//子供部屋移動用透明な当たり判定
	eEnemyManager,				//エネミーマネージャー
	eNavManager,				//ノードマネージャー
	eUI,
	eFilta,						//フィルター
	eDebug,						//デバッグ用
};
//レイヤー
enum TaskLayer {
	eDefault,
	eOption,
	eMax
};

//タスククラス
class Task
{
public:
	ETaskTag m_tag;			//オブジェクトごとの種類名
	bool m_remove;			//オブジェクトごとの削除フラグ

	CModelObj m_model;		//オブジェクトモデル

	CVector3D m_pos;		//オブジェクト座標
	CVector3D m_rot;		//オブジェクト回転値
	CVector3D m_at;			//オブジェクト注視点
	CVector3D m_vec;		//オブジェクト移動ベクトル
	CVector3D m_dir;		//オブジェクト前方向ベクトル

	//当たり判定用
	//球、カプセル用
	float m_rad;			//オブジェクト半径
	float m_height;			//オブジェクト高さ
	CVector3D m_lineS;		//オブジェクト線分始点
	CVector3D m_lineE;		//オブジェクト線分終点

	//箱形用
	COBB m_obb;				//箱形

	int m_kill;				//削除フラグ

private:
	//ETaskTag m_tag;			//タスクの種類
	bool m_isKill;			//削除フラグ

public:
	//コンストラクタ
	Task(ETaskTag tag ,bool Remove);

	//デストラクタ
	virtual~Task();

	//削除フラグON
	//void SetKill();

	//タスクの種類を取得
	ETaskTag GetTag() const;

	//オブジェクトの持つモデルの取得
	virtual CModel* GetModel() {
		return nullptr;
	}

	//オブジェクト同士の当たり判定検証
	virtual void Task::Collision(Task* t);

	//削除フラグを取得
	bool IsKill() const;

	//タスクを削除
	void Kill();

	//更新処理
	virtual void Update();

	//3D描画処理
	virtual void Render();

	//2D描画処理
	virtual void Draw();
};