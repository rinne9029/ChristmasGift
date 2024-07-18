#include "DangerFilta.h"

#define TIME 1.0f		//ŽžŠÔ
#define MAX_ALPHA 0.5f	//“§–¾“x

DangerFilta::DangerFilta()
	:Task(ETaskTag::eFilta, true)
	,m_alpha(0.0f)
	,m_time(0.0f)
{
	m_filta = COPY_RESOURCE("DangerFilta", CImage);
}

void DangerFilta::Update()
{
	if(m_isalphachenge) m_time += CFPS::GetDeltaTime();
	else				m_time -= CFPS::GetDeltaTime();

	//5•b‚²‚Æ‚É‰ÁŽZ‚ÆŒ¸ŽZ‚ðØ‚è‘Ö‚¦
	if (m_time > TIME && m_isalphachenge)	m_isalphachenge = false;
	else if (m_time < 0 && !m_isalphachenge) m_isalphachenge = true;
	
	m_alpha = MAX_ALPHA * (m_time / TIME);
}

void DangerFilta::Draw()
{
	m_filta.SetColor(1.0f, 1.0f, 1.0f, m_alpha);
	m_filta.Draw();
}
