#pragma once

//�G�t�F�N�g�̃x�[�X�N���X

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
	//�R���X�g���N�^
	EffectBase(int Effect_Mode);
	//UV�G�t�F�N�g
	void DestinationEffect();
	//�X�V����
	void Update();
};