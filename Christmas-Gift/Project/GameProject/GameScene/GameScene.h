#pragma once

//ゲームシーン管理クラス

class  GameScene :public Task {
private:
	int m_stage;		//現在のステージ
public:
	//コンストラクタ
	GameScene(int stage);
	//デストラクタ
	~GameScene();
	//更新処理
	void Update();
};
