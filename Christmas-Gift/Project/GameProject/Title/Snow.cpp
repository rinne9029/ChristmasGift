#include "Snow.h"

Snow::Snow()
	:Task(ETaskTag::eScene,true)
{
	m_snow = COPY_RESOURCE("Snow", CImage);
	int wide = rand() % 1920;
	m_snowPos = CVector2D(wide, -20);
	m_snowAng = 0;
}

Snow::~Snow()
{
}

void Snow::Update()
{
	m_snowAng += 0.1f;
	m_snowPos.y += 2;

	if (m_snowPos.y > 1200) Kill();
}

void Snow::Draw()
{
	m_snow.SetSize(64, 64);
	m_snow.SetCenter(32, 32);
	m_snow.SetPos(m_snowPos);
	m_snow.SetAng(m_snowAng);
	m_snow.Draw();
}
