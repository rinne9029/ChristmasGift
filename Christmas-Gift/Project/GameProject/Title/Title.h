#pragma once

//�^�C�g����ʊǗ��N���X

class Title : public Task {
private:
	enum TitleScene
	{
		eState_Title,
		eState_Stage,
		eState_Menu,
		eState_Collection
	};
	int m_state;					//���݂̃X�e�[�g
	int m_select;					//�Z���N�g�i���o�[
	float m_alpha;					//�����x
	float m_elapsedtimeY;			//Y���W�𓮂����o�ߎ���	
	float m_imageposY;				//�摜��Y���W
	float m_volume;					//����

	CImage m_backgroundtitleimage;	//�^�C�g���w�i
	CImage m_titlerogoimage;		//�^�C�g�����e�L�X�g
	CImage m_stageimage1;			//�X�e�[�W�P
	CImage m_stageimage2;			//�X�e�[�W�Q
	CImage m_collectionimage1;		//�R���N�V�����摜1
	CImage m_collectionimage2;		//�R���N�V�����摜2
	CImage m_collectionimage3;		//�R���N�V�����摜3

	void TitleUpdate();			//�^�C�g����ʂ̍X�V����
	void TitleDraw();			//�^�C�g����ʂ̕`�揈��
	void StageSelectUpdate();	//�X�e�[�W�I������
	void StageSelectDraw();		//�X�e�[�W�I��`��
	void TitleMenuUpdate();		//�^�C�g�����j���[��ʂ̍X�V����
	void TitleMenuDraw();		//�^�C�g�����j���[��ʂ̕`�揈��
	void CollectionUpdate();	//�R���N�V������ʂ̍X�V����
	void CollectionDraw();		//�R���N�V������ʂ̕`�揈��
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