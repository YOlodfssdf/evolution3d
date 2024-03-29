// TestBase.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <BaseLib/xXmlDocument.h>
#include <BaseLib/xLogger.h>
#include <OperationSys/xOperationSys.h>
#include <Application/xPluginMgr.h>
#include <Application/xPlatform.h>
#include <Application/xApplication.h>
#include <RenderApi/xRenderApi.h>
#include <RenderAPI/xBaseShader.h>
#include <RenderAPI/xShaderNode.h>
#include <RenderAPI/xShaderManager.h>
#include <tchar.h>
#include <XMathLib/xMathlib.h>
#include <Image/xILImage.h>
#include <RenderAPI/xBaseTexture.h>
#include "xModel/xBaseModel.h"
#include "Renderer/xDrawableHelper.h"
#include "Image/xFreeImage.h"
#include "font/xDistanceFont.h"
using namespace XEvol3D;
int _tmain(int argc, _TCHAR* argv[])
{
	xOperationSys::singleton()->init(argv[0]);
	xXmlDocument xml;
	xml.load(_XEVOL_ABSPATH_(L".\\fontGenerator.xml"));
	const wchar_t* sourceFile = xml.root()->value(L"source");
	const wchar_t* destFile   = xml.root()->value(L"dest");
	int      destSize   = xml.root()->int_value(L"dest_size");
	int      superSzie  = xml.root()->int_value(L"super_size");

	xDistanceFontGenerator fontGen(destSize , superSzie);
	//fontGen.generate( _XEVOL_ABSPATH_(L".\\font\\STXINGKA.TTF") ,_XEVOL_ABSPATH_(L".\\font\\test") );
	fontGen.generate( _XEVOL_ABSPATH_(sourceFile) ,_XEVOL_ABSPATH_(destFile) );
	return 0;
}

