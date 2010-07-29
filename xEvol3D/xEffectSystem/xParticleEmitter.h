#ifndef __XEVOL_PARTICLE_EMITTER_H__
#define __XEVOL_PARTICLE_EMITTER_H__
#include "xParticleSystem.h"
#include "xParticle.h"
BEGIN_NAMESPACE_XEVOL3D;

struct xEmitterInfo
{
    nsParticleSystem::Vec3Range     m_VelocityRange;//�ٶȵķ�Χ  
    nsParticleSystem::Vec3Range     m_AcellRange;   //���ٶȵķ�Χ 
   
    nsParticleSystem::ColorRange    m_ColorRange;
    nsParticleSystem::ColorRange    m_ColorVelRange;

    nsParticleSystem::Vec3Range     m_AngleAcellRange;
    nsParticleSystem::Vec3Range     m_AngleVelRange;//�Ƕȱ仯���ٶȣ����ٶ�

    nsParticleSystem::Vec3Range     m_SizeRange;    //��С������
    nsParticleSystem::Vec3Range     m_SizeVelRange; //��С�ı仯���ٶ�
    nsParticleSystem::Vec3Range     m_SizeAcellRange;//��С�ı仯�ļ��ٶ�

    nsParticleSystem::IntRange      m_EmitterFrequecy;//ÿ����������Ӹ�����

    nsParticleSystem::ScalarRange   m_LifeRange;//�����ķ�Χ
    nsParticleSystem::CHANGE_TYPE   m_colorChangeType;

    nsParticleSystem::EMITTER_TYPE  m_EmitterType;//�����������ͣ�
    nsParticleSystem::ScalarRange   m_InterValTime;//�ǳ�������ļ��ʱ��
    std::ds_wstring                 m_PoolType;

public:
    xEmitterInfo()
    {
      m_VelocityRange.setRange( xMathLib::xvec3(-50.0f , -50.0f , 10.0f) , xMathLib::xvec3(50.0f , 50.0f , 50.0f) );//�ٶȵķ�Χ  
      m_AcellRange.setRange( xMathLib::xvec3(0.0f , 0.0f , 0.0f) , xMathLib::xvec3(0.0f , 0.0f , 0.0f) );;   //���ٶȵķ�Χ 
   
      m_ColorRange   .setRange( xColor_4f(1.0f , 1.0f , 1.0f , 1.0f) , xColor_4f(0.4f , 0.4f , 0.4f , 1.0f) );
      m_ColorVelRange.setRange( xColor_4f(0.0f , 0.0f , 0.0f , 0.0f) , xColor_4f(0.0f , 0.0f , 0.0f , 0.0f) );

      m_AngleAcellRange.setRange( xMathLib::xvec3(0.0f , 0.0f , 0.0f) , xMathLib::xvec3(10.0f , 10.0f , 10.0f) );
      m_AngleVelRange  .setRange( xMathLib::xvec3(0.0f , 0.0f , 0.0f) , xMathLib::xvec3(0.0f , 0.0f , 0.0f) );//�Ƕȱ仯���ٶȣ����ٶ�

      m_SizeRange     .setRange( xMathLib::xvec3(20.0f , 20.0f , 20.0f) , xMathLib::xvec3(40.0f , 40.0f , 40.0f) );    //��С������
      m_SizeVelRange  .setRange( xMathLib::xvec3(0.0f , 0.0f , 0.0f) , xMathLib::xvec3(0.0f , 0.0f , 0.0f) ); //��С�ı仯���ٶ�
      m_SizeAcellRange.setRange( xMathLib::xvec3(0.0f , 0.0f , 0.0f) , xMathLib::xvec3(0.0f , 0.0f , 0.0f) );//��С�ı仯�ļ��ٶ�

      m_EmitterFrequecy.setRange( 1 , 10 );//ÿ����������Ӹ�����

      m_LifeRange.setRange( 4.0f , 10.0f );//�����ķ�Χ
      m_colorChangeType = nsParticleSystem::CHANGE_UNIFORM;

      m_EmitterType  = nsParticleSystem::ET_CONTINUE ;//�����������ͣ�
      m_InterValTime.setRange( 1.0f , 10.0f );;//�ǳ�������ļ��ʱ��
    }
};

class _XEVOL_BASE_API_ IParticleEmitter : public IParticle
{
public:
    typedef ds_vector(IParticleEffector*)  vEffectors;

    IParticleEmitter(xParticleSystem* pParticleSystem , int arg);
public:
    virtual void                 pushEffector(IParticleEffector* pEffector);
    virtual void                 popEffector();
    virtual int                  nEffector();
    virtual IParticleEffector*   getEffector(int idx);

public:
    virtual bool                 init(const wchar_t* poolType) = 0;
    virtual bool                 load(xXmlNode* pXml) = 0;
    virtual bool                 save(xXmlNode* pXml) = 0; 
    virtual xMathLib::xvec3      emittePos() = 0;
public:
    virtual void                 update(float passedTime);
    //��������״̬
    virtual void                 start();
    virtual void                 stop();
    virtual bool                 render(float passedTime);
    virtual bool                 is_active();
    virtual void                 emitte(int n );
protected:
    virtual bool                 _emitte();
  
public:
    xParticleSystem*             particleSystem() { return m_pParticleSystem ; }

	xEmitterInfo                 m_EmitterInfo;
protected:
    vEffectors                   m_vEffectors;
    IParticlePool*               m_Pool;
    xParticleSystem*             m_pParticleSystem;
    

    bool                         m_EmitterContinue;
    bool                         m_isEmitter;
    int                          m_EmittedParticle;//�Ѿ�������������Ŀ
    float                        m_PassedTime;//�Ѿ���ȥ��ʱ�䣮���ϴ������ʼ��ʱ
    float                        m_fCountNeedGen;//������Ҫ���������Ӹ���,�����һ��ҪС��1����Ϊÿ�μ����ʱ��  ����Ҫ������㼸�����ӣ������ֵ�ۼ���ȥ
   
};

class _XEVOL_BASE_API_ xParticleEmitterCreator;
class _XEVOL_BASE_API_ xParticleEmitterFactoryMgr;
DECL_OBJECT_FACTORY_AND_MGR(IParticleEmitter , xParticleEmitterCreator , xParticleEmitterFactoryMgr , xParticleSystem* , int );

#define IMPL_PARTICLEEMITTER_FACTORY_2(EmitterClass , EmitterName) IMPL_OBJECT_FACTORY(EmitterClass , IParticleEmitter ,  EmitterClass##Factory , xParticleEmitterCreator , xParticleEmitterFactoryMgr ,  _TEXT_(EmitterName)  ,  _TEXT_(EmitterName)   , xParticleSystem* , int );
#define IMPL_PARTICLEEMITTER_FACTORY(EmitterClass)                 IMPL_OBJECT_FACTORY(EmitterClass , IParticleEmitter ,  EmitterClass##Factory , xParticleEmitterCreator , xParticleEmitterFactoryMgr ,  _WIDETEXT_(#EmitterClass),  _WIDETEXT_(#EmitterClass) , xParticleSystem* , int );


END_NAMESPACE_XEVOL3D
#endif
