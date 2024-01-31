#pragma once

class  GameScene :public Task {
private:

	typedef struct
	{
		CVector3D		pos;
		CVector3D		size;
	} S_STATUS;
public:
	//コンストラクタ
	GameScene();
	//デストラクタ
	~GameScene();
	//更新処理
	void Update();
};
