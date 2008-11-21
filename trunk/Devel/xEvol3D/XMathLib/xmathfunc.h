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

#ifndef __XMATH_FUNC_INLINE_INCLUDE__
#define __XMATH_FUNC_INLINE_INCLUDE__

#include "xmathbasedef.h"

#ifdef __XMATHLIB_ASM__

#ifdef _MSC_VER
#include "XMath_ASM_VC.h"
#endif

#ifdef _GNUC_
#include "XMath_ASM_GCC.h"
#endif

#endif

#ifndef __XMATHLIB_ASM__
BEGIN_NAMESPACE_XMATHLIB
__FORCE_INLINE__ void   XM_SinCosD(float angle,float& s,float& c)
{
	angle = (float)angle / 180 * (float)XM_PI;
	s = (float)sin(angle);
	c = (float)cos(angle);
}

__FORCE_INLINE__ void   XM_SinCosR(float angle,float& s,float& c)
{
	s = (float)sin(angle);
	c = (float)cos(angle);
}

__FORCE_INLINE__ float  XM_CosD(float angle)
{
	angle = (float)angle / 180 * (float)XM_PI;
	return (float)cos(angle);
}

__FORCE_INLINE__ float  XM_SinD(float angle)
{
	angle = (float)angle / 180 * (float)XM_PI;
	return (float)sin(angle);
}

__FORCE_INLINE__ float  XM_CosR(float angle)
{
	return (float)cos(angle);
}

__FORCE_INLINE__ float  XM_SinR(float angle)
{
	return (float)sin(angle);
}


END_NAMESPACE_XMATHLIB
#endif


//��ͨ����ѧ�������
BEGIN_NAMESPACE_XMATHLIB
namespace static_table
{
	static double s_sin_table [36000];
	static double s_cos_table [36000];
}

__FORCE_INLINE__ void   XM_InitTriTable()
{
	double d = 1/180.0 *XM_PI;
	for(int i = 0 ;i < 36000 ; i ++)
	{
		static_table::s_cos_table[i] = (float)cos(i/100. * d);
		static_table::s_sin_table[i] = (float)cos(i/100. * d);
	}
}
__FORCE_INLINE__ double XM_FastSinD(float angle)
{
	int a = static_cast<int>(angle * 100);
	if(a > 36000)
	{
		return static_table::s_sin_table[a%36000];
	}
	else if(a < 0)
	{
		a = -a;
		if(a > 36000)
		{
			return -static_table::s_sin_table[a%36000];
		}
		else
		{
			return -static_table::s_sin_table[a];
		}
	}
	else
	{
		return -static_table::s_sin_table[a];
	}
}

__FORCE_INLINE__ double XM_FastCosD(float angle)
{
	int a = static_cast<int>(fabs(angle) * 100);

	if(a > 36000)
	{
		return static_table::s_cos_table[a%36000];
	}
	else
	{
		return static_table::s_cos_table[a];
	}
}

__FORCE_INLINE__ double XM_FastSinR(float angle)
{
	int a = static_cast<int>(angle * 180.0/XM_PI * 100);
	if(a > 36000)
	{
		return static_table::s_sin_table[a%36000];
	}
	else if(a < 0)
	{
		a = -a;
		if(a > 36000)
		{
			return -static_table::s_sin_table[a%36000];
		}
		else
		{
			return -static_table::s_sin_table[a];
		}
	}
	else
	{
		return -static_table::s_sin_table[a];
	}
}

__FORCE_INLINE__ double XM_FastCosR(float angle)
{
	int a = static_cast<int>(fabs(angle) * 180.0/XM_PI* 100);

	if(a > 36000)
	{
		return static_table::s_cos_table[a%36000];
	}
	else
	{
		return static_table::s_cos_table[a];
	}
}
END_NAMESPACE_XMATHLIB

//���õ���ѧ�������϶�û�л��汾��
BEGIN_NAMESPACE_XMATHLIB
const float NEAR_ZERO =0.0000000001f;
__FORCE_INLINE__ bool FEQUAL (float r,float l) 
{   
	return  ( fabs((r)-(l)) <= NEAR_ZERO ) ;
}

template <typename T> void  XM_Swap(T& arg1,T& arg2)
{
	T temp= arg1;
	arg1 = arg2;
	arg2 = temp;
	return ;
}



__FORCE_INLINE__ float XM_InvSqrt (float x)
{
	float hf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x*(1.5f - hf*x*x);
	return x;
}

__FORCE_INLINE__ float  XM_Rad2Deg(float rad)
{
	return (float)rad / (float)XM_PI * 180;
}

__FORCE_INLINE__ float  XM_Deg2Rad(float deg)
{
	return (float)deg / 180 * (float)XM_PI;
}


__FORCE_INLINE__ float  XM_ACosD(float c)
{
	float rt = (float)acos(c);
	return (float)(rt / XM_PI * 180);
}

__FORCE_INLINE__ float  XM_ASinD(float s)
{
	float rt = (float)asin(s);
	return (float)(rt / XM_PI * 180);
}
__FORCE_INLINE__ float  XM_ACosR(float c)
{
	return (float)acos(c);
}
__FORCE_INLINE__ float  XM_ASinR(float s)
{

	return (float)asin(s);
}
END_NAMESPACE_XMATHLIB

#endif

