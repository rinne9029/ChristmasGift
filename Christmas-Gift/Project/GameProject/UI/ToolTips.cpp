#include "ToolTips.h"

//コンストラクタ
ToolTips::ToolTips()
	:Task(ETaskTag::eUI,true)
	,m_Iskill(false)
{
	m_Image = COPY_RESOURCE("MouseL", CImage);
}

//2D描画処理
void ToolTips::Draw()
{
	Utility::DrawQuad(CVector2D(1040, 320), CVector2D(500, 128), CVector4D(1, 1, 1, 0.4f));
	FONT_T()->Draw(1134, 400, 1, 0, 0,m_Text);
	m_Image.SetPos(1060,330);
	m_Image.SetSize(64,128);
	m_Image.Draw();

	if (m_Iskill)Kill();
}
