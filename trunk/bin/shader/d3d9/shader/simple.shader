<?xml version ="1.0" encoding="unicode" ?>
<shader>
   <Direct3D9>
       <Paramaters>
              <sampler  name="diffTexture"        stage="0" address="repeat"/>
              <sampler  name="videoTexure"        stage="1" manager="default" file="videoTexture.mpeg" address="clamp"/>
              <sampler  name="ToonShaderTexture"  stage="3" manager="default" file="Toon.bmp" address="clamp"/>
       </Paramaters>

       <shader file="simple.vertex" type="VertexShader"/>
       <shader file="simple.pixel"  type="PixelShader" />
    </Direct3D9>
  
</shader>