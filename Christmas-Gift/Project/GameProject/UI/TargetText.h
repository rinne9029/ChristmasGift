#pragma once

//�Q�[�����̖ڕW�e�L�X�g�Ǘ��N���X

class TargetText : public Task {
private:
	char* m_Text;
public:
	//�R���X�g���N�^
	TargetText();
	//�X�V����
	void Update();
	//2D�`�揈��
	void Draw();
};