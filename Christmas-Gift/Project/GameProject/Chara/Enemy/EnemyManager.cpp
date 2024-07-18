#include"EnemyManager.h"
#include"Enemy.h"

//�G�̊Ǘ��N���X�̃C���X�^���X
EnemyManager* EnemyManager::ms_instance = nullptr;
//�G�̒T���ʒu�m�[�h�̃��X�g
std::list<SearchNode> EnemyManager::ms_searchNodes =
{
	CVector3D(-0.969914f,1.0f,16.858345f),			//���֑O
	CVector3D(-10.756325f,10.053700f,13.386012f),	//2�K�g�C���O
	CVector3D(2.369029f,0.960001f,-0.000122f),		//�\�t�@�[�O
	CVector3D(0.943921f,10.053699f,1.877687f),		//���e����
	CVector3D(-10.848549f,1.0f,3.523031f),			//���ʏ�
	
};

//�R���X�g���N�^
EnemyManager::EnemyManager()
	:CharaBase(ETaskTag::eEnemyManager,true)
{
	m_remove = true;
}

//�f�X�g���N�^
EnemyManager::~EnemyManager()
{
	ms_instance = nullptr;
}

//�C���X�^���X���擾
EnemyManager* EnemyManager::Instance()
{
	//�C���X�^���X���Ȃ����
	if (ms_instance == nullptr)
	{
		//�C���X�^���X�𐶐�����
		ms_instance = new EnemyManager();
	}
	return ms_instance;
}

//�G�����X�g�ɒǉ�
void EnemyManager::Add(Enemy* enemy)
{
	m_enemies.push_back(enemy);
}

//�G�����X�g�����菜��
void EnemyManager::Remove(Enemy* enemy)
{
	m_enemies.remove(enemy);
}

//���ɒT������m�[�h���擾
SearchNode* EnemyManager::GetNextSearchNode() const
{
	//���T���m�[�h�̑I������
	//�@���̓G���T�����̃m�[�h�͑I�����Ȃ�
	//�A���݊m������ԍ����m�[�h��I������
	
	SearchNode* ret = nullptr;
	//���X�g�̐擪���珇�Ԃɒ��ׂāA
	//��ԏ����ɋ߂���v����m�[�h���擾����
	for (SearchNode& node : ms_searchNodes)
	{
		//���ɑ��̓G���T�����̃m�[�h�͑ΏۊO
		if (node.enemy != nullptr) continue;
		//�߂�l�̃m�[�h���܂��ݒ肳��Ă��Ȃ��������́A
		//���݊m�����������̃m�[�h�̖@�������̂ł���΁A
		//�������̃m�[�h��D�悷��
		if (ret == nullptr || node.prob > ret->prob)
		{
			ret = &node;
		}
	}
	return ret;
}

//�X�V
void EnemyManager::Update()
{
	for (SearchNode& node : ms_searchNodes)
	{
		node.prob = Utility::Clamp01(node.prob + 0.0005f);
	}
}

//�`��
void EnemyManager::Render()
{
	if (!g_isRenderDebug) return;

	for (SearchNode& node : ms_searchNodes)
	{
		float per = node.prob;
		CVector3D color;
		if (per > 0.5f)
		{
			color = CVector3D::Leap
			(
				CVector3D(1.0f, 1.0f, 0.0f),
				CVector3D(1.0f, 0.0f, 0.0f),
				(per - 0.5f) / 0.5f
			);
		}
		else
		{
			color = CVector3D::Leap
			(
				CVector3D(0.0f, 1.0f, 0.0f),
				CVector3D(1.0f, 1.0f, 0.0f),
				per / 0.5f
			);
		}
		Utility::DrawCube(node.pos, 2.0f, CVector4D(color, 1.0f));
	}
}