#include"Title.h"
#include"Filta/Filta.h"
#include"GameScene/GameScene.h"
#include"GameScene/GameData.h"

//コンストラクタ
Title::Title()
	:Task(ETaskTag::eScene,true)
	,mp_filta(nullptr)
	,m_select(0)
	,m_fuwafuwa(0.0f)
{
	m_StartText = COPY_RESOURCE("StartText", CImage);
	m_ManualText = COPY_RESOURCE("ManualText", CImage);
	m_RankingText = COPY_RESOURCE("RankingText", CImage);

	GameData::Start = false;
	mp_filta = dynamic_cast<Filta*>(TaskManager::FindObject(ETaskTag::eFilta));
	mp_filta->m_FadeinCheck = true;
}

//デストラクタ
Title::~Title()
{
	//ゲームシーン起動
	new GameScene();	
}

//ふわふわ動く文字
void Title::FuwaFuwa()
{
	m_fuwafuwa += 0.02f;

	//選ばれた文字がsinカーブでふわふわ動く
	switch (m_select)
	{
	case 0:
		m_StartText.SetPos(200, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_StartText.Draw();
		break;
	case 1:
		m_ManualText.SetPos(855, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_ManualText.Draw();
		break;
	case 2:
		m_RankingText.SetPos(1270, 750 - abs(sin(m_fuwafuwa)) * 64);
		m_RankingText.Draw();
		break;
	}
}

//セレクトされていない文字を表示
void Title::Erabu()
{
	switch (m_select)
	{
	case 0:
		m_ManualText.SetPos(855, 750);
		m_ManualText.Draw();
		m_RankingText.SetPos(1270, 750);
		m_RankingText.Draw();
		break;
	case 1:
		m_StartText.SetPos(200, 750);
		m_StartText.Draw();
		m_RankingText.SetPos(1270, 750);
		m_RankingText.Draw();
		break;
	case 2:
		m_StartText.SetPos(200, 750);
		m_StartText.Draw();
		m_ManualText.SetPos(855, 750);
		m_ManualText.Draw();
		break;
	}
	

}

//更新処理
void Title::Update()
{
	if (PUSH(CInput::eButton5))		//zボタン
	{
		switch (m_select)
		{
		case 0:
			mp_filta->m_FadeoutCheck = true;
			GameData::Start = true;
			break;
		case 1:
			break;
		case 2:
			break;
		}
	}

	if (!mp_filta->m_FadeoutCheck && GameData::Start)
	{
		TaskManager::KillALL();
	}
	
}

//3D描画処理
void Title::Render()
{
}

//2D描画処理
void Title::Draw()
{
	if (PUSH(CInput::eLeft) && m_select > 0)
	{
		m_select--;
		m_fuwafuwa = 0;
	}
	if (PUSH(CInput::eRight) && m_select < 2)
	{
		m_select++;
		m_fuwafuwa = 0;
	}
	Erabu();
	FuwaFuwa();	
}