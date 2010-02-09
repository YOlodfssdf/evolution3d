#ifndef __XEVOL_PARTICLE_EMITTER_H__
#define __XEVOL_PARTICLE_EMITTER_H__
#include "../BaseLib/stl_dllalloc.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xParticle
{
public:
	xvec4     m_size;      //���ӵĴ�С
	xColor_4f m_color;     //���ӵ���ɫ
	xvec4     m_pos;       //���ӵ�λ��
	xquat     m_rotate;    //���ӵ���ת
	xvec4     m_life;      //.x life ; .y total life.
public:

};

END_NAMESPACE_XEVOL3D
#endif
