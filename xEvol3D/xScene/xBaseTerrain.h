#ifndef _XTERRAIN_SCENE_BASE_H_
#define _XTERRAIN_SCENE_BASE_H_
#include "xSceneGraph.h"

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xTerrainBase : public ISceneGraph
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xTerrainBase);
public:
	xTerrainBase(ISceneGraph*  pScene , ISceneNode* pParent = NULL);
    virtual ~xTerrainBase();

public:
	//�ɶ�������
	virtual float height(float x , float y) = 0;
	virtual bool  region(float& x , float& y , float& w , float& h) = 0; //������α�ʾ������
    virtual bool  resolution(float& x  , float& y ) = 0 ; // x , y ����ķֱ���
	
	//��д������
    virtual bool  setHeight(float x , float y , float Height) = 0;
};
END_NAMESPACE_XEVOL3D

#endif
