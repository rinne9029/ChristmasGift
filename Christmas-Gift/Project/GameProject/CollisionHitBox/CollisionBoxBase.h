#pragma once

//�����蔻��x�[�X

class ToolTips;

class CollisionBoxBase : public Task
{
public:
	ToolTips* m_tooltips;
	float m_time;
	//�R���X�g���N�^
	CollisionBoxBase(ETaskTag tag,bool remove);

	virtual ~CollisionBoxBase();
	//�`�揈��
	void Render();
};