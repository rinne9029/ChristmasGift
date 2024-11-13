#include "GameClear.h"
#include"Title/Title.h"

//コンストラクタ
GameClear::GameClear()
	:Task(ETaskTag::eResult, true)
	, m_select(0)
	, m_High(0.0f)
{
	//フェードイン実行
	GameData::StartFadeIn = true;
	GameData::GameStart = false;

	//クリアサウンド再生
	SOUND("SE_Clearfanfare")->Play();

	m_BuckGraund = COPY_RESOURCE("ClearBackGround", CImage);
	m_TitleText = COPY_RESOURCE("TitleText", CImage);
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
	}
}

//ハイライト表示処理
void GameClear::HighLight()
{
	m_High = m_High + 0.05f;
	m_TitleText.SetPos(674.0f, 540.0f);
	m_TitleText.SetSize(512.0f + 40.0f * cos(m_High), 128.0f + 10.0f * cos(m_High));
	m_TitleText.Draw();
}

//更新処理
void GameClear::Update()
{
	//フェードイン・フェードアウト中は実行しない
	if (GameData::StartFadeIn) return;
	if (GameData::StartFadeOut) return;

	//スペースキーで決定
	if (PUSH(CInput::eButton1))
	{
		SOUND("SE_Click")->Volume(GameData::volume);
		SOUND("SE_Click")->Play();
		//フェードアウト実行
		GameData::StartFadeOut = true;
	}
}

//2D描画処理
void GameClear::Draw()
{
	//背景描画
	m_BuckGraund.Draw();
	//強調表示描画
	//HighLight();
}
