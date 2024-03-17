#include "Ranking.h"
#include"Title/Title.h"
#include"GameScene/GameData.h"

Ranking::Ranking(const char* file)
	:Task(ETaskTag::eScene,true)
{
	//フェードイン実行
	GameData::StartFadeIn = true;

	m_TitleText = COPY_RESOURCE("TitleText", CImage);

	//ファイルポインタ
	FILE* fp = NULL;
	//ファイルをテキスト読み込みモードで開く
	fopen_s(&fp, file, "r");
	//開くのに失敗
	if (!fp)return;
	m_line_size = 0;
	//ファイルの末尾まで繰り返す
	while (!feof(fp))
	{
		//一行読み込む
		fgets(m_text[m_line_size], m_max_size, fp);
		//改行まで読み込むので改行(\n)を消す
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

//ふわふわ動く文字
void Ranking::FuwaFuwa()
{
	m_fuwafuwa += 0.02f;
	m_TitleText.SetPos(704, 750 - abs(sin(m_fuwafuwa)) * 64);
	m_TitleText.SetColor(0, 0, 0, 1);
	m_TitleText.Draw();
}

void Ranking::Update()
{
	//フェードイン・フェードアウト中は実行しない
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	//スペースボタン入力
	if (PUSH(CInput::eButton5))
	{
		SOUND("SE_Click")->Volume(0.3);
		SOUND("SE_Click")->Play();
		GameData::StartFadeOut = true;
	}
}

void Ranking::Draw()
{
	FuwaFuwa();
	for (int cnt = 1; cnt <5 ; cnt++)
	{
		FONT_T()->Draw(300, 300 + 64 * cnt, 0, 0, 0, "%d位",cnt);
		FONT_T()->Draw(450, 300 + 64 * cnt, 0, 0, 0,m_text[cnt-1]);
	}
	
}
