#ifndef _TERRAIN_DATA_H_
#define _TERRAIN_DATA_H_

#include "xTerrainDefinations.h"
#include "BaseLib/xStringTable.h"
#include "RenderAPI/xTextureManager.h"
#include "XMathLib/xvolume.h"

using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
// �������������
// DataSet��		��ǰΨһ�ģ�ָʾ��ǰ���༭������������ܺͣ������ڵ����򹹳�
//
// Data��			ĳ����������ݣ�ָʾ���������е�ĳ�����飬�ɲ�������ɡ�
//					�����Ǹ��ݾ������ġ����磬һ��1km x 1km��������5m���ȣ��ó��Ĳ�������200x200����
//					ͬ����С�����������1m x 1m�ľ��ȣ����������1000x1000����
//					�ж�����Data�Ƿ��ڽӵ���Ҫ��Ϣ����λ�á�һ��Data������λ������ܹ�����һ��Data������λ����ĳ����Ե���غϣ��������Ϊ�Ǵ����ڽӵ�״̬��
//					���ڱ�����������أ�ĳ������ֻ�ܱ����ɸ�Patch�ֱ濪�������������������Ҫ��ֵ��ȷ����һ�㡣
//
// TerrainPatch��	��Ⱦ�ĸ��һ��Patch�൱����Data Set��һ��View��Viewһ�鵽���Data��
//					Terrain Patch����Viewһ�鵽����Data�������������Ҫ����
//					Patchӳ���飬����ǱȽϻ����ģ���˵�ˡ�
//					Patchӳ���飬����Щ��û�����ݣ���ʱ���ȱ�����ڽӿ飬��ô��LOD����ʱ��Ҫע�⡣
//
// ����������������
//		��ǰ�ķ����ǣ�ͬʱ��ֻ�ܴ���һ�龫�ȵ�Data������������£����ǿ�����ΪPatch����ֱ�ӷ�ӳһ�龫�ȷ�Χ��
//		Ŀǰÿ������1000x800�����ӳ�䵽1024x1024��Ҳ����8x8Patch��
//
//		����һ�ַ���������ͬ��Terrain��߱���ͬ�ľ��ȣ�����������£����ǵ�ƴ�����⣬Patch����֯���밴��LOD����
//		���磬����1m���Ȼ���5m���ȣ�Terrain������ݶ�һ����ֻ��1m��������LOD0����ʾ����߲�����LOD0����5m��������LOD4����ʾ����߲�����LOD4����
//


// ����������
class xTerrainData
{
public:
	xTerrainData();
	~xTerrainData();
	struct SurfaceData
	{
		HBaseTexture hTexture;
		float minU;
		float minV;
		float sizeU;
		float sizeV;
	};

private:

	// �̱߳�
	float*	m_pDepth;

	// ���������������ղ��������㣬��λ���㣩
	unsigned int m_ResX;
	unsigned int m_ResY;

	// ��ǰPatch����ʵ��γ���µ���ʼ�ͽ���
	double m_realStartX;
	double m_realStartY;
	double m_realEndX;
	double m_realEndY;

	// ������Ϊ������Tile ID
	int _TileX;
	int _TileY;

	// ���ݾ��ȣ�ÿ�����������������ʵ���ȣ���λ���ף�
	xvec3 m_vScale;

	// ��Ӧ��TIFF
	std::ds_wstring m_ccImageName;

	// �������ݼ��뵽Set�Ż����ɣ�_format������

	unsigned int m_DataIDXInLevel;
	unsigned int m_DataIDYInLevel;
	// ���ĵ㣨�����ݼ�����õ�����ʼ����ϵ��
	xvec3 m_vRelativeCenter;
	// ��������ʼ�������ݼ�����õ�����ʼ����ϵ��
	xvec3 m_relativeStart;

private:
	// �����
	// TODO �����ļ�
	HBaseTexture m_hTexture;
	// �����С�����ղ��������㣬��λ���㣩
	unsigned int m_surfResX;
	unsigned int m_surfResY;
	std::ds_wstring m_ccTextureName;
	//// ���澫�ȣ�ÿ����������������ʵ���ȣ���λ���ף�
	//float m_surfDeltaX;
	//float m_surfDeltaY;

public:

	inline 	int GetTileX() const { return _TileX; }
	inline 	int GetTileY() const { return _TileY; }

	inline double getRealStartX() {return m_realStartX;}
	inline double getRealStartY() {return m_realStartY;}
	inline double getRealEndX() {return m_realEndX;}
	inline double getRealEndY() {return m_realEndY;}
	inline unsigned int getXID() {return m_DataIDXInLevel;}
	inline unsigned int getYID() {return m_DataIDYInLevel;}

public:
	inline unsigned int getResX(){return m_ResX;}
	inline unsigned int getResY(){return m_ResY;}

	inline unsigned int getSurfaceResX(){ return m_surfResX; }
	inline unsigned int getSurfaceResY(){ return m_surfResY; }

	inline const xvec3& getScale() const { return m_vScale; }
	inline const xvec3& getStart() const { return m_relativeStart; }
	inline const xvec3& getCenter() const{ return m_vRelativeCenter; }

	inline float		getHeight(float X,float Y)
	{
		unsigned int x = X - m_relativeStart.x;
		unsigned int y = Y - m_relativeStart.y;
		if ( x < m_ResX && y < m_ResY )
			return m_pDepth[y*m_ResX + x];
		return 0.0f;
	}

	float	getAcceptableData(double x, double y);

public:

	void _format(unsigned int Xinlevel, unsigned int Yinlevel, double setStartX, double setStartY);
	void _clearupEdges(xTerrainData* px, xTerrainData* pX, xTerrainData* py, xTerrainData* pY);
	// ��ѯ������
	// ��ѯ��������
	// һ���������ݼ��ɶ�����Patch��Ⱦ�����ÿ������Patchֻ��View��������ݼ��е�һС�顣
	void patchView(unsigned int startX, unsigned int startY, unsigned int sampleX, unsigned int sampleY, std::vector<float>& outDepth, SurfaceData& outSurface);

	bool load(xBaseTextureMgr* pTexMgr, xXmlNode* pNode);
	bool save(xXmlNode* pNode);

};

// xTerrainDataSet��TerrainData�ļ��ϣ����վ�γ�������й���
// ͬʱֻ����һ��xTerrainDataSet
class xTerrainDataSet
{
public:
	xTerrainDataSet();
	~xTerrainDataSet();
private:

	// Terrain Data��
	// һ��Scene���ܰ�������Height Data��ÿ��Height Data���վ�γ�������֣���������Ҳ��Ҫ���ǽ�ȥ��
	// Terrain Dataռλ�ǰ���ʵ��ռλ����ġ�
	std::vector< xTerrainData*,dllsafe_alloc<xTerrainData*> > m_TerrainHeightDatas;
	// Bound
	double			m_TerrainOriginX;
	double			m_TerrainOriginY;
	double			m_TerrainSizeX;
	double			m_TerrainSizeY;
	// Terrain Data���򸲸ǣ���ЩData���ܲ����ڣ�
	unsigned int	m_TerrainDataCountX;
	// Terrain Data���򸲸ǣ���ЩData���ܲ����ڣ�
	unsigned int	m_TerrainDataCountY;
	xBaseTextureMgr* m_pTexMgr;

public:

	// TODO ���õ����
	inline void _setTextureMgr(xBaseTextureMgr* pTexMgr) { m_pTexMgr = pTexMgr; }

	// ��ѯ��ǰ���ݼ�����ϵԭʼ����
	double getOriginX();
	double getOriginY();
	double getSizeX();
	double getSizeY();

	// ��ѯĳ��ĸ߶ȣ������X,Y��ָ�����ݼ��ֲ�����ϵ��λ�ã�
	float getHeight(float X, float Y);
	// ��ѯĳ��ĸ߶ȣ������X,Y��ָ��ʵλ�ã�
	float getHeightReal( double x, double y);

	void GetSideTerrain(xTerrainData* pSource, xTerrainData** ppx, xTerrainData** ppX, xTerrainData** ppy, xTerrainData** ppY);
public:

	bool load(xXmlNode* pNode);
	bool save(xXmlNode* pNode);
	// ע�⣺TerData�����ǿյģ�����ĳ�����ǿյģ�
	struct PatchDataView
	{
		xTerrainData* pData;
		unsigned int xID;
		unsigned int yID;
	};
	void generatePatchView( unsigned int tileCountX, unsigned int tileCountY, unsigned int& patchXCount, unsigned int& patchYCount, std::vector<PatchDataView>& pTerData );

private:

	// �����ڽӡ������ԣ��ȵ�
	bool analysis();

};


END_NAMESPACE_XEVOL3D

#endif