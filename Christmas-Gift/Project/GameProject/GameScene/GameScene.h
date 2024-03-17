#pragma once

//ゲームシーン管理クラス

class  GameScene :public Task {
private:
public:
	//コンストラクタ
	GameScene();
	//デストラクタ
	~GameScene();
	//更新処理
	void Update();
};
