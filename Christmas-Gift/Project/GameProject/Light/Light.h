#pragma once
#include"ObjectBase/ObjectBase.h"

//ライト管理クラス

class Light : public ObjectBase
{
private:
	int m_no;			//電気ごとの番号
	float m_range;		//光の強さ
	int m_type;			//光の種類
public:
	int m_roomNo;		//電気のついている部屋番号
	bool m_islight;		//電気のOn,Off
	Light::Light(CVector3D& pos,int no,int roomno,float range, int light,bool islight);
	Light::~Light();
	void Update();
};

