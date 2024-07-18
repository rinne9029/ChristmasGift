#include "Timer.h"

//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Timer::Timer()
	:Task(ETaskTag::eUI, true)
{
	m_Clock = COPY_RESOURCE("Clock", CImage);
	m_ClockHands = COPY_RESOURCE("ClockHands", CImage);
	m_count = 0;
}

void Timer::Update()
{
		GameData::second -= CFPS::GetDeltaTime();	//‚P•b‰ÁŽZ
		if (GameData::second < 0.0f)
		{
			GameData::second = 0.0f;
		}
}

//2D•`‰æˆ—
void Timer::Draw()
{
	CVector2D size = CVector2D(256.0f, 256.0f);
	CVector2D pos = CVector2D(1700.0f, size.y * 0.5f);
	m_Clock.SetSize(size);
	m_Clock.SetCenter(size * 0.5f);
	m_Clock.SetPos(pos);
	m_Clock.Draw();

	float timeRatio = 1.0f - GameData::second / GAME_TIME;
	CVector4D color = CVector4D(1.0f, 0.0f, 0.0f,0.5f);
	glBlendFunc(GL_ONE, GL_ONE);
	Utility::DrawCircle(pos, size.x * 0.3f, 360.0f * timeRatio, color);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_ClockHands.SetSize(size);
	m_ClockHands.SetCenter(size * 0.5);
	m_ClockHands.SetPos(pos);
	m_ClockHands.SetAng(DtoR(-360.0f * timeRatio));
	m_ClockHands.Draw();

}
