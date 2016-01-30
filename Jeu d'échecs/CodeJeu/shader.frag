// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

// Add here all the value you need to describe the light or the material.
// At first used const values.
// Then, use uniform variables and set them from the CPU program.

uniform vec3 lightPos1 ;
uniform vec3 lightPos2 ;
uniform vec3 lightPos3 ;

uniform vec3 matAlbedo ;
uniform float alpha ;

const float Pi = 3.14f ;

varying vec4 P; // fragment-wise position
varying vec3 N; // fragment-wise normal

void main (void) {
    gl_FragColor = vec4 (0.0, 0.0, 0.0, 1.0);

    vec3 p = vec3 (gl_ModelViewMatrix * P);
    vec3 n = normalize (gl_NormalMatrix * N);
    vec3 l1 = normalize (lightPos1 - p);
    vec3 l2 = normalize (lightPos2 - p);
    vec3 l3 = normalize (lightPos3 - p);
    vec3 v = normalize (-p);

    // ---------- Bling-Phong -------------

    vec3 wh1 = normalize(v + l1) ;
    vec3 wh2 = normalize(v + l2) ;
    vec3 wh3 = normalize(v + l3) ;

    //vec3 ks = vec3(0.3,0.3,0.3);
    //float s = 13.0f ;
    vec3 L1 = vec3(0.2f,0.6f,0.7f) ;
    float L2 = 2.0f ;
    vec3 L3 = vec3(0.7f,0.7f,0.7f) ;


    vec3 fd = matAlbedo/3.14 ;
    //vec3 fs = ks*pow(dot(n,wh),s);
    //vec3 f = fs + fd ;

    //vec3 vecFinal = f*(L0*dot(n,l)) ;

    //vec4 color = vec4 (vecFinal, 1.0);
    // ----------------------------------------

    float F0 = 0.04f ;
    float GGGX2 = (dot(n,v)*2.0f)/(dot(n,v)+sqrt(pow(alpha,2)+(1.0f-pow(alpha,2))*(pow(dot(n,v),2))))  ;

    // ---------- GGX pour la source 1-------------

    float Fresnel1 = F0 + (1.0f-F0)*pow(1.0f-max(0.0f,dot(l1,wh1)),5.0f) ;

    float DGGX1 = pow(alpha,2)/(Pi*pow(1.0f+(pow(alpha,2)-1.0f)*pow(dot(n,wh1),2),2)) ;

    float GGGX1_L1 = (dot(n,l1)*2.0f)/(dot(n,l1)+sqrt(pow(alpha,2.0f)+(1.0f-pow(alpha,2))*(pow(dot(n,l1),2))))  ;
    float GGGX1 = GGGX1_L1*GGGX2 ;

    float fs_GGX11 = (DGGX1*Fresnel1*GGGX1)/(dot(n,l1)*dot(n,v)*4.0f) ;
    vec3 f1 = fs_GGX11 + fd ;

    vec3 vecFinal1 = f1*L1*dot(n,l1) ;

    vec4 color1 = vec4 (vecFinal1, 1.0);

    // ----------------------------------------

    // ---------- GGX pour la source 2-------------

    float Fresnel2 = F0 + (1.0f-F0)*pow(1.0f-max(0.0f,dot(l2,wh2)),5.0f) ;

    float DGGX2 = pow(alpha,2)/(Pi*pow(1.0f+(pow(alpha,2)-1.0f)*pow(dot(n,wh2),2),2)) ;

    float GGGX1_L2 = (dot(n,l2)*2.0f)/(dot(n,l2)+sqrt(pow(alpha,2.0f)+(1.0f-pow(alpha,2))*(pow(dot(n,l2),2))))  ;
    float GGGX2_2 = GGGX1_L2*GGGX2 ;

    float fs_GGX12 = (DGGX2*Fresnel2*GGGX2_2)/(dot(n,l2)*dot(n,v)*4.0f) ;
    vec3 f2 = fs_GGX12 + fd ;

    vec3 vecFinal2 = f2*L2*dot(n,l2) ;

    vec4 color2 = vec4 (vecFinal2, 1.0);

    // ----------------------------------------

    // ---------- GGX pour la source 3-------------

    float Fresnel3 = F0 + (1.0f-F0)*pow(1.0f-max(0.0f,dot(l3,wh3)),5.0f) ;

    float DGGX3 = pow(alpha,2)/(Pi*pow(1.0f+(pow(alpha,2)-1.0f)*pow(dot(n,wh3),2),2)) ;

    float GGGX1_L3 = (dot(n,l3)*2.0f)/(dot(n,l3)+sqrt(pow(alpha,2.0f)+(1.0f-pow(alpha,2))*(pow(dot(n,l3),2))))  ;
    float GGGX3 = GGGX1_L3*GGGX2 ;

    float fs_GGX13 = (DGGX3*Fresnel3*GGGX3)/(dot(n,l3)*dot(n,v)*4.0f) ;
    vec3 f3 = fs_GGX13 + fd ;

    vec3 vecFinal3 = f3*L3*dot(n,l3) ;

    vec4 color3 = vec4 (vecFinal3, 1.0);

// ----------------------------------------


    gl_FragColor = color1 + color2 + color3 ;
}
