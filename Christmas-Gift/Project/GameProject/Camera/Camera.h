#pragma once
#include"ObjectBase/ObjectBase.h"

class Player;

//プレイヤーカメラ管理クラス

class Camera : public ObjectBase {
private:
	Player* mp_player;		//プレイヤー

	int m_idx;
	int m_state;			//現在の状態

	//状態変化
	enum {
		eState_Idle,		//通常状態
		eState_ClosetIn,	//隠れた状態
	};			
public:
	//通常状態
	void StateIdle();
	//隠れた状態
	void StateClosetIn();
	//コンストラクタ
	Camera(const CVector3D& rot);
	//更新処理
	void Update();
	//描画処理
	void Render();
};