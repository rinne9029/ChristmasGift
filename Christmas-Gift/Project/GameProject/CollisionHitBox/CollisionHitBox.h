#pragma once
#include"ObjectBase/ObjectBase.h"
//部屋移動用の画面に見えない当たり判定

class Filta;

class WarpRoomBox : public ObjectBase {
private:
	Filta* mp_filta;
	CVector3D m_size;		
public:
	//コンストラクタ
	WarpRoomBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);

	//描画処理
	void Render();

	//衝突処理
	void Collision(Task* t);
};

class WarpChildRoomBox : public ObjectBase {
private:
	CVector3D m_size;		//大きさ
public:
	//コンストラクタ
	WarpChildRoomBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size);

	//描画処理
	void Render();

	//衝突処理
	void Collision(Task* t);

};


