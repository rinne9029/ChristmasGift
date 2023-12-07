#include"Filta.h"
#include"Chara/Player/Player.h"

#define FADETIME 90.0

Filta::Filta()
	:ObjectBase(ETaskTag::eFilta,false)
	, mp_plyaer(nullptr)
	, m_time(0)
	, m_transparent(0)
	, m_FadeinCheck(false)
	, m_FadeoutCheck(false)
{
	m_filta = COPY_RESOURCE("Filta", CImage);
	//m_remove = false;
}

Filta::~Filta()
{

}

void Filta::Fadein()
{
	m_time -= 3.0f;
	if (m_time >= 0)
	{
		m_transparent = m_time / FADETIME;
	}
	else
	{
		m_FadeinCheck = false;
	}
}

void Filta::Fadeout()
{
	m_time += 3.0f;
	if (m_time <= 90)
	{
		m_transparent = m_time / FADETIME;
	}
	else
	{
		m_FadeoutCheck = false;
	}
}

void Filta::Update()
{
	if (!mp_plyaer)
	{
		mp_plyaer = dynamic_cast<Player*>(TaskManager::FindObject(ETaskTag::ePlayer));
	}

	if (m_FadeinCheck) 
	{
		Fadein();
	}
		

	if (m_FadeoutCheck) 
	{
		Fadeout();
	}
		
}

void Filta::Render()
{
	m_filta.SetColor(1, 1, 1, m_transparent);
	m_filta.Draw();
}