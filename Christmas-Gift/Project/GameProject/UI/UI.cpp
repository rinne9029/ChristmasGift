#include"UI.h"
#include"Chara/Player/Player.h"
#include"Filta/Filta.h"
#include"GameScene/GameData.h"

SleepLife::SleepLife()
	:Task(ETaskTag::eUI,true)
	,mp_player(nullptr)
	,mp_filta(nullptr)
{
	mp_player = dynamic_cast<Player*>(TaskManager::FindObject(ETaskTag::ePlayer));
	mp_filta = dynamic_cast<Filta*>(TaskManager::FindObject(ETaskTag::eFilta));
}

SleepLife::~SleepLife()
{
	
}

//更新処理
void SleepLife::Update()
{
	//子供の睡眠が覚醒（ゲームオーバー）
	if (GameData::BlueSleepSize >= 0) return;

	if (mp_player->m_CheckKill && !mp_filta->m_FadeoutCheck)
	{
		GameData::BlueSleepSize = 300;
		TaskManager::KillALL();
	}
	//プレイヤー死亡状態
	mp_player->m_CheckKill = true;
	mp_filta->m_FadeoutCheck = true;
		
	
}

//2D描画処理
void SleepLife::Draw()
{
	//赤の睡眠ゲージ描画
	Utility::DrawQuad(CVector2D(20, 250), CVector2D(300, 25), CVector4D(1, 0, 0, 1));
	//青の睡眠ゲージ描画
	Utility::DrawQuad(CVector2D(20, 250), CVector2D(GameData::BlueSleepSize,25), CVector4D(0, 0, 1, 1));

}