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

    /// ����һ��Polygon������һ��Line Strip�ߣ�Triangle List�棩
    /// ���鷵�ص�Line��Triangle����ȫ����������Ǻϡ�
    virtual void gainDecalLines(xMathLib::xvec3* pVertices, int VertexCount, xMathLib::xvec3*& pOutDecalVertices, int& OutDecalVertCount) { return ; }
    virtual void gainDecalTriangles(xMathLib::xvec3* pVertices, int VertexCount, xMathLib::xvec3*& pOutDecalVertices, unsigned short*& pOutDecalTriangles) { return ; }

    /// ѹƽһ�����
    /// �ṩ��˳ʱ���͹���������
    /// Ӧ�ã��������ڴ�͹������ڵ����ж���ȫ��ѹƽ��ƽ���߶ȡ�
    /// Ӧ�ú�ɻ�ȡ���˴�ѹƽ���ƽ���߶ȣ�OutHeight�����˴�ѹƽ��Ӱ�������OutArea���������OutVolume��
    virtual void setHeightsInPoly(xMathLib::xvec3* pVertices, int VertexCount, float& OutHeight, float& OutArea, float& OutVolume) { return ; }
    /// �˺�����ָ���߶ȵİ汾
    virtual void setSpecHeightsInPoly(xMathLib::xvec3* pVertices, int VertexCount, float SpecHeight, float& OutArea, float& OutVolume) { return ; }
    bool load(xXmlNode* pXml);
    bool save(xXmlNode* pXml);
};
END_NAMESPACE_XEVOL3D

#endif
