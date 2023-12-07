#pragma once
#include "../GLLibrary.h"

class CRendaring {
private:

	CTextureFrame* m_gbuffer;
	CTextureFrame* m_tmp_buffer;
	CTextureFrame* m_ligting_buffer;
	CTextureFrame* m_scene;
	CTextureFrame* m_blurtmp, *m_blur, *m_blur1, *m_blur2;
	float m_focus;
	static CRendaring* m_instance;
	void DrawFillQuad();
	void Blur(CTexture* src, CTextureFrame* tmp, CTextureFrame* result, float pow, float pix = 1.0f);
public:

	CRendaring(int screen_width, int screen_height,float focus);
	~CRendaring();

	static void CreatInstance(int screen_width, int screen_height, float focus=0.0f);
	static CRendaring* GetInstance();
	static void ClearInstance();
	
	void Render(std::function<void()> render);

	void SetFocus(float f) {
		m_focus = f;
	}

};