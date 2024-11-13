#pragma once
#include"../CharaBase/CharaBase.h"

class Camera;
class Switch;
class GimmickObject;
class ToolTips;
class Closet;

//プレイヤー機能クラス

class Player : public CharaBase {
private:
	Switch* mp_switch;		//スイッチ
	Camera* m_camera;		//カメラ
	ToolTips* m_tooltips;	//ツールチップ
	NavNode* node;			//ノード
	Closet* mp_closet;		//クローゼット

	float key_ang;			//方向ベクトルの角度
	
	//デバッグ用
	CVector3D m_lS;			//レイの始点
	CVector3D m_lE;			//レイの終点

	//アニメーションID
	enum class AnimId
	{
		Idle,				//待機
		Walk,				//歩き
		Crouch,				//しゃがみ
		Stand,				//立つ
		Crouchidle,			//しゃがみ待機
		Crouchwalk,			//しゃがみ歩き
	};

	//状態変化
	enum {
		eState_Idle,		//通常状態
		eState_Squat,		//しゃがみ状態
		eState_ClosetIn,	//クローゼットに入った状態
	};

	//ステータス関数
	//通常状態
	void StateIdle();
	//しゃがみ状態
	void StateSquat();
	//隠れた状態
	void StateClosetIn();

public:
	int m_state;			//ステータス状態
	bool m_hide;			//ステルス状態フラグ

	CVector3D m_Closet_rot; //カメラ用のクローゼットの正面方向ベクトル
	CVector3D m_Closet_pos;	//クローゼット内部の座標
	CVector3D m_copy_pos;	//タンスから元の位置に出るためのプレイヤーの座標
	//コンストラクタ
	Player(const CVector3D& pos,const CVector3D& rot, const CVector3D& scale);

	//デストラクタ
	~Player();

	//更新処理
	void Update();

	//描画処理
	void Render();

	//衝突処理
	void Collision(Task* t);

	//レティクル
	void Shot();
};