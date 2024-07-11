#include "TargetText.h"

TargetText::TargetText()
	:Task(ETaskTag::eUI,true)
{

}

void TargetText::Update()
{
	if (!GameData::isGift)
	{
		m_Text = "Žq‹Ÿ•”‰®‚ÉƒvƒŒƒ[ƒ“ƒg‚ð”z’u";
	}
	else
	{
		m_Text = "’g˜F‚É–ß‚Á‚Ä’Eo";
	}
}

void TargetText::Draw()
{
	Utility::DrawQuad(CVector2D(700, 0), CVector2D(500, 100), CVector4D(1, 1, 1, 0.4));
	CREATE_FONT_F
	(
		"name",
		"C:\\Windows\\Fonts\\msgothic.ttc",
		32
	)->Draw(720, 34, 0, 0, 0, "Mission");
	CREATE_FONT_F
	(
		"name",
		"C:\\Windows\\Fonts\\msgothic.ttc",
		32
	)->Draw(756, 70, 0, 0, 0, m_Text);
}
