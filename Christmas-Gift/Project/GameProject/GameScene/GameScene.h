#pragma once

//�Q�[���V�[���Ǘ��N���X

class DangerFilta;

class  GameScene : public Task {
private:
	int m_stage;				//���݂̃X�e�[�W
	DangerFilta* m_dangerfilta;
public:
	//�R���X�g���N�^
	GameScene(int stage);
	//�f�X�g���N�^
	~GameScene();
	//�X�V����
	void Update();
};
