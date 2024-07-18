#pragma once

//ゲームシーン管理クラス

class DangerFilta;

class  GameScene : public Task {
private:
	int m_stage;				//現在のステージ
	DangerFilta* m_dangerfilta;
public:
	//コンストラクタ
	GameScene(int stage);
	//デストラクタ
	~GameScene();
	//更新処理
	void Update();
};
