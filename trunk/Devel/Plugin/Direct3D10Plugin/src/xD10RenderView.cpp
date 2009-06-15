#include "xD10RenderView.h"
#include "xDirect3D10API.h"
#include "xD10RenderTarget.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xD10RenderView   , IRenderView);
IMPL_BASE_OBJECT_CLASSID(xD10RenderWindow , xD10RenderView);

xD10RenderView::xD10RenderView(xD3D10RenderApi* pRenderApi) : IRenderView(pRenderApi)
{
	m_RefCount = 1;
	m_pD10Api = pRenderApi;
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		m_RenderTargetsView[i] = NULL;
		m_RenderTargets[i] = NULL;
	}
	m_DepthTexture      = NULL;
	m_pDepthStencilView    = NULL;
	m_DepthBuffer          = NULL;

	IRenderTarget* pDefDepthBuffer = m_pD10Api->DefDepthBuffer();
	if(pDefDepthBuffer)
	{
		xTextureDesc desc;
		pDefDepthBuffer->desc( desc);
		m_widht   = desc.m_width;
		m_height  = desc.m_height;
	};
}

xD10RenderView::~xD10RenderView()
{
	destory();
}
bool  xD10RenderView::destory()
{
	XSAFE_RELEASEOBJECT(m_DepthTexture);
	XSAFE_RELEASEOBJECT(m_DepthBuffer);
	for(size_t i = startIdx() ; i < MAX_RENDER_TARGET ; i ++ )
	{
		XSAFE_RELEASEOBJECT(m_RenderTargets[i]);
	}
	return true;
}

bool  xD10RenderView::createRenderTarget(int nRenderTarget , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture)
{
	int w = m_widht;
	int h = m_height;
	for(size_t i = 0 ; i < (size_t)nRenderTarget ; i ++ )
	{
		IRenderTarget* pRenderTarget = m_pD10Api->createRenderTarget(w , h ,fmt , bLockable , bAsTexture);
		setRenderTarget(pRenderTarget , i);
	}
	return setup();
}

int  xD10RenderView::nRenderTargetView()
{
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		if( m_RenderTargetsView[i] == NULL)
			return (int)i;
	}
	return MAX_RENDER_TARGET;
}

bool xD10RenderView::setRenderTarget(IBaseTexture*  pTexture , size_t rtIdx) 
{
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return false;

	if(pTexture == NULL)
		return false;
	IRenderTarget* pRT = pTexture->toRenderTarget();
	return setRenderTarget(pTexture , rtIdx);
}

bool xD10RenderView::setRenderTarget(IRenderTarget* pRenderTarget , size_t rtIdx)
{
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return false;

	if(pRenderTarget == NULL)
		return false;
	
	ID3D10RenderTargetView* pRTV = (ID3D10RenderTargetView*)pRenderTarget->handle();
	if(pRTV == NULL)
	   return false ; 

	//�������ü���
	pRenderTarget->AddRef();
	if(m_RenderTargets[rtIdx]) m_RenderTargets[rtIdx]->ReleaseObject();
	m_RenderTargets[rtIdx] = pRenderTarget;
	m_RenderTargetsView[rtIdx] = pRTV;
	return true;
}

bool xD10RenderView::setDepthBuffer(IRenderTarget* pDepthBuffer) 
{
	if(pDepthBuffer == NULL) 
	{
		if(m_DepthTexture == NULL)
			return false;
		setDepthBuffer( m_DepthTexture->toRenderTarget() );
	}

	ID3D10DepthStencilView* pDSV = (ID3D10DepthStencilView*)pDepthBuffer->handle();
	if(m_DepthBuffer) m_DepthBuffer->ReleaseObject();
	pDepthBuffer->AddRef();
	m_DepthBuffer = pDepthBuffer;
	m_pDepthStencilView = pDSV;

	xTextureDesc desc;
	m_DepthBuffer->desc( desc);
	m_widht   = desc.m_width;
	m_height  = desc.m_height;
	return m_pDepthStencilView != NULL ; 
}

bool xD10RenderView::resize(int w , int h  , bool bDestorRT)
{
	TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	if(bDestorRT)
	{
		for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
		{
			XSAFE_RELEASEOBJECT( m_RenderTargets[i] );
		}
	}
	resize(w , h);
	return true;
}

bool xD10RenderView::resize(int w , int h)
{
	TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	if( m_DepthTexture )
	{
		m_DepthTexture->unload();
		if( false == m_DepthTexture->create(w , h , PIXELFORMAT_DEPTH32) )
		{
			return false;
		}
		return setDepthBuffer( m_DepthTexture->toRenderTarget() );
	}
	return false;
}

int xD10RenderView::addRenderTarget(IBaseTexture*  pTexture)
{ 
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		if( m_RenderTargetsView[i] == NULL)
		{
			if(setRenderTarget(pTexture , i) == false)
				return (int)i ;
			return (int)i + 1;
		}
	}
	return MAX_RENDER_TARGET;
}

int xD10RenderView::addRenderTarget(IRenderTarget* pRenderTarget) 
{ 
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		if( m_RenderTargetsView[i] == NULL)
		{
			if(setRenderTarget(pRenderTarget , i) == false) 
				return (int)i ;
			return (int)i + 1;
		}
	}
	return MAX_RENDER_TARGET;; 
}

IRenderTarget*  xD10RenderView::depthBuffer()
{ 
	if(m_DepthBuffer) 
	{
		m_DepthBuffer->AddRef();
		return m_DepthBuffer;
	}
	return m_DepthTexture->toRenderTarget();
}

IRenderObject*  xD10RenderView::renderTarget(size_t rtIdx )
{ 
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return NULL;
	m_RenderTargets[rtIdx]->AddRef();
	return m_RenderTargets[rtIdx] ; 
}

ID3D10DepthStencilView* xD10RenderView::depthView()
{
	ID3D10DepthStencilView* pDepthStencilView = m_pDepthStencilView;
	if(pDepthStencilView == NULL) pDepthStencilView = m_pD10Api->DefDepthStencilView();
	return pDepthStencilView;
}

bool xD10RenderView::install()
{
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	int nRV = nRenderTargetView();
	if(nRV == 0)
		return false;
	pDevice->OMSetRenderTargets(nRV , m_RenderTargetsView , depthView() );
	
	D3D10_VIEWPORT vp;
	xTextureDesc _desc;
	desc(_desc);
	vp.Width  = _desc.m_width;
	vp.Height = _desc.m_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDevice->RSSetViewports( 1, &vp );
	return true;
}

bool xD10RenderView::setup()
{
	int nRT  = nRenderTargetView();
    if(m_TextureDesc.m_nArraySize == 0)
		return false;
	xTextureDesc _desc;
	depthBuffer()->desc(_desc);
	for(size_t i = 0 ; i < (size_t)nRT ; i ++)
	{
		m_RenderTargets[i]->desc( m_TextureDesc );
		if(_desc.m_fmt != m_TextureDesc.m_fmt || _desc.m_width != m_TextureDesc.m_width || _desc.m_height != m_TextureDesc.m_height)
			return false;
	}
	m_TextureDesc.m_nArraySize = nRT;
	return true;
}
bool xD10RenderView::clear(xColor_4f& bkColor)
{
	TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	pDevice->ClearRenderTargetView( m_RenderTargetsView[0]  , &bkColor.r );
	return true;
}

bool xD10RenderView::clear(xColor_4f& bkColor , float z , unsigned int stencil)
{
    TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	pDevice->ClearRenderTargetView( m_RenderTargetsView[0]  , &bkColor.r );
	pDevice->ClearDepthStencilView( depthView() , D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL , z, stencil );
	return true;
}
bool xD10RenderView::clear(xColor_4f& bkColor, int nClear)
{
	TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	for(int i = 0 ;  i < nClear ; i ++)
	{
		if(m_RenderTargetsView[i]) pDevice->ClearRenderTargetView( m_RenderTargetsView[i]  , &bkColor.r );
	}
	return true;
}
bool xD10RenderView::clear(xColor_4f& bkColor ,  float z , unsigned int stencil, int nClear )
{
	TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	for(int i = 0 ;  i < nClear ; i ++)
	{
		if(m_RenderTargetsView[i]) pDevice->ClearRenderTargetView( m_RenderTargetsView[i]  , &bkColor.r );
	}
	pDevice->ClearDepthStencilView( depthView() , D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL , z, stencil );
	return true;
}

bool xD10RenderView::createDepthView(int w , int h)
{
	HRESULT hr = S_OK;
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	
	xD10DepthTexture* pDepthTexture = new xD10DepthTexture(false , false , m_pD10Api);
	if( false == pDepthTexture->create(w , h , PIXELFORMAT_DEPTH32) )
	{
		delete pDepthTexture;
		return false;
	}
	m_DepthTexture = pDepthTexture;
	return setDepthBuffer( pDepthTexture->toRenderTarget() );
}

//==================���ڻ���====================================================
xD10RenderWindow::xD10RenderWindow(HWND hWnd , xD3D10RenderApi* pRenderApi) : xD10RenderView(pRenderApi),m_WindowRT(pRenderApi)
{
	m_hWnd = hWnd;
	m_pSwapChain = NULL;
	m_pD10Api = pRenderApi;
}

xD10RenderWindow::~xD10RenderWindow()
{
	destory();
}

bool xD10RenderWindow::resize(int w , int h)
{
    TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	if(m_widht == w && m_height == h )
		return true; 
	m_pD10Api->d10Device()->OMSetRenderTargets(0 , NULL , NULL );

	XSAFE_UNLOAD(m_DepthTexture);
	XSAFE_RELEASE(m_RenderTargetsView[0]);
	DXGI_SWAP_CHAIN_FLAG swcFlag = (DXGI_SWAP_CHAIN_FLAG)0;
	m_widht  = w;
	m_height = h;
	DXGI_SWAP_CHAIN_DESC _desc;
	m_pSwapChain->GetDesc(&_desc);
	HRESULT hr = m_pSwapChain->ResizeBuffers(_desc.BufferCount , m_widht , m_height , _desc.BufferDesc.Format , swcFlag);
    return _createRenderTargets();
}

bool xD10RenderWindow::_createRenderTargets()
{
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	HRESULT hr = S_OK;
	// Create a render target view
	ID3D10Texture2D *pBackBuffer;
	hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (LPVOID*)&pBackBuffer );
	if( FAILED(hr) )
		return false;

	hr = pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_RenderTargetsView[0] );
	pBackBuffer->Release();
	if( FAILED(hr) )
		return false;

	m_WindowRT.m_TexDesc.m_width  = m_widht;
	m_WindowRT.m_TexDesc.m_height = m_height;
	//���´���Depth Stencil view
    if( createDepthView(m_widht , m_height) )
	{
		return 	setup();
	}
	return false;
}

bool xD10RenderWindow::create(IDXGISwapChain* pSwapChain , int width , int height)
{
	m_pSwapChain = pSwapChain;
	DXGI_SWAP_CHAIN_DESC _desc;
	m_pSwapChain->GetDesc(&_desc);
	xD10GIFormatInfo* pFmtInfo = xD10ConstLexer::singleton()->GetPixelFormat(_desc.BufferDesc.Format);
	if(pFmtInfo == NULL)
	{
		m_WindowRT.m_TexDesc.m_fmt = PIXELFORMAT_R8G8B8A8;
	}
	else 
	{
		m_WindowRT.m_TexDesc.m_fmt = pFmtInfo->m_fmt;
	}

	m_widht  = width;
	m_height = height;
	m_WindowRT.m_TexDesc.m_width  = m_widht;
	m_WindowRT.m_TexDesc.m_height = m_height;
	m_WindowRT.m_TexDesc.m_nArraySize = 1;
	m_WindowRT.m_TexDesc.m_depth  = 1;

	m_RenderTargets[0] = &m_WindowRT;
	return _createRenderTargets();
}

bool  xD10RenderWindow::destory()
{
	m_RenderTargets[0] = NULL;
	XSAFE_RELEASE(m_RenderTargetsView[0]);
	return xD10RenderView::destory();
}

END_NAMESPACE_XEVOL3D