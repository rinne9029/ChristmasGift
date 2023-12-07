#pragma once
#include "CMatrix.h"
#include "CLight.h"
#include "CCamera.h"
#include "CShader.h"
#include "CTextureFrame.h"
#include <functional>
class CShadow {
public:
	enum {
		eNone,
		eShadow,
		eDisplay,
	};
	int		m_depthtexWidth;
	int		m_depthtexHeight;
	int		m_screenWidth;
	int		m_screenHeight;
	CMatrix m_lightProj;
	CMatrix m_lightView;
	float	m_lightHeight;
	float	m_lightViewLength;
	int		m_state;
	CTextureFrame* mp_render_target;

	static CShadow *m_shadow;
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <param name="length">描画範囲</param>
	/// <param name="height">描画開始の高さ</param>
	/// <param name="texWidth">テクスチャーサイズ幅</param>
	/// <param name="texHeight">テクスチャーサイズ高さ</param>
	static void CreateInscance(float length = 40, float height=20,int texWidth = 2048, int texHeight = 2048);
	static CShadow* GetInstance();
	static void ClearInstance();
	CShadow(float length, float height,int texWidth, int texHeight);
	~CShadow();
	void SetLightProjectionMatrix(CMatrix &m) {
		m_lightProj = m;
	}
	void SetLightViewMatrix(CMatrix &m) {
		m_lightView = m;
	}
	/// <summary>
	/// 影付き描画
	/// </summary>
	/// <param name="render">モデルを描画する関数</param>
	/// <param name="gbuffer">最終的な描画先があれば指定</param>
	void Render(std::function<void()> render, CTextureFrame* gbuffer=nullptr);
	void DrawDepthTex(int x,int y,int w, int h);
	int GetState() {
		return m_state;
	}
};