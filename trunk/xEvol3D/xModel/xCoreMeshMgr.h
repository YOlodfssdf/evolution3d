#ifndef __xCoreMeshMgr_h__
#define __xCoreMeshMgr_h__

#include "../BaseLib/xXmlDocument.h"
#include "../RenderAPI/xRenderAPI.h"
#include "../RenderAPI/xBaseTexture.h"
#include "../RenderAPI/xTextureManager.h"
#include "../xcomdoc/xcomdoc.h"
#include "../BaseLib/xResPackageMgr.h"
#include "../BaseLib/xResourceMgr.h"

#include "xModelFWDecl.h"


BEGIN_NAMESPACE_XEVOL3D

//======Define of the MeshManager
class  _XEVOL_BASE_API_ xCoreMeshName
{
public:
	ds_wstring      m_MeshName   ;    //ÿ��mesh���Լ�������. ������ֿ��ܻ��һ�����
	ds_wstring      m_ModelName  ;    //ÿ��Mesh���ܻ�����ĳһ��Model.���������ĳ��Model,��Ҳ
	int             m_strHash    ;
    xCoreSkeleton*  m_pSkeleton;
	xcomdoc         m_comdoc     ;    //����ø����ĵ�����,��Ӹ��ĵ���ȥload mesh
public:
	bool operator == (const xCoreMeshName& rhv) const;
	bool operator <  (const xCoreMeshName& rhv) const;
	xCoreMeshName(const wchar_t* meshName = NULL, const wchar_t* modelName = NULL, xCoreSkeleton* pSkeleton = NULL);
	~xCoreMeshName();
#ifdef _MSC_VER
	size_t hash_value() const;
#endif
};

#ifdef _MSC_VER
inline size_t hash_value(const xCoreMeshName& _Keyval)
{
	return ( _Keyval.hash_value() ^ _HASH_SEED);
}
#endif


class  _XEVOL_BASE_API_  xCoreMeshLoader 
{
public:
	void   KillObject();
	static xCoreMeshMgr* createInstance(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr , const wchar_t* _name);
protected:
	unsigned int  _getResSize(xCoreMesh* pRes);
	virtual  bool _isResLoaded(xCoreMesh* pRes);
	virtual  bool _postLoadResource  (const xCoreMeshName& strResName , xCoreMesh* & pRes , int& ResSize, unsigned int arg){ return true ; }
	virtual  bool _preLoadResource  (const xCoreMeshName& strResName , xCoreMesh* & pRes , int& ResSize, unsigned int arg) { return true ; }
	virtual  bool _loadResource  (const xCoreMeshName& strResName , xCoreMesh* & pRes , int& ResSize, unsigned int arg);
	virtual  bool _unloadResource(const xCoreMeshName& strResName , xCoreMesh* & pRes , unsigned int& TotalResSize);
	virtual  void _deleteResource(const xCoreMeshName& strResName , xCoreMesh* pRes);
protected:
	bool __loadFromDirectory(const xCoreMeshName& strResName,xCoreMesh* & pCoreMesh , xResPkgPathItem& resPkgItem, int i);
	bool __loadFromPackage(const xCoreMeshName& strResName,xCoreMesh* & pCoreMesh , xResPkgPathItem& item , int i);
	xCoreMesh*    newInstance(const xCoreMeshName& name);
protected:
	xCoreMeshLoader();
	virtual ~xCoreMeshLoader();
protected:
	IRenderApi*       m_pRenderApi;
	xBaseModelLoader* m_pOwner;
	std::ds_wstring   m_MeshExtName;
};


END_NAMESPACE_XEVOL3D
#endif
