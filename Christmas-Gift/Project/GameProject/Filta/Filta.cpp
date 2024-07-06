#include"Filta.h"
#include"Chara/Player/Player.h"

#define FADETIME 1	//フェードイン、フェードアウト完了時間

//コンストラクタ
Filta::Filta()
	:Task(ETaskTag::eFilta,false)
	, m_alpha(1.0)
	, m_time(1)
{
	m_filta = COPY_RESOURCE("Filta", CImage);
}

//フェードイン処理
void Filta::Fadein()
{
	m_time -= CFPS::GetDeltaTime();
	//フェードイン中
	if (m_alpha > 0)
	{
		m_alpha = m_time / FADETIME;
	}
	//フェードイン完了
	else
	{
		//フェードインスタートフラグリセット
		GameData::StartFadeIn = false;
	}
}

//フェードアウト処理
void Filta::Fadeout()
{
	m_time += CFPS::GetDeltaTime();
	//フェードアウト中
	if (m_alpha < 1)
	{
		m_alpha = m_time / FADETIME;
	}
	//フェードアウト完了
	else
	{
		//フェードアウトスタートフラグリセット
		GameData::StartFadeOut = false;
		//フィルター以外の全タスク削除
		TaskManager::KillALL();
	}
}

//更新処理
void Filta::Update()
{
	//フェードインスタートOn
	if (GameData::StartFadeIn) Fadein();
	//フェードアウトスタートOn
	if (GameData::StartFadeOut) Fadeout();
}

//2D描画処理
void Filta::Draw()
{
	//フィルター描画
	m_filta.SetColor(1, 1, 1, m_alpha);
	m_filta.Draw();
}