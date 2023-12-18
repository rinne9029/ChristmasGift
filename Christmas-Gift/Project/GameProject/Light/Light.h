#pragma once
#include"ObjectBase/ObjectBase.h"

//部屋番号
enum class ERoomNo
{
	eKitchen_Dining_Room,
	eLiving_Room
};

class Light : public ObjectBase
{
private:
	int m_no;			//電気ごとの番号
public:
	ERoomNo m_roomNo;		//電気のついている部屋番号
	bool m_lightOn;		//電気のOn,Off
	Light::Light(CVector3D& pos,int no,ERoomNo roomno);
	Light::~Light();
	void Update();
};

