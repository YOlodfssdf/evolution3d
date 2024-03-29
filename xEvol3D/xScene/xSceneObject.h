#ifndef _XSCENE_OBJECT_H_
#define _XSCENE_OBJECT_H_
#include "../RenderAPI/xRenderAPI.h"
#include "../BaseLib/xObjectFactory.h"
#include "../Renderer/xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ ISceneGraph;
class _XEVOL_BASE_API_ ISceneObject : public IRenderPassArg
{
public:
	typedef std::vector<ISceneObject* , dllsafe_alloc<ISceneObject*> > vSceneObjects;
public:
	DECL_BASE_OBJECT_DLLSAFE(ISceneObject);
public:
	ISceneObject(ISceneGraph*  pScene , int arg);
	virtual bool isDrawable() { return false ; }
	virtual bool updateFrame(unsigned long passedTime , IRenderCamera* pCamera) = 0;
	virtual bool load(xXmlNode* pXml)= 0;
	virtual bool unload() = 0;
	virtual bool save(xXmlNode* pNode) = 0;


    virtual unsigned  int memUsage() { return 1 ; }
    virtual bool          isLoaded() { return true ; }
    virtual bool          load(const wchar_t* _fileName , void* buf , size_t len , int arg){ return false ; }
    virtual bool          load(xcomdoc& doc , const wchar_t* _name , int arg ){ return false ; }

protected:
	ISceneGraph*          m_pSceneGraph;
};


//SceneNode Factory defination
class  _XEVOL_BASE_API_ xSceneObjectCreator;
class  _XEVOL_BASE_API_ xSceneObjectFactoryMgr;

DECL_OBJECT_FACTORY_AND_MGR(ISceneObject , xSceneObjectCreator , xSceneObjectFactoryMgr , ISceneGraph* , int);

#define IMPL_SCENEOBJECT_FACTORY(SceneObjClass , SceneObjName) IMPL_OBJECT_FACTORY(SceneObjClass ,ISceneObject ,  SceneObjClass##Factory , xSceneObjectCreator , xSceneObjectFactoryMgr ,  _TEXT_(SceneObjName)      ,  _TEXT_(SceneObjName)      , ISceneGraph* , int);
#define IMPL_SCENEOBJECT_FACTORY_2(SceneObjClass) IMPL_OBJECT_FACTORY(SceneObjClass ,ISceneObject ,  SceneObjClass##Factory , xSceneObjectCreator , xSceneObjectFactoryMgr ,  _WIDETEXT_(#SceneObjClass), _WIDETEXT_(#SceneObjClass) , ISceneGraph* , int);



END_NAMESPACE_XEVOL3D

#endif
