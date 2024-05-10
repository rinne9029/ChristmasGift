#include"FieldFloor.h"
#include"../ObjectBase/ObjectBase.h"
#include"GameScene/GameData.h"

FieldFloor::FieldFloor()
	:ObjectBase(ETaskTag::eFieldFloor,true)
{
	m_model = COPY_RESOURCE("Map", CModelObj);
}

void FieldFloor::Update()
{

}

void FieldFloor::Render()
{
	m_model.Render();
}