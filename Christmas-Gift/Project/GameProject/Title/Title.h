#pragma once

//�^�C�g����ʊǗ��N���X

class Title : public Task {
private:
	CImage m_BackGroundTitle;	//�^�C�g���w�i
	CImage m_TitleRogo;			//�^�C�g�����e�L�X�g
	CImage m_Manual1;			//�������P
	CImage m_Manual2;			//�������Q
	CImage m_Stage1;			//�X�e�[�W�P
	CImage m_Stage2;			//�X�e�[�W�Q
	CImage m_Stage3;			//�X�e�[�W�R
	int m_select;				//�Z���N�g�t���O
	int m_step;				
	float m_fuwafuwa;			//�ӂ�ӂ�\���p����
	void FuwaFuwa();			//�ӂ�ӂ�\������
	void ModeChenge();			//���[�h�ύX����
	void ManualMode();			//�������[�h����
	void ManualDraw();			//�����`��
	void StageSelecte();		//�X�e�[�W�I������
	void StageDraw();			//�X�e�[�W�I��`��
public:
	//�R���X�g���N�^
	Title();
	//�f�X�g���N�^
	~Title();
	//�X�V����
	void Update();
	//2D�`�揈��
	void Draw();
};