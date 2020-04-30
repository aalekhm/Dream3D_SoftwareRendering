#pragma once
#include "Common/Defines.h"
#include <vector>
#include <GL/glew.h>

#define SPR_MAX					16384
#define TEXTURE_CORE			0
#define FONT_TEXTURE_START_X	700
#define CHARACTER_WIDTH			7
#define CHARACTER_HEIGHT		13

class Console2D
{
	public:
		static Console2D*		getInstance();

		void					fillRect(u32 COLOR_ID, Rect* rect);
		void					drawFont(int xX, int yY, int charW, int charH, int tX, int tY);
		void					drawQuadU(	int TEXTURE_ID,
											float posX, float posY, float posW, float posH,
											float texX, float texY, float texW, float texH);
		void					drawStringFont(const char* cStr, int x, int y, int anchor);
		void					updateConsole();
		void					renderConsole();

		void					setClip(int x, int y, int w, int h);
	protected:
								Console2D();
		virtual					~Console2D();

		void					init();
		bool					loadTexture(char* sTexName, GLuint &texid, GLuint* texW, GLuint* texH);
		void					glSelectTexture(int id);
		
	private:
		static Console2D*		m_pInstance;

		VertexT2F_C4UB_V3F*		m_VertexBuffer;
		int32_t					m_iSpriteCount;
		RectF*					m_ClipRect;
		ColorUV*				m_ColorUVTable;

		GLuint					m_iGLTexture;
		GLuint					m_iTexW;
		GLuint					m_iTexH;
};