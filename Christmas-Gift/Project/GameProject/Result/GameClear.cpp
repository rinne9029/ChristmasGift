#include "GameClear.h"
#include"Title/Title.h"
#include"Ranking/Ranking.h"
#include"GameScene/GameData.h"

//コンストラクタ
GameClear::GameClear()
	:Task(ETaskTag::eResult, true)
	, m_select(0)
	, m_High(0)
{
	//フェードイン実行
	GameData::StartFadeIn = true;
	GameData::GameStart = false;

	//ランキング切り替え
	ChengeRanking();

	//クリアサウンド再生
	SOUND("SE_Clearfanfare")->Play();

	m_BuckGraund = COPY_RESOURCE("ClearBackGround", CImage);
	m_TitleText = COPY_RESOURCE("TitleText", CImage);
	m_RankingText = COPY_RESOURCE("RankingText", CImage);
}

//デストラクタ
GameClear::~GameClear()
{
	switch (m_select)
	{
		//タイトル
	case 0:
		new Title();
		break;
		//ランキング
	case 1:
		new Ranking("Ranking.txt");
		break;
	}
}

//ハイライト表示処理
void GameClear::HighLight()
{
	m_High = m_High + 0.05;
	switch (m_select)
	{
		//タイトル文字強調
	case 0:
		m_TitleText.SetPos(674, 540);
		m_TitleText.SetSize(512 + 40 * cos(m_High), 128 + 10 * cos(m_High));
		m_TitleText.Draw();
		m_RankingText.SetPos(674, 720);
		m_RankingText.Draw();
		break;
		//ランキング文字強調
	case 1:
		m_TitleText.SetPos(674, 540);
		m_TitleText.Draw();
		m_RankingText.SetPos(674, 720);
		m_RankingText.SetSize(640 + 50 * cos(m_High), 128 + 10 * cos(m_High));
		m_RankingText.Draw();
	}
}

//ランキング更新処理
void GameClear::ChengeRanking()
{
	//ファイルポインタ
	FILE* fp = NULL;
	//ファイルをテキスト読み込みモードで開く
	fopen_s(&fp, "Ranking.txt", "r");
	//開くのに失敗
	if (!fp) return;
	m_line_size = 0;

	char m_text[256] = "";

	//ファイルの末尾まで繰り返す
	while (!feof(fp))
	{
		//一行読み込む
		fgets(m_text, m_max_size, fp);
		//改行まで読み込むので改行(\n)を消す
		char* p = strrchr(m_text, '\n');
		if (p) *p = '\0';
		
		//記録を配列に保存
		sscanf_s(m_text, "%d", &m_Rank[m_line_size]);

		m_line_size++;
	}
	//ファイルを閉じる
	fclose(fp);

	//ランキング入れ替え
	//順位数分繰り返す
	for (int cnt = 4; cnt > 0; cnt--)
	{
		for (int a = 3; a > 0; a--)
		{
			//下のほうが高いなら
			if (m_Rank[a] > m_Rank[a - 1])
			{
				int c = m_Rank[a - 1];
				m_Rank[a - 1] = m_Rank[a];
				m_Rank[a] = c;
			}
		}
	}
	
	//新しい記録記入
	//ランク外なら記録しない
	if (m_Rank[3] > GameData::second)
	{
		//ランク外なので記録しない
	}
	else
	{
	for (int a = 2; a >= 0; a--)
	{
		//下から比べる
		if (m_Rank[a] < GameData::second)
		{
			//下の順位に移動する
			m_Rank[a + 1] = m_Rank[a];
		}
		else
		{
			//比べた順位の下が確定する
			m_Rank[a + 1] = GameData::second;
			break;
		}
	}
	if (m_Rank[0] < GameData::second) m_Rank[0] = GameData::second;
	}

	//ファイルをテキスト書き込みモードで開く
	fopen_s(&fp, "Ranking.txt", "w");
	//開くのに失敗
	if (!fp)return;
	for (int a = 0; a < 4; a++)
	{
		fprintf(fp, "%d\n", m_Rank[a]);
	}
	fclose(fp);

}

//更新処理
void GameClear::Update()
{
	//フェードイン・フェードアウト中は実行しない
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	//スペースキーで決定
	if (PUSH(CInput::eButton5))
	{
		SOUND("SE_Click")->Volume(0.3);
		SOUND("SE_Click")->Play();
		//フェードアウト実行
		GameData::StartFadeOut = true;
	}

	//Wキー入力
	if (PUSH(CInput::eUp) && m_select > 0)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select--;
		m_High = 0;	//強調リセット
	}
	//Sキー入力
	if (PUSH(CInput::eDown) && m_select < 1)
	{
		SOUND("SE_Select")->Volume(0.3);
		SOUND("SE_Select")->Play();
		m_select++;
		m_High = 0;	//強調リセット
	}
}

//2D描画処理
void GameClear::Draw()
{
	//背景描画
	m_BuckGraund.Draw();
	//強調表示描画
	HighLight();
	//タイム表示
	FONT_T()->Draw(1350, 250, 0, 0, 0, "残り時間：%d秒",GameData::second);
}
