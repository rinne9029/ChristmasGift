#pragma once

class Pause : public Task {
private:
	int select;
	int step;
	CImage m_Manual;	//������@���
	void PauseMenu1();	//�|�[�Y��ʑI��
	void PauseMenu2();	//������@
public:
	//�R���X�g���N�^
	Pause();
	//2D�`�揈��
	void Draw();
};
