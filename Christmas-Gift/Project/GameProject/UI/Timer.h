#pragma once

//�������ԊǗ��N���X

class Timer : public Task
{
private:
	CImage m_Image;
	int m_count;
public:
	//�R���X�g���N�^
	Timer();
	//�X�V����
	void Update();
	//2D�`�揈��
	void Draw();
};