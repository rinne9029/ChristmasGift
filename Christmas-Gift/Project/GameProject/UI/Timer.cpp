#include "Timer.h"
#include"GameScene/GameData.h"

Timer::Timer()
	:Task(ETaskTag::eUI, true)
{
	m_Image = COPY_RESOURCE("Clock", CImage);
	count = 0;
	a = 0;
}

Timer::~Timer()
{
}

void Timer::Update()
{
}

void Timer::Draw()
{
	bool i;
	count++;
	//1•bŒo‰ß‚ÅƒJƒEƒ“ƒg‚ğ‘‰Á
	if (count > 60)
	{
		GameData::second++;	//‚P•b‰ÁZ
		count = 0;
		i = true;
	}
	else
	{
		i = false;
	}
	m_Image.SetPos(1620, 0);
	m_Image.SetSize(300, 300);
	m_Image.Draw();

	//1•ªŒo‰ß
	if (GameData::second % 60 == 0 && GameData::second != 0 && i == true)
	{
		a++;
	}

	//5`6
	Utility::DrawLine(CVector2D(1770, 150), CVector2D(1810 - 8 * a, 220 + 2*a), CVector4D(1, 0, 0, 1));


}
