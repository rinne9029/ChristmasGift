#include"FieldFloor.h"
#include"../ObjectBase/ObjectBase.h"
#include"GameScene/GameData.h"

FieldFloor::FieldFloor()
	:ObjectBase(ETaskTag::eFieldFloor,true)
{
	switch (GameData::Stage)
	{
	case GameData::A_1:
		m_model = COPY_RESOURCE("Map", CModelObj);
		break;
	case GameData::B_1:
		m_model = COPY_RESOURCE("Map", CModelObj);
		break;
	case GameData::B_2:
		m_model = COPY_RESOURCE("Map", CModelObj);
		break;
	}
	
}

void FieldFloor::Update()
{

}

void FieldFloor::Render()
{
	m_model.Render();
}