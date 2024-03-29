/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef __XM_4X3_INLINE_INCLUDE__
#define __XM_4X3_INLINE_INCLUDE__

#include "xmathbasedef.h"

BEGIN_NAMESPACE_XMATHLIB

class  _XEVOL_BASE_API_ xmat4x3
{
public:
	xmat4x3()
	{
		memset((void*) data,0,sizeof(float)*12);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	xmat4x3( float  i )
	{
		memset((void*) data,0,sizeof(float)*12);
		m[0][0] = i;
		m[1][1] = i;
		m[2][2] = i;
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	xmat4x3( float _data[12] )
	{
		for(int  c = 0 ; c < 12 ; c++)
		{
			data[c] = _data[c];
		}
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void zero()
	{
		memset((void*) data,0,sizeof(float)*12);
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	xmat4x3(xvec3 vec[4])
	{
		m[0][0] = vec[0].x;
		m[0][1] = vec[0].y;
		m[0][2] = vec[0].z;

		m[1][0] = vec[1].x;
		m[1][1] = vec[1].y;
		m[1][2] = vec[1].z;

		m[2][0] = vec[2].x;
		m[2][1] = vec[2].y;
		m[2][2] = vec[2].z;

		m[3][0] = vec[3].x;
		m[3][1] = vec[3].y;
		m[3][2] = vec[3].z;

	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void identity()
	{
		memset((void*) data,0,sizeof(float)*12);
		m[0][0] = 1;
		m[1][1] = 1;
		m[2][2] = 1;
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------        
	void scale(float sx,float sy,float sz)
	{
		m[0][0] *= sx ; m[0][1] *= sx ; m[0][2] *= sx ;
		m[1][0] *= sy ; m[1][1] *= sy ; m[1][2] *= sy ;
		m[2][0] *= sz ; m[2][1] *= sz ; m[2][2] *= sz ;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void  from(const xmat4& _m);
	void  from(const xmat3& _m);

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void  transform_l_3x3(const xvec3& vin,xvec3& vout);
	void  transform_l_3x3(const xvec4& vin,xvec3& vout);
	void  transform_l_3x3(const xvec3& vin,xvec4& vout);
	void  transform_l_3x3(const xvec4& vin,xvec4& vout);
	void  transform_r_3x3(const xvec3& vin,xvec3& vout);
	void  transform_r_3x3(const xvec4& vin,xvec3& vout);
	void  transform_r_3x3(const xvec3& vin,xvec4& vout);
	void  transform_r_3x3(const xvec4& vin,xvec4& vout);

	operator float* (){ return data ; }
	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	bool  inverse();
public:
	union
	{
		struct{
			float m00,m01,m02;
			float m10,m11,m12;
			float m20,m21,m22;
			float m30,m31,m32;
		};
		float data[12];
		float m[4][3];
	};
};

END_NAMESPACE_XMATHLIB

#endif

