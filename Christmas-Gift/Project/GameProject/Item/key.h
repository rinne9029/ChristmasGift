#pragma once

//�q�������̌��Ǘ��N���X

class key : public Task {
private:
	CModelObj m_model;
public:
	//�R���X�g���N�^
	key(const CVector3D& pos,const CVector3D& rot, const CVector3D& scale);

	//�`�揈��
	void Render();
};