#pragma once

//�Q�[���N���A��ʊǗ��N���X

class GameClear : public Task {
private:
	CImage m_BuckGraund;		//�w�i
	CImage m_TitleText;			//�^�C�g������
	int m_select;				//�Z���N�g�t���O
	float m_High;				//�����\���p����
	void HighLight();			//�����\������
public:
	//�R���X�g���N�^
	GameClear();
	//�f�X�g���N�^
	~GameClear();
	//�X�V����
	void Update();
	//2D�`�揈��
	void Draw();
};
