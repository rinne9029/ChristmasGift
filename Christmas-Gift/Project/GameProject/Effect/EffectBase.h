#pragma once

//エフェクトのベースクラス

class EffectBase : public Task
{
private:
	int m_Effect_Mode;

public:
	float m_alpha;

	enum
	{
		eDefault,
		eDestinationEffect_Effect
	};
	//コンストラクタ
	EffectBase(int Effect_Mode);
	//UVエフェクト
	void DestinationEffect();
	//更新処理
	void Update();
};