#include "Ranking.h"
#include"Title/Title.h"

Ranking::Ranking(const char* file)
	:Task(ETaskTag::eScene,true)
{
	//�t�F�[�h�C�����s
	GameData::StartFadeIn = true;

	m_TitleText = COPY_RESOURCE("TitleText", CImage);

	//�t�@�C���|�C���^
	FILE* fp = NULL;
	//�t�@�C�����e�L�X�g�ǂݍ��݃��[�h�ŊJ��
	fopen_s(&fp, file, "r");
	//�J���̂Ɏ��s
	if (!fp)return;
	m_line_size = 0;
	//�t�@�C���̖����܂ŌJ��Ԃ�
	while (!feof(fp))
	{
		//��s�ǂݍ���
		fgets(m_text[m_line_size], m_max_size, fp);
		//���s�܂œǂݍ��ނ̂ŉ��s(\n)������
		char* p = strrchr(m_text[m_line_size], '\n');
		if (p) *p = '\0';

		m_line_size++;
	}
	fclose(fp);
}

Ranking::~Ranking()
{
	new Title();
}

//�ӂ�ӂ퓮������
void Ranking::FuwaFuwa()
{
	m_fuwafuwa += 0.02f;
	m_TitleText.SetPos(704.0f, 750.0f - abs(sin(m_fuwafuwa)) * 64.0f);
	m_TitleText.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_TitleText.Draw();
}

void Ranking::Update()
{
	//�t�F�[�h�C���E�t�F�[�h�A�E�g���͎��s���Ȃ�
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	//�X�y�[�X�{�^������
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(0.3f);
		SOUND("SE_Click")->Play();
		GameData::StartFadeOut = true;
	}
}

void Ranking::Draw()
{
	FuwaFuwa();
	for (int cnt = 1; cnt <5 ; cnt++)
	{
		FONT_T()->Draw(300, 300 + 64 * cnt, 0.0f, 0.0f, 0.0f, "%d��",cnt);
		FONT_T()->Draw(450, 300 + 64 * cnt, 0.0f, 0.0f, 0.0f,m_text[cnt-1]);
	}
	
}
