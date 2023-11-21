#pragma once
#include"../CharaBase/CharaBase.h"

class Player;
class NavNode;
class SearchNode;

//敵制御クラス

//家族
class Enemy : public CharaBase {
private:
	CModelA3M m_model;
	Player* mp_player;			//プレイヤー
	CVector3D m_moveDir;		//移動方向
	CVector3D m_rot_target;		//プレイヤー方向の回転値
	CVector3D m_movePos;		//移動先の座標
	NavNode* m_moveNode;		//移動目的地のノード
	float m_elapsedTime;		//経過時間計測用
	
	//目
	float eye_length;			//視野距離
	float eye_ang;				//視野角度

	//耳
	float ear_length;			//聴覚距離
	float ear_ang;				//聴覚角度

	bool m_isChasing;			//追跡中のフラグ
	NavNode* m_nextNode;		//次に移動するノード格納用
	NavNode* m_lostNode;		//プレイヤーを見失った場所に配置
	SearchNode* m_searchNode;	//探索中のノード

	CVector4D color;			//視野範囲の色

	int m_cnt;					//タイマー

	//アニメーション番号
	enum class AnimId
	{
		Idle,	//待機
		Walk,	//移動
	};
	
	//状態変化
	enum {
		eState_Idle,			//待機状態
		eState_Move,			//移動状態
		eState_Chase,			//追跡状態
		eState_Lost,			//プレイヤーを見失った状態
	};

	int m_state;	//現在の状態

	//通常状態
	void StateIdle();	
	//移動状態
	void StateMove();		
	//追跡状態
	void StateChase();	
	//プレイヤーを見失った状態
	void StateLost();

	//状態ごとの視野範囲の色
	void View_Color();

	//指定座標への移動処理
	bool MoveTo(const CVector3D& target);

	//探知フラグ
	bool IsEyeFoundPlayer();
	bool IsEarFoundPlayer();

	//現在位置からプレイヤーが見えるかどうかのフラグ
	bool IsLookPlayer() const;
public:
	//コンストラクタ
	Enemy(const CVector3D& pos, const CVector3D& scale);
	//デストラクタ
	~Enemy();

	//探索ノードを設定
	void SetSearchNode(SearchNode* node);
	//探索中のノードを取得
	SearchNode* GetSearchNode() const;
	
	//更新処理
	void Update();
	//描画処理
	void Render();
	//接触判定
	void Collision(Task* t);

};

class Child : public CharaBase
{
private:
public:
	//コンストラクタ
	Child(const CVector3D& pos,const CVector3D& vec);

	//デストラクタ
	~Child();

	//更新処理
	void Update();

	//描画処理
	void Render();
};