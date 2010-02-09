// TestBase.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "TestBase.h"
#include "working/xWorkingThread.h"
#include "XMathLib/xspline.h"
using namespace XEvol3D;

class CMyJob : public IWorkingJob
{
	int  m_JobID;
	bool m_InWorking;
public:
	IMPL_REFCOUNT_OBJECT_INTERFACE(CMyJob);
	CMyJob(int iJobId)
	{
		m_InWorking = false;
		m_JobID = iJobId;
	}
	bool  isWorking()
	{
		return m_InWorking;
	}

	void  beginWorking()
	{
		m_InWorking = true;
	}
	void  endWorking() 
	{
		m_InWorking = false;
	}
	bool  doWorking()
	{
       XEVOL_LOG(eXL_DEBUG_HIGH ,L"Do a Job Working ID = %d \n" , m_JobID );
	   xSleep(1000 + rand()%1000 );
	   return true;
	}
};

xWorkingThreadPool g_ThreadPool;
bool   CEvolEnviroment::onResize()
{
	x2DRect _rect;
	xTextureDesc _desc;
	m_pRenderApi->onResize(0 , 0 );
	m_pRenderApi->getRenderView()->desc(_desc);
	m_p2DCamera->setPerspective2D(_desc.m_width , _desc.m_height , 60 ,true);
	m_pCamera->setAspect( float(_desc.m_width)/(_desc.m_height) );
	return true;
}
void     CEvolEnviroment::topCamera(xGeomLib::xaabb& aabox)
{
	xvec3 cen = 0.5f * (aabox.m_Max + aabox.m_Min);
	xvec3 len =        (aabox.m_Max - aabox.m_Min);

	m_pCamera->m_Eye.m_EyePos    = xvec4(cen.x         ,cen.y , cen.z  + 10 * len.z);
	m_pCamera->m_Eye.m_EyeTarget = xvec4(cen.x         ,cen.y , cen.z);
	m_pCamera->m_Eye.m_Up        = xvec4(0.0f,1.0f,0.0f);
	m_cameraStep = len.x/5.0f;
	m_pCamera->setFarPlan(len.z * 15.0f);
	m_pCamera->setNearPlan(0.1f);
}

xvec3   CEvolEnviroment::getModelCeneter(xBaseModel* pModel)
{
	xGeomLib::xaabb& aabox = pModel->boundBox();
	xvec3 cen = 0.5f * (aabox.m_Max + aabox.m_Min);
	return cen;
}

void     CEvolEnviroment::frontCamera(xGeomLib::xaabb& aabox)
{
	xvec3 cen = 0.5f * (aabox.m_Max + aabox.m_Min);
	xvec3 len =        (aabox.m_Max - aabox.m_Min);

	m_pCamera->m_Eye.m_EyePos    = xvec4(cen.x + 10 * len.x ,cen.y , cen.z );
	m_pCamera->m_Eye.m_EyeTarget = xvec4(cen.x         ,cen.y , cen.z);
	m_pCamera->m_Eye.m_Up        = xvec4(0.0f,0.0f,1.0f);
	m_cameraStep = len.x/10.0f;
	m_pCamera->setFarPlan(len.x * 150.0f);
	m_pCamera->setNearPlan(0.1f);
}

void     CEvolEnviroment::exit()
{
	if(m_pScene)
	{
		m_pScene->unload();
		m_pScene->ReleaseObject();
	}
	m_hProgram.setNULL();
	m_hFont.setNULL();
	m_pEngine->exit();
}

xvec2i*  CEvolEnviroment::GetSelectData( xTextureDesc& _desc , int x , int y)
{
	m_pRenderApi->lock();
	m_pRenderApi->colorSelector()->setRenderObjectID(0xcdef0a74 , 0x12345678 );
	m_pRenderApi->beginSelectMode();

	m_pRenderApi->getRenderView()->desc(_desc);
	xvec2i * pRTData = new xvec2i[_desc.m_width * _desc.m_height];

	if( x < 0  || x >= _desc.m_width || y < 0  || y >= _desc.m_height )
	{
		m_pRenderApi->endSelectMode(0 , 0 , _desc.m_width, _desc.m_height , pRTData );
		m_pRenderApi->unlock();
		delete [] pRTData ;
		return NULL;
	}

	updateFrame(0 , m_ColorSelVisitor);
	m_pRenderApi->endSelectMode(0 , 0 , _desc.m_width, _desc.m_height , pRTData );
	m_pRenderApi->unlock();
	return pRTData;
}

void  CEvolEnviroment::ReleaseSelectData(xvec2i* pRTData)
{
	delete [] pRTData ;
}


bool   CEvolEnviroment::onMessage(xWindowMsg& msg)
{
	if(m_xuiWinMgr == NULL || m_pRenderApi == NULL)
		return false;

	bool bRet = m_xuiWinMgr->dispatchMsg(msg);
    if(msg.MsgID == WIN_CLOSE )
	{
	    m_pEngine->platform()->quit();
	}
	if(msg.MsgID == WIN_RESIZE)
	{
		onResize();
		return true;
	}
	else if(msg.MsgID == WIN_KEYUP)
	{
		if(msg.Keyboard.cKeyChar >= '1' && msg.Keyboard.cKeyChar <= '9')
		{
			//m_Model->setAction( msg.Keyboard.cKeyChar - '1' );
		}

		if(msg.Keyboard.nKey == VKEY_F9)
		{
			bool bDrawPlane = m_pEngine->sysValue(L"defualt",L"drawPlane" , false);
		}

		if(msg.Keyboard.nKey == VKEY_DELETE)
		{
			bool bAllowOpScene = m_pEngine->sysValue(L"defualt",L"sceneEditable" , true);
			if(bAllowOpScene && GetSelection()->GetCurSelNode())
			{
				ISceneNode* pNode = GetSelection()->GetCurSelNode();
				ISceneNode* pParentNode = pNode->getParent();
				if(pParentNode == NULL)
				{
					MessageBox(GetActiveWindow() , L"�ڵ�û�и��ڵ�", L"����" , MB_OK);
				}
				else
				{
					pParentNode->removeChild(pNode,true);
					GetSelection()->setCurSelNode(NULL);
				}
			}
		}
		return false;
	}
	else if(msg.MsgID == WIN_KEYDOWN)
	{
		if(msg.Keyboard.cKeyChar == 'a' )
		{
			m_pCamera->shift(-m_cameraStep);
		}
		else if(msg.Keyboard.cKeyChar == 'd')
		{
			m_pCamera->shift(m_cameraStep);
		}
		else if(msg.Keyboard.cKeyChar == 'w')
		{
			m_pCamera->toward(m_cameraStep);
		}
		else if(msg.Keyboard.cKeyChar == 's')
		{
			m_pCamera->toward(-m_cameraStep);
		}
		else if(msg.Keyboard.nKey == VKEY_UP)
		{
			m_pCamera->pitch(2);
		}
		else if(msg.Keyboard.nKey == VKEY_DOWN)
		{
			m_pCamera->pitch(-2);
		}
		if(msg.Keyboard.nKey == VKEY_LEFT)
		{
			m_pCamera->yaw(2);
		}
		else if(msg.Keyboard.nKey == VKEY_RIGHT)
		{
			m_pCamera->yaw(-2);
		}
		else if(msg.Keyboard.nKey == VKEY_SHIFT)
		{
			m_pCamera->upDown(m_cameraStep);
		}
		else if(msg.Keyboard.nKey == VKEY_CONTROL)
		{
			m_pCamera->upDown(-m_cameraStep);
		}
		else 
		{
			return false;
		}
		return true;
	}
	m_pScenePlacementOprator->onMessage(msg);
	return false;
}

const wchar_t*     CEvolEnviroment::sysCfgfile()
{
	return m_cfgFile.c_str() ; 
}

bool     CEvolEnviroment::createScene(const wchar_t* _SceneType)
{
	xSceneInitArg arg;
	arg.m_pModelMgr   = m_pEngine->modelMgr();
	arg.m_pRenderer   = this->m_pRenderer;
	arg.m_pTextureMgr = m_pEngine->textureManager();
	arg.m_pResPathMgr = m_pEngine->getResPathMgr();
	ISceneGraph* pScene = xSceneFactoryMgr::singleton()->createInstance( _SceneType , NULL , NULL );
	if(pScene == NULL)
		return false;

	pScene->setRenderer(arg.m_pRenderer);
	pScene->setModelManager(arg.m_pModelMgr);
	pScene->setTextureManager(arg.m_pTextureMgr);
	pScene->setPathManager(arg.m_pResPathMgr);
	return  __AfterCreateScene(pScene , L"");
}

bool     CEvolEnviroment::loadScene(const wchar_t* _Scene)
{

	if(_Scene != NULL)
	{
		xSceneInitArg arg;
		arg.m_pModelMgr   = m_pEngine->modelMgr();
		arg.m_pRenderer   = this->m_pRenderer;
		arg.m_pTextureMgr = m_pEngine->textureManager();
		arg.m_pResPathMgr = m_pEngine->getResPathMgr();
		ISceneGraph* pNewScene = XEvol_CreateScene(_Scene ,arg );
		return __AfterCreateScene(pNewScene, _Scene);

	}
	return false;
}

bool CEvolEnviroment::__AfterCreateScene( ISceneGraph* pNewScene, const wchar_t* _Scene ) 
{
	if(pNewScene)
	{
		if(m_pScene)
		{
			m_pScene->unload();
			m_pScene->ReleaseObject();
		}
		m_pScene = pNewScene;
		m_sceneFile = _Scene;
		m_pScene->setPathManager( m_pEngine->getResPathMgr() );
		return true;
	}
	return false;
}

bool    CEvolEnviroment::init()
{
	m_pScene = NULL;
	wchar_t appname[512] = {0};
	GetModuleFileName(NULL , appname , 512);
	xOperationSys::singleton()->init( appname );
	xStringTable::singleton()->addPath(_XEVOL_ABSPATH_(L"Language/"));
	xStringTable::singleton()->setLanguage(NULL);

	m_pEngine = new xEvol3DEngine;
	m_cfgFile = _XEVOL_ABSPATH_(L"system.xml");
	m_pEngine->initPlatform(this , m_cfgFile.c_str() , _XEVOL_ABSPATH_(L"./") );
   
	//4���߳�
	g_ThreadPool.init(4);

	for(int i = 0  ; i < 20 ; i ++)
	{
		CMyJob* pJob = new CMyJob(i + 0);
		g_ThreadPool.addJob(pJob);
	}
	return true;
}


void     CEvolEnviroment::initRenderer(HWND hRenderWindow)
{
	m_pEngine->initRenderer(L"BaseDemo" , (void*)hRenderWindow );
	m_pRenderApi = m_pEngine->renderApi();
	//===============

	m_pRenderer = xRendererManager::singleton()->createRenderer(L"KidRenderer",m_pRenderApi);
	m_pRenderer->loadConfig( sysCfgfile() ,NULL,0);

	m_pEngine->textureManager()->addPath( _XEVOL_ABSPATH_(L"./texture/Arch/") );
	m_pEngine->textureManager()->addPath( _XEVOL_ABSPATH_(L"./texture/Planet/") );
	wchar_t* ch_ = L"��";
	//xDistanceFontGenerator fontGen(32 , 4096 , ch_[0] , ch_[0] + 2);
	//fontGen.generate( _XEVOL_ABSPATH_(L".\\font\\STXINGKA.TTF") ,_XEVOL_ABSPATH_(L".\\font\\test") );
	//fontGen.generate( _XEVOL_ABSPATH_(L".\\font\\msyh.TTF") ,_XEVOL_ABSPATH_(L".\\font\\test") );
	//m_FontGen.load(m_pRenderApi , _XEVOL_ABSPATH_(L"./font/font.tga"));
	//m_FontGen.proccess();

	m_pRenderApi = m_pRenderApi;


	//��ʼ�������
	m_pCamera = m_pRenderApi->createCamera(L"Default");
	m_pCamera->m_Eye.m_EyePos = xvec4(0.0f,0.0f,-500.0f);
	m_pCamera->m_Eye.m_EyeTarget = xvec4(0.0f,0.0f,0.0f);
	m_pCamera->m_Eye.m_Up = xvec4(0.0f,1.0f,0.0f);


	m_pCamera->setFarPlan(1000.0f);
	m_pCamera->setNearPlan(0.1f);
	m_pCamera->setFOV(45.0f);
	m_pCamera->setProjectType(xGeomLib::xCamera::PT_PROJECT);

	m_p2DCamera = m_pRenderApi->createCamera(L"2DCamera");
	onResize();

	m_ProcedureTexture = m_pRenderApi->createLockableTexture(256,256,PIXELFORMAT_R32G32B32A32F,false );
	xTextureLockArea _lock;
	m_ProcedureTexture->lock(eLock_WriteDiscard , _lock);
	m_ProcedureTexture->unlock(_lock);
	m_pStencilState = m_pRenderApi->createDepthStencilState(L"Overlay");

	m_hProgram = m_pRenderApi->gpuProgramManager()->load(L"simple2D.vertex" , L"simple2D.pixel<0:simple.texture,simple.fakehdr>" , NULL);;

	m_hFont    = m_pRenderApi->findFont(L"small" );


	//m_Model = m_pModelMgr->add(L"einxin.xrm" , true , 0);
	xGeomLib::xaabb aabb;
	aabb.m_Max = xvec3(20,20,20);
	aabb.m_Min = xvec3(-20, -20, -20);
	frontCamera( aabb );

	m_DefRasterizer= m_pRenderApi->createRasterizerState(L"Default");
	m_pRenderApi->setRasterizerState(m_DefRasterizer);




	m_GridPlan = (IDrawableHelper *)(IDrawableHelper *)xDrawElementFactoryMgr::singleton()->createInstance(L"PlaneGrid" , this->renderer() , 0) ;
	m_GridPlan->init(m_pEngine->textureManager() );


	m_pSelView = m_pRenderApi->createRenderView(800 , 600 , true);
	m_pSelView->createRenderTarget(1 , PIXELFORMAT_R8G8B8A8 , true , true);

	ILightingEnv* pLightEnv = m_pRenderApi->findLightingState(L"xLightState");
	pLightEnv->enableLight(2);
	xLightDesc* pLight= pLightEnv->lightDesc(0);
	pLight->m_position  = xMathLib::xvec4(1000.0f , 1000.0f , 1000.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 25.0f); 
	pLight->m_Ambient   = xMathLib::xvec4(0.1f , 0.1f , 0.1f , 1.0f);

	//�ڶ����ƹ�
	pLight= pLightEnv->lightDesc(1);
	pLight->m_position  = xMathLib::xvec4(1000.0f , -1000.0f , -500.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 25.0f);  
	pLight->m_Ambient   = xMathLib::xvec4(0.1f , 0.1f , 0.1f , 1.0f);

	pLightEnv->applyChange();
	m_xuiWinMgr = xui::xuiWindowManager::createManager(m_pRenderApi , m_pEngine->textureManager() );
	m_xuiWinMgr->getXMLManager()->addPath(_XEVOL_ABSPATH_(L"xui/"));
	m_InfoPanel  = XUI_CREATE_DIALOG(xuiDemoInfoPannel,m_xuiWinMgr);



	//������Ⱦ������ȾTech.
	m_pRenderer->setRenderTech(L"Basic");

	m_ColorSelVisitor     = new xSceneColorSelectVisitor(m_pRenderer , m_pCamera);
	m_renderVisitor = new xSceneRenderVisitor(m_pRenderer , m_pCamera);

	//��ʼ��������
    m_Selection.init(this);

	m_pScenePlacementOprator = new xObjPlacement( & this->m_Selection );
	m_pScenePlacementOprator->init(this);
	m_pScenePlacementOprator->SetMoveOperator();
	

	xImage* pImg = xImageLib::load(IMAGE_UNKNOWN, _XEVOL_ABSPATH_(L"./texture/test.dds") , (const int8*)NULL , (int)0 , PIXELFORMAT_R8G8B8A8 );

	return ;
}

void     CEvolEnviroment::run()
{
	this->setApplicationState(IApplication::AS_RUN);
	m_pEngine->messageLoop();
}

void     CEvolEnviroment::updateFrame(long passedTime)
{
	return updateFrame(passedTime , m_renderVisitor);
}


void     CEvolEnviroment::updateFrame(long passedTime , ISceneVisitor* pVisitor)
{
	static float angle = 0;
	static int   nFrame = 0;
	static int   tPassed = 0;
	tPassed += passedTime;
	nFrame ++;
	angle += passedTime/1000.0f * 35.0f;
	xSleep(5);
	m_pRenderApi->identityMatrix(MATRIXMODE_World);
	m_pRenderApi->applyCamera(m_pCamera);
	m_pRenderApi->begin(xColor_4f(0.0f,0.0f,0.3f,1.0f));
	m_pRenderApi->beginScene();
	xMathLib::xmat4 mat;
	//xMathLib::XM_RotateY(mat,angle);

	//����ģ��
	m_pRenderApi->colorSelector()->setRenderObjectID(1,0);
	m_pRenderApi->identityMatrix(MATRIXMODE_World);

	this->m_pRenderer->begin(passedTime);
	//m_pModelEffect->draw( m_pModelElement );

	xSceneRenderVisitor rv(m_pRenderer , m_pCamera);
	xSceneUpdateVisitor uv;
	uv.setFrameTime(passedTime);
	if(m_pScene) 
	{
		m_pScene->visit(&uv);
		m_pScene->visit(pVisitor);
	}
	this->m_pRenderer->end(passedTime);


	bool bDrawPlane = m_pEngine->sysValue(L"defualt",L"drawPlane" , false);
	if(bDrawPlane)
	{
		//Plan
		m_pRenderApi->identityMatrix(MATRIXMODE_World);
		xMathLib::XM_Scale(mat,500.0f,500.0f,500.0f);
		m_pRenderApi->multiMatrix(mat.data, MATRIXMODE_World);
		m_pRenderApi->colorSelector()->setRenderObjectID(101,0);
		m_GridPlan->begin();
		m_GridPlan->render(passedTime);
	}

	m_pRenderApi->applyCamera(m_p2DCamera);
	m_pRenderApi->setBlendState( m_pRenderApi->createBlendState(L"AlphaBlend") );
	//===================
	m_pRenderApi->identityMatrix(MATRIXMODE_World);
	m_pRenderApi->setDepthStencilState(m_pStencilState);
	xvec4     destRect(20,300,400,300);
	xvec4     srcRect(0,0,256,256);
	xColor_4f cl(1.0f,1.0f,1.0f,1.0f);
	IGpuProgram* pGpuProgram = m_hProgram.getResource()  ;
	wchar_t texInfo[1024] = {0};
	swprintf(texInfo , L"FPS=%f \nRenderApi=%s" , nFrame*1000.0f/tPassed ,m_pRenderApi->name() );

	m_InfoPanel->show();
	ds_wstring infoText = ds_wstring(texInfo) + L"\nMudus 3D Studio "; 
	m_InfoPanel->setText(infoText.c_str() );
	m_xuiWinMgr->draw();
	//m_pRenderApi->
	//===================
	m_pRenderApi->endScene();
	//m_pRenderApi->endSelectMode(0,0,NULL);
	m_pRenderApi->swapBuffer();         
}

CEvolEnviroment::CEvolEnviroment()
{
	m_pEngine   = NULL;
	m_pRenderApi   = NULL;
	m_GridPlan   = NULL;

	m_pCamera   = NULL;
	m_pStencilState   = NULL;
	m_DefRasterizer   = NULL;
	m_ProcedureTexture   = NULL;
	m_cameraStep =1.0f;

	m_pSelView   = NULL;
	m_pRenderer    = NULL;


	return ;
}



int _tmain(int argc, _TCHAR* argv[])
{
	CEvolEnviroment app;
	xOperationSys::singleton()->init(argv[0]);
	

	xMathLib::xquat q1( xMathLib::xvec4(1.0,0.0f,0.0f,1.0) , -90.0f);
	xMathLib::xquat q2( xMathLib::xvec4(0.0,0.0f,1.0f,1.0) , 90.0f);
	xMathLib::xquat q3 = q2 * q1;

	xMathLib::xvec3 axis;
	float angle;
	q3.toAxis( axis , angle);

	xGeomLib::xBezier<xvec3> myBezier(2);
	myBezier[0] = xvec3(1.0f , 1.0f , 1.0f);
	myBezier[1] = xvec3(1.0f , 4.0f , 2.0f);

	xvec3 v1 = myBezier(0.0f);
	xvec3 v2 = myBezier(1.0f);

	app.init();
	app.initRenderer(NULL);
	//app.loadScene(_XEVOL_ABSPATH_(L".\\WorldComposer\\scene\\demoScene.xml"));
	app.loadScene(_XEVOL_ABSPATH_(L".\\demoScene.xml"));
	app.run();
	app.exit();
    IBaseObject::DumpObjectList();
	return 0;
}

