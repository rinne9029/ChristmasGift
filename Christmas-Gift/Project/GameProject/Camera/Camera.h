#pragma once
#include"ObjectBase/ObjectBase.h"

class Player;
class Closet;

class Camera : public ObjectBase {
private:
	Player* mp_player;		//プレイヤー
	Closet* mp_closet;		//アイテム
	int m_state;			//現在の状態
	//状態変化
	enum {
		eState_Idle,		//通常状態
		eState_Hide,		//隠れた状態
	};
public:
	int m_camera_mode;		//現在のカメラのモード
	//カメラの状態
	enum {
		FirstPersonCamera,	//一人称
		LookDownCamera,		//見下ろし
		ThirdPersonCamera,	//三人称
	};
	//通常状態
	void StateIdle();
	//隠れた状態
	void StateHide();		
public:
	//コンストラクタ
	Camera(const CVector3D& pos, const CVector3D& rot);
	//更新処理
	void Update();
	//描画処理
	void Render();
};