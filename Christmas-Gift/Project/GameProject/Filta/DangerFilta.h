#pragma once

//�댯�M���Ǘ��N���X

class DangerFilta : public Task
{
private:
	CImage m_filta;			//�t�B���^�[
	float m_alpha;			//�����x
	float m_time;			//����
	bool m_isalphachenge;	//�����x�̉��Z�ƌ��Z��ύX
public:
	//�R���X�g���N�^
	DangerFilta();
	//�X�V����
	void Update();
	//2D�`�揈��
	void Draw();

};
