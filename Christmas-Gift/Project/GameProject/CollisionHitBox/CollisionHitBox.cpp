#include"CollisionHitBox.h"
#include"Filta/Filta.h"
#include"GameScene/GameData.h"

WarpRoomBox::WarpRoomBox(const CVector3D& pos, const CVector3D& rot, const CVector3D& size)
	:ObjectBase(ETaskTag::eWarpRoomBox,true)
{
	mp_filta = dynamic_cast<Filta*>(TaskManager::FindObject(ETaskTag::eFilta));
	m_pos = pos;
	m_rot = rot;
	m_size = size;
	m_obb = COBB(
		m_pos,
		m_rot,
		m_size
	);
	GameData::ChengeStage = false;
}


void WarpRoomBox::Render()
{
	//デバッグ用
	Utility::DrawOBB(m_obb, CVector4D(1, 0, 0, 1));
}

void WarpRoomBox::Collision(Task* t)
{
	switch (t->GetTag())
	{
	case ETaskTag::ePlayer:
		float dist;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(m_obb, t->m_lineS, t->m_lineE, t->m_rad, &axis, &dist))
		{
			//Eボタン入力でプレゼント配置
			if (PUSH(CInput::eButton9))
			{
				GameData::isGift = true;
				printf("クリア可能状態");
			}

			//Eボタン入力でマップ移動
			//if (PUSH(CInput::eButton9))
			//{
			//	//遷移するステージにデータを変更
			//	switch (GameData::Stage)
			//	{
			//	case GameData::A_1:
			//		GameData::Stage = GameData::B_1;
			//		break;
			//	case GameData::B_1:
			//		GameData::Stage = GameData::B_2;
			//		break;
			//	case GameData::B_2:
			//		GameData::Stage = GameData::B_1;
			//		break;
			//	}

			//	
			//	GameData::ChengeStage = true;
			//	mp_filta->m_FadeoutCheck = true;
			//}
			//if (!mp_filta->m_FadeoutCheck && GameData::ChengeStage)
			//{
			//	TaskManager::KillALL();
			//}
		}
	}
}


