#ifndef __XEVOL_PARTICLE_H__
#define __XEVOL_PARTICLE_H__
#include "xParticleSystem.h"
BEGIN_NAMESPACE_XEVOL3D;

//�����ٶȵĲ���
template<typename T> class  xParticleAcellArg
{
public:
    T   m_Value;      //��ǰֵ
    T   m_Velocity;   //�ٶ�
    T   m_Accellator; //���ٶ�
public:    
    void           update(float passedTime)
    {
        m_Value    += passedTime * m_Velocity;
        m_Velocity += passedTime * m_Accellator;
    }
};

//���ٶȵĲ���
template<typename T> class  xParticleVelocityArg
{
public:
    T   m_Value;      //��ǰֵ
    T   m_Velocity;   //�ٶ�
public:
    void           update(float passedTime)
    {
        m_Value    += passedTime * m_Velocity;
    }
};

class _XEVOL_BASE_API_ xParticleInfo
{
public:
    xParticleAcellArg<xMathLib::xvec3> m_AngleArg;   //�Ƕȡ���ŷ���Ǳ�ʾ
    xParticleAcellArg<xMathLib::xvec3> m_PosArg;     //λ��
    xParticleAcellArg<xMathLib::xvec3> m_SizeArg;
    xParticleVelocityArg<xColor_4f>    m_ColorArg;   //��ɫ
    float                              m_life;       //��ǰ����ֵ
    float                              m_TotalLife;  //ʣ������ֵ
};

class _XEVOL_BASE_API_ IParticle : public IRefCountObject
{
public:
    void           update(float passedTime);
public:
    xParticleInfo  m_Info;
};

class _XEVOL_BASE_API_ IParticlePool : public IBaseObject
{
public:
    IMPL_BASE_OBJECT_CLASSID_INTERFACE(IParticlePool);
    IParticlePool(IParticleEmitter* , int);
    virtual IParticle* createParticle() = 0;
    virtual void       update(IParticleEffector* pEffector , float passedTime) = 0 ;
    virtual bool       load(xXmlNode* pXml) = 0;
    virtual bool       save(xXmlNode* pXml) = 0;
    virtual void       render(float passedTime) = 0;
	virtual void       onEmitte(IParticle* pParticle) = 0 ;
protected:
    IParticleEmitter* m_Emitter;
};

class _XEVOL_BASE_API_ xParticlePoolCreator;
class _XEVOL_BASE_API_ xParticlePoolFactoryMgr;
DECL_OBJECT_FACTORY_AND_MGR(IParticlePool , xParticlePoolCreator , xParticlePoolFactoryMgr , IParticleEmitter* , int );

#define IMPL_PARTICLEPOOL_FACTORY_2(PoolClass , PoolName)    IMPL_OBJECT_FACTORY(PoolClass , IParticlePool ,  PoolClass##Factory , xParticlePoolCreator , xParticlePoolFactoryMgr ,  _TEXT_(PoolName)  ,  _TEXT_(PoolName)   , IParticleEmitter* , int );
#define IMPL_PARTICLEPOOL_FACTORY(PoolClass)                 IMPL_OBJECT_FACTORY(PoolClass , IParticlePool ,  PoolClass##Factory , xParticlePoolCreator , xParticlePoolFactoryMgr ,  _WIDETEXT_(#PoolClass),  _WIDETEXT_(#PoolClass) , IParticleEmitter* , int );


END_NAMESPACE_XEVOL3D
#endif
