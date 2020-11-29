attribute vec4 vPosition ;
attribute vec4 vNormal ;
attribute vec4 vTexCoord ;
attribute vec4 vTangent ;

uniform mat4 modelViewMatrix ;
uniform mat4 projectionMatrix ;
uniform mat3 normalMatrix ;

uniform vec4 lightPosition ;

varying vec3 fE ;
varying vec3 fL ;
varying vec2 fTexCoord ;

void main(void)
{
    vec3 bitangent = vTangent . w * cross ( vNormal , vTangent .
                                         xyz ) ;
     vec3 T = normalMatrix * vTangent . xyz ;
     vec3 B = normalMatrix * bitangent ;
     vec3 N = normalMatrix * vNormal ;
     mat3 TBN = mat3 (T.x , B.x , N.x ,
                     T .y , B .y , N .y ,
                     T .z , B .z , N . z );

     vec4 eyePosition = modelViewMatrix * vPosition ;
     fL = TBN * ( lightPosition . xyz - eyePosition . xyz ) ;

     fE = TBN * (- eyePosition . xyz );
     fTexCoord = vTexCoord ;
     gl_Position = projectionMatrix * eyePosition ;
}
