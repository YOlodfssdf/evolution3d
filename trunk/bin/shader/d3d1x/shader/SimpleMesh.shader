<?xml version="1.0" encoding="unicode"?>
<EvolShader >
   <Direct3D10>
      <shader type = "VertexShader" file = "simpleMesh.vertex(0:SkinAni)" />
      <!--<shader type = "GeomShader"   file = "GsShowNormal" />-->
      <shader type = "PixelShader"  file = "simpleMesh.pixel(0:simple.texture,simple.Lighting;)" />
   </Direct3D10>
  
   <Direct3D11>
      <!--<shader type = "GeomShader"   file = "GsShowNormal" />-->
      <shader type = "VertexShader" file = "simpleMesh.vertex(0:SkinAni)" />
      <shader type = "PixelShader"  file = "simpleMesh.pixel(0:simple.texture,simple.Lighting;)" />
   </Direct3D11>
  
</EvolShader>
