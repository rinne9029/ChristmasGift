#pragma once
#include"../CharaBase/CharaBase.h"

class Player;
class NavNode;
class SearchNode;

//敵制御クラス

//家族
class Enemy : public CharaBase {
private:
	Player* mp_player;			//プレイヤー
	CVector3D m_moveDir;		//移動方向
	CVector3D m_rot_target;		//プレイヤー方向の回転値
	CVector3D m_movePos;		//移動先の座標
	NavNode* m_moveNode;		//移動目的地のノード
	float m_elapsedTime;		//経過時間計測用
	bool m_isvigilance;			//警戒フラグ
	int m_modelno;				//モデル外見
	float m_warning;			//警戒値
	
	//目
	float eye_length;			//視野距離
	float eye_ang;				//視野角度

	int walk_se_idx;			//歩き効果音のフラグ 0:右足 1:左足
	int walk_se_frame[2];		//歩き効果音発生フレーム 0:右足 1:左足

	NavNode* m_nextNode;		//次に移動するノード格納用
	NavNode* m_lostNode;		//プレイヤーを見失った場所に配置
	SearchNode* m_searchNode;	//探索中のノード

	CVector4D color;			//視野範囲の色
	COBB m_WarningGauge;		//警戒値のゲージ

	//アニメーション番号
	enum class AnimId
	{
		Idle,		//待機
		Walk,		//歩く
		Run,		//走る
		Sit,		//座る
		SitIdle,	//座り待機
		Up,			//立つ
	};
	
	//状態変化
	enum {
		eState_Idle,			//待機状態
		eState_Move,			//移動状態
		eState_Look,			//警戒状態
		eState_Chase,			//追跡状態
		eState_Lost,			//プレイヤーを見失った状態
	};

	int m_state;	//現在の状態

	//待機状態
	void StateIdle();	
	//警戒状態
	void StateLook();
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

	//現在位置からプレイヤーが見えるかどうかのフラグ
	bool IsLookPlayer() const;
public:
	//コンストラクタ
	Enemy(const CVector3D& pos,const CVector3D& dir, const CVector3D& scale, int model);
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