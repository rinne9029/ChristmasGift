#pragma once
//#include"GameData.h"

class Player;
class Filta;

class  GameScene :public Task {
private:
	int GameCount;
	Player* mp_player;
	Filta* mp_filta;

	typedef struct
	{
		CVector3D		pos;
		CVector3D		size;
	} S_STATUS;


public:
	//GameData::StageId enumId;
public:
	GameScene();
	~GameScene();
	void Update();
};
