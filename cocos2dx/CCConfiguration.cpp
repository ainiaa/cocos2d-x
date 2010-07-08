/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCConfiguration.h"
#include "ccMacros.h"
#include "ccConfig.h"

using namespace std;

// singleton stuff
static CCConfiguration *g_pSharedConfiguration;
static char *g_pGlExtensions;

CCConfiguration::CCConfiguration()
{
	CCLOG("cocos2d: GL_VENDOR:     %s", glGetString(GL_VENDOR));
	CCLOG("cocos2d: GL_RENDERER:   %s", glGetString(GL_RENDERER));
	CCLOG("cocos2d: GL_VERSION:    %s", glGetString(GL_VERSION));

	g_pGlExtensions = (char *)glGetString(GL_EXTENSIONS);

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_nMaxTextureSize);
	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &m_nMaxModelviewStackDepth);

	m_bSupportsPVRTC = checkForGLExtension("GL_IMG_texture_compression_pvrtc");
	m_bSupportsNPOT = checkForGLExtension("GL_APPLE_texture_2D_limited_npot");
	m_bSupportsBGRA8888 = checkForGLExtension("GL_IMG_texture_format_BGRA888");
	m_bSupportsDiscardFramebuffer = checkForGLExtension("GL_EXT_discard_framebuffer");

	CCLOG("cocos2d: GL_MAX_TEXTURE_SIZE: %d", m_nMaxTextureSize);
	CCLOG("cocos2d: GL_MAX_MODELVIEW_STACK_DEPTH: %d",m_nMaxModelviewStackDepth);
	CCLOG("cocos2d: GL supports PVRTC: %s", (m_bSupportsPVRTC ? "YES" : "NO"));
	CCLOG("cocos2d: GL supports BGRA8888 textures: %s", (m_bSupportsBGRA8888 ? "YES" : "NO"));
	CCLOG("cocos2d: GL supports NPOT textures: %s", (m_bSupportsNPOT ? "YES" : "NO"));
	CCLOG("cocos2d: GL supports discard_framebuffer: %s", (m_bSupportsDiscardFramebuffer ? "YES" : "NO"));
	CCLOG("cocos2d: compiled with NPOT support: %s",
#if CC_TEXTURE_NPOT_SUPPORT
				"YES"
#else
				"NO"
#endif // CC_TEXTURE_NPOT_SUPPORT 
			  );

	CCLOG("cocos2d: compiled with VBO support in TextureAtlas : %s",
#if CC_TEXTURE_ATLAS_USES_VBO
			  "YES"
#else
			  "NO"
#endif // CC_TEXTURE_ATLAS_USES_VBO
			  );
}

CCConfiguration* CCConfiguration::sharedConfiguration(void)
{
	if (! g_pSharedConfiguration)
	{
		g_pSharedConfiguration = new CCConfiguration();
	}

	return g_pSharedConfiguration;
}

bool CCConfiguration::checkForGLExtension(const string &searchName)
{
	bool ret = false;
	const char *kSearchName = searchName.c_str();
	
	if (strstr(g_pGlExtensions, kSearchName))
		ret = true;

	delete kSearchName;

	return ret;
}