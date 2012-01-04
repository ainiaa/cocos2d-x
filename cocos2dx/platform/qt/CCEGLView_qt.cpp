/****************************************************************************
Copyright (c) 2012 cocos2d-x.org

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

#include "CCEGLView_qt.h"

#include "CCGL.h"

#include "CCSet.h"
#include "ccMacros.h"
#include "CCDirector.h"
#include "CCTouch.h"
#include "CCTouchDispatcher.h"
#include "CCIMEDispatcher.h"

NS_CC_BEGIN;
static CCEGLView* s_pMainWindow = NULL;

CCEGLView::CCEGLView()
: m_bCaptured(false)
, m_bOrientationReverted(false)
, m_bOrientationInitVertical(false)
, m_pDelegate(NULL)
, bIsInit(false)
, m_eInitOrientation(CCDeviceOrientationPortrait)
, m_fScreenScaleFactor(1.0f)
{
	m_pTouch = new CCTouch;
	m_pSet = new CCSet;
	m_sSizeInPoint.width = m_sSizeInPoint.height = 0;
//    SetRectEmpty(&m_rcViewPort);
}

CCEGLView::~CCEGLView()
{
}

bool CCEGLView::Create(const char* pTitle, int iPixelWidth, int iPixelHeight, int iWidth, int iHeight, int iDepth) {

	return true;
}

CCSize CCEGLView::getSize()
{
	return CCSize((float)(m_sSizeInPoint.width), (float)(m_sSizeInPoint.height));
}

bool CCEGLView::isOpenGLReady()
{
	return bIsInit;
}

void CCEGLView::release()
{
}

void CCEGLView::setTouchDelegate(EGLTouchDelegate * pDelegate) {
	//TODO touch event
	m_pDelegate = pDelegate;

}
void CCEGLView::swapBuffers() {
	if (bIsInit) {
		/* Swap buffers */
        // glfwSwapBuffers();
	}
}

int CCEGLView::setDeviceOrientation(int eOritation) {
	CCLog("warning:could not setDeviceOrientation after initialized");
    return -1;
}

void CCEGLView::setViewPortInPoints(float x, float y, float w, float h) {
//	TODO
	if (bIsInit) {
		float factor = m_fScreenScaleFactor / CC_CONTENT_SCALE_FACTOR();
		glViewport((GLint)(x * factor) + m_rcViewPort.origin.x,
				(GLint)(y * factor) + m_rcViewPort.origin.y,
				(GLint)(w * factor),
				(GLint)(h * factor));
	}
}
void CCEGLView::setScissorInPoints(float x, float y, float w, float h) {
	//TODO
	if (bIsInit) {
		float factor = m_fScreenScaleFactor / CC_CONTENT_SCALE_FACTOR();
		glScissor((GLint)(x * factor) + m_rcViewPort.origin.x,
				(GLint)(y * factor) + m_rcViewPort.origin.y,
				(GLint)(w * factor),
				(GLint)(h * factor));
	}
}

void CCEGLView::setIMEKeyboardState(bool bOpen) {

}

//void CCEGLView::resize(int width, int height) {
//	//TODO
//	return;
//}

//void CCEGLView::centerWindow() {
//	//TODO
//	return;
//}
//void CCEGLView::setScreenScale(float factor) {
//	m_fScreenScaleFactor = factor;
//
//}

bool CCEGLView::canSetContentScaleFactor() {
	return false;
}

void CCEGLView::setContentScaleFactor(float contentScaleFactor) {
	CCLog("could not set contentScaleFactor after initialized");

}

CCEGLView& CCEGLView::sharedOpenGLView()
{
	CC_ASSERT(s_pMainWindow);
	return *s_pMainWindow;
}

NS_CC_END;
