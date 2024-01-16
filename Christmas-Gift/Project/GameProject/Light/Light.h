#pragma once
#include"ObjectBase/ObjectBase.h"

class Light : public ObjectBase
{
private:
	int m_no;			//電気ごとの番号
public:
	int m_roomNo;		//電気のついている部屋番号
	bool m_islight;		//電気のOn,Off
	Light::Light(CVector3D& pos,int no,int roomno,bool islight);
	Light::~Light();
	void Update();
};

