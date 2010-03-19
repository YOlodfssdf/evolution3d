#include "xTerrainTool.h"
#include "geotiffio.h"
#include "libxtiff/xtiffio.h"
#include "geo_normalize.h"
void GeoTif_Scale(float* OriginDepth , int OriginW , int OriginH , float* NewDepth , int imageWidth , int imageHeight)
{
    // ԭʼ���磺1000 X 800���㵽1025 X 1025
    float xfactor = ((float) (OriginW  - 1)) / 1025.0f;
    float yfactor = ((float) (OriginH - 1)) / 1025.0f;
    // ���ɸ߶���Ϣ
    uint32 px , py;
    uint32 sample_x[2];
    uint32 sample_y[2];
    for ( int y = 0 ; y < imageHeight ; ++y )
    {
        for ( int x = 0 ; x < imageWidth ; ++x )
        {
            // �ҵ��������������ͼ�ϵ�����λ��
            float fx = x * xfactor;
            float fy = y * yfactor;
            px = fx;     py = fy;

            if(px > fx) { sample_x[0] = px - 1 ; sample_x[1] = px    ; } 
            else        { sample_x[0] = px     ; sample_x[1] = px + 1; }

            if(py > fy) { sample_y[0] = py - 1 ; sample_y[1] = py    ; } 
            else        { sample_y[0] = py     ; sample_y[1] = py + 1; }

           
            // ȡ�����صĸ߶�
            float factor_x = sample_x[1] - fx ;
        
            float v1 = OriginDepth[ sample_y[0]*OriginW + sample_x[0] ]  *  factor_x +
                       OriginDepth[ sample_y[0]*OriginW + sample_x[0] ]  *  (1.0f - factor_x ) ;

            float v2 = OriginDepth[ sample_y[1]*OriginW + sample_x[0] ]  *  factor_x +
                       OriginDepth[ sample_y[1]*OriginW + sample_x[0] ]  *  (1.0f - factor_x ) ;

            float factor_y = sample_y[1] - fy ;


            NewDepth[imageWidth*y + x] = v1 * factor_y + v2 * (1.0f - factor_y)  ;//OriginDepth[py*imageWidth + px]; 
        }
    }
}