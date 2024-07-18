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
	Utility::DrawQuad(CVector2D(1040.0f, 320.0f), CVector2D(500.0f, 128.0f), CVector4D(1.0f, 1.0f, 1.0f, 0.4f));
	FONT_T()->Draw(1134, 400, 1.0f, 0.0f, 0.0f,m_Text);
	m_Image.SetPos(1060.0f,330.0f);
	m_Image.SetSize(64.0f,128.0f);
	m_Image.Draw();

	if (m_Iskill)Kill();
}
