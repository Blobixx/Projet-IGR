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

uniform vec3 lightPos;
uniform vec3 matAlbedo;

varying vec4 P; // fragment-wise position
varying vec3 N; // fragment-wise normal

const float Li = 5.0f;
const float Pi = 3.14159265359f;
uniform float alpha; //rugosité

void main (void) {
    gl_FragColor = vec4 (0.0, 0.0, 0.0, 1.0);

    vec3 p = vec3 (gl_ModelViewMatrix * P);
    vec3 n = normalize (gl_NormalMatrix * N);
    vec3 l = normalize (lightPos - p);
    vec3 v = normalize (-p);

    // ---------- Code to change -------------

    vec3 wi = l;
    vec3 wCam = vec3(0,0,0);
    vec3 w0 = v;
    wi = normalize(wi);
    w0 = normalize(w0);

    vec3 wh = wi+v;
    wh = normalize(wh);

    float norwh = dot(n,wh);
    float norw0 = dot(n,w0);
    float norwi = max(0.0f, dot(n,wi));

    float fd = 1.0f/Pi;

    /*
     //-------------Modele de Blinn-Phong ----------------

    float ks = 0.1f;

    float fs = pow(norwh,5.0)*ks;

    */
    // ------------Model GGX ---------------
    float ap = 0.01f;
    float zero = 0.0f;
    float Dsup = pow(ap,2.0f);
    float Dinf = Pi*pow( 1.0f+(pow(ap,2.0f)-1.0f)*pow(norwh,2.0f) ,2);
    float D = Dsup/Dinf;
    float F = 0.02f + (1.0f-0.022f)*pow(  1.0f-max(zero,dot(wi,wh))  ,5);
    float G01 = 2.0f*norwi/(norwi+sqrt(pow(alpha,2)+(1.0f-pow(alpha,2))*pow(norwi,2)));
    float G02 = 2.0f*norw0/(norw0+sqrt(pow(alpha,2)+(1.0f-pow(alpha,2))*pow(norw0,2)));
    float G = G01*G02;

    float fs = D*F*G/(4.0f*norwh*norw0);

    float f = fs+fd;

    float L0 = Li*f*norwi;

    vec4 color = vec4 (0.38*L0,0.22*L0,0.4*L0, 0.0);

    gl_FragColor += color;
}
