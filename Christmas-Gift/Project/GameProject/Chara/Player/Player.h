#pragma once
#include"../CharaBase/CharaBase.h"

class Camera;
class Filta;
class SleepLife;
class Light;

class Player : public CharaBase {
public:
	bool m_hide;			//ステルス状態フラグ
	bool m_islegsound;		//足音のフラグ
	bool m_CheckKill;		//プレイヤー死亡フラグ

	CVector3D m_Closet_rot; //カメラ用のクローゼットの正面方向ベクトル
	CVector3D m_Closet_pos;	//クローゼット内部の座標
private:
	Camera* mp_camera;		//カメラ
	Filta* mp_filta;		//フィルター
	SleepLife* mp_sleeplife;//睡眠ゲージ
	Light* mp_light;		//ライト

	float key_ang;			//方向ベクトルの角度
	CVector3D m_copy_pos;	//タンスに入るときのプレイヤーの座標
	int m_HideCount;
	bool m_HideAnim;

	CModelA3M m_model;		//モデルデータ

	
	//デバッグ用
	CVector3D m_lS;
	CVector3D m_lE;

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
		eState_Hide,		//隠れた状態
	};
	int m_state;			//ステータス状態

	//ステータス関数
	//通常状態
	void StateIdle();
	//しゃがみ状態
	void StateSquat();
	//隠れた状態
	void StateHide();

public:
	//コンストラクタ
	Player(const CVector3D& pos, const CVector3D& scale);

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