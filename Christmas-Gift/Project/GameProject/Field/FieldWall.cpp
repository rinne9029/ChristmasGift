#include"FieldWall.h"
#include"../ObjectBase/ObjectBase.h"
#include"GameScene/GameData.h"

FieldWall::FieldWall()
	:ObjectBase(ETaskTag::eFieldWall,true)
{
	m_model = COPY_RESOURCE("WallMap", CModelObj);
}

void FieldWall::Update()
{

}

void FieldWall::Render()
{
	m_model.Render();
}