#include"SleepLife.h"
#include"Chara/Player/Player.h"
#include"Filta/Filta.h"
#include"GameScene/GameData.h"

bool SleepLife::m_REM = true;

SleepLife::SleepLife()
	:Task(ETaskTag::eUI, true)
	, mp_player(nullptr)
	, mp_filta(nullptr)
{
	mp_player = dynamic_cast<Player*>(TaskManager::FindObject(ETaskTag::ePlayer));
	mp_filta = dynamic_cast<Filta*>(TaskManager::FindObject(ETaskTag::eFilta));
	m_ImageREM = COPY_RESOURCE("REMsleep", CImage);
	m_ImageNREM = COPY_RESOURCE("NREMsleep", CImage);
	m_ChengeSleep = rand() % 100;
}

SleepLife::~SleepLife()
{

}

//更新処理
void SleepLife::Update()
{
	//子供の睡眠が覚醒（ゲームオーバー）
	if (GameData::BlueSleepSize >= 0) return;

	//ゲームオーバー
	GameData::GameOverCheck = true;
	TaskManager::KillALL();
}

//2D描画処理
void SleepLife::Draw()
{
	//赤の睡眠ゲージ描画
	Utility::DrawQuad(CVector2D(20, 250), CVector2D(300, 25), CVector4D(1, 0, 0, 1));
	//青の睡眠ゲージ描画
	Utility::DrawQuad(CVector2D(20, 250), CVector2D(GameData::BlueSleepSize, 25), CVector4D(0, 0, 1, 1));

	m_ImageREM.SetPos(106, 216);
	m_ImageREM.SetSize(128, 32);

	m_ImageNREM.SetPos(42, 216);
	m_ImageNREM.SetSize(256, 32);

	if (GameData::second >= m_ChengeSleep)
	{
		m_REM = !m_REM;
		m_ChengeSleep = m_ChengeSleep + rand() % 100;
	}

	//レム睡眠状態なら
	if (m_REM)	m_ImageREM.Draw();
	//そうじゃないなら
	else		m_ImageNREM.Draw();
	
	
	

	
	
}