#pragma once
#include"EffectBase.h"

//�ڕW�n�_�Ǘ��N���X

class Destination : public EffectBase
{
private:
	CModelObj m_model;
public:
	//�R���X�g���N�^
	Destination(CModelObj model,const CVector3D& pos,const CVector3D& scale);
	//�X�V����
	void Update();
	//�`�揈��
	void Render();
};