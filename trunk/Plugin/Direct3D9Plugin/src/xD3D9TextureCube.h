#ifndef _XEVOL_DIRECT3D9_CUBE_TEXTURE_H_
#define _XEVOL_DIRECT3D9_CUBE_TEXTURE_H_
#include "xD3D9BaseTexture.h"
#include "xD3D9BaseRenderTarget.h"
BEGIN_NAMESPACE_XEVOL3D

class xD3D9TextureCube : public xD3D9BaseTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD3D9TextureCube);
protected:
	bool                      m_bReadable;
	xD3D9TextureBuffer        m_texBuffer;
	IDirect3DCubeTexture9*    m_pCubeTexture;
    IDirect3DSurface9*        m_pLockedSurface;
    xTextureInitDesc          m_InitDesc;
    
protected:
	//bool                     _load(ID3D11Resource* pTexture);
	bool                     _createCubeTexture(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData , int nInitData );

	bool                     _lockreadAble(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0 );
	bool                     _unlockreadAble(xTextureLockArea& lockInfo);

public:
	xD3D9TextureCube(xD3D9RenderApi* pAPI , bool bReadable = false);
	~xD3D9TextureCube();
	virtual void*          handle();
	virtual bool           load(const wchar_t* fileName , unsigned long  arg);
	virtual bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	virtual bool           isLoaded();
	virtual bool           unload();

	virtual bool           update(void* data  , int dateLen , int rowPitch , int depthPicth , int mipmapLevel = 0 , int arraySlice = 0);
	virtual bool           lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0 );
	virtual bool           unlock(xTextureLockArea& lockInfo);
	virtual bool           create(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData = NULL, int nInitData = 0);
	virtual IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0);
    virtual bool           grabRenderTagetData(void* pData , int x , int y , int w , int h  , int arraySlice , int mipmapLevel );

    void onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action);
};


END_NAMESPACE_XEVOL3D

#endif

