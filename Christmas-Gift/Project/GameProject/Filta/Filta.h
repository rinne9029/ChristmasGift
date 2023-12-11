#pragma once
#include"ObjectBase/ObjectBase.h"

class Player;

class Filta : public ObjectBase
{
private:
	Player* mp_plyaer;
	CImage m_filta;
	float m_time;
	float m_transparent;
	void Fadein();
	void Fadeout();
public:
	bool m_FadeinCheck;
	bool m_FadeoutCheck;
	Filta();
	~Filta();
	void Update();
	void Draw();
};