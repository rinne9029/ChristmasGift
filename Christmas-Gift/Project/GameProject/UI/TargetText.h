#pragma once

//ゲーム中の目標テキスト管理クラス

class TargetText : public Task {
private:
	char* m_Text;
public:
	//コンストラクタ
	TargetText();
	//更新処理
	void Update();
	//2D描画処理
	void Draw();
};