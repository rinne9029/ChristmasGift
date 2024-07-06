#include "GameClear.h"
#include"Title/Title.h"
#include"Ranking/Ranking.h"

//�R���X�g���N�^
GameClear::GameClear()
	:Task(ETaskTag::eResult, true)
	, m_select(0)
	, m_High(0)
{
	//�t�F�[�h�C�����s
	GameData::StartFadeIn = true;
	GameData::GameStart = false;

	//�����L���O�؂�ւ�
	ChengeRanking();

	//�N���A�T�E���h�Đ�
	SOUND("SE_Clearfanfare")->Play();

	m_BuckGraund = COPY_RESOURCE("ClearBackGround", CImage);
	m_TitleText = COPY_RESOURCE("TitleText", CImage);
	m_RankingText = COPY_RESOURCE("RankingText", CImage);
}

//�f�X�g���N�^
GameClear::~GameClear()
{
	switch (m_select)
	{
		//�^�C�g��
	case 0:
		new Title();
		break;
		//�����L���O
	case 1:
		new Ranking("Ranking.txt");
		break;
	}
}

//�n�C���C�g�\������
void GameClear::HighLight()
{
	m_High = m_High + 0.05;
	switch (m_select)
	{
		//�^�C�g����������
	case 0:
		m_TitleText.SetPos(674, 540);
		m_TitleText.SetSize(512 + 40 * cos(m_High), 128 + 10 * cos(m_High));
		m_TitleText.Draw();
		m_RankingText.SetPos(674, 720);
		m_RankingText.Draw();
		break;
		//�����L���O��������
	case 1:
		m_TitleText.SetPos(674, 540);
		m_TitleText.Draw();
		m_RankingText.SetPos(674, 720);
		m_RankingText.SetSize(640 + 50 * cos(m_High), 128 + 10 * cos(m_High));
		m_RankingText.Draw();
	}
}

//�����L���O�X�V����
void GameClear::ChengeRanking()
{
	//�t�@�C���|�C���^
	FILE* fp = NULL;
	//�t�@�C�����e�L�X�g�ǂݍ��݃��[�h�ŊJ��
	fopen_s(&fp, "Ranking.txt", "r");
	//�J���̂Ɏ��s
	if (!fp) return;
	m_line_size = 0;

	char m_text[256] = "";

	//�t�@�C���̖����܂ŌJ��Ԃ�
	while (!feof(fp))
	{
		//��s�ǂݍ���
		fgets(m_text, m_max_size, fp);
		//���s�܂œǂݍ��ނ̂ŉ��s(\n)������
		char* p = strrchr(m_text, '\n');
		if (p) *p = '\0';
		
		//�L�^��z��ɕۑ�
		sscanf_s(m_text, "%d", &m_Rank[m_line_size]);

		m_line_size++;
	}
	//�t�@�C�������
	fclose(fp);

	//�����L���O����ւ�
	//���ʐ����J��Ԃ�
	for (int cnt = 4; cnt > 0; cnt--)
	{
		for (int a = 3; a > 0; a--)
		{
			//���̂ق��������Ȃ�
			if (m_Rank[a] > m_Rank[a - 1])
			{
				int c = m_Rank[a - 1];
				m_Rank[a - 1] = m_Rank[a];
				m_Rank[a] = c;
			}
		}
	}
	
	//�V�����L�^�L��
	//�����N�O�Ȃ�L�^���Ȃ�
	if (m_Rank[3] > GameData::second)
	{
		//�����N�O�Ȃ̂ŋL�^���Ȃ�
	}
	else
	{
	for (int a = 2; a >= 0; a--)
	{
		//�������ׂ�
		if (m_Rank[a] < GameData::second)
		{
			//���̏��ʂɈړ�����
			m_Rank[a + 1] = m_Rank[a];
		}
		else
		{
			//��ׂ����ʂ̉����m�肷��
			m_Rank[a + 1] = GameData::second;
			break;
		}
	}
	if (m_Rank[0] < GameData::second) m_Rank[0] = GameData::second;
	}

	//�t�@�C�����e�L�X�g�������݃��[�h�ŊJ��
	fopen_s(&fp, "Ranking.txt", "w");
	//�J���̂Ɏ��s
	if (!fp)return;
	for (int a = 0; a < 4; a++)
	{
		fprintf(fp, "%d\n", m_Rank[a]);
	}
	fclose(fp);

}

//�X�V����
void GameClear::Update()
{
	//�t�F�[�h�C���E�t�F�[�h�A�E�g���͎��s���Ȃ�
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	//�X�y�[�X�L�[�Ō���
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(0.3);
		SOUND("SE_Click")->Play();
		//�t�F�[�h�A�E�g���s
		GameData::StartFadeOut = true;
	}

	//W�L�[����
	if (PUSH(CInput::eUp) && m_select > 0)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select--;
		m_High = 0;	//�������Z�b�g
	}
	//S�L�[����
	if (PUSH(CInput::eDown) && m_select < 1)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select++;
		m_High = 0;	//�������Z�b�g
	}
}

//2D�`�揈��
void GameClear::Draw()
{
	//�w�i�`��
	m_BuckGraund.Draw();
	//�����\���`��
	HighLight();
	//�^�C���\��
	FONT_T()->Draw(1350, 250, 0, 0, 0, "�c�莞�ԁF%d�b",GameData::second);
}
