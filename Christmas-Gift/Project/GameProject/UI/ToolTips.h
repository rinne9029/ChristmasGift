#pragma once

//�c�[���`�b�v�Ǘ��N���X

class ToolTips : public Task
{
private:
	CImage m_Image;
	
public:
	char* m_Text;
	bool m_Iskill;
	//�R���X�g���N�^
	ToolTips();
	//2D�`�揈��
	void Draw();
};