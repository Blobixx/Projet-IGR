//Derni�re mise � jour le 09/01/2011

#ifndef __OBJ_LIB__
#define __OBJ_LIB__

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

std::string doubleSlash(std::string s);
std::string remplacerSlash(std::string s);
std::vector<std::string> splitSpace(std::string s);
std::string get_directory(std::string s);
float* vector2float(std::vector<float>& tableau);

class FloatVector
{
    /*
    Classe FloatVector : simple vecteur XYZ ou XYZA (dans le cas de couleurs).
    */
    public:
        FloatVector(float px=0,float py=0,float pz=0,float pa=0);
           //Constructeur

        ~FloatVector();

        FloatVector operator=(const FloatVector &fv);

        float x,y,z,a;
};

class MeshObj
{
    public:
        MeshObj(std::string,MeshObj *first=NULL);
        /* MeshObj(std::string,MeshObj *first=NULL);
           Constructeur, prend en arguments le nom du mod�le � charger et le pointeur de la premi�re frame si le mod�le appartient � une animation (sinon laissez-le � NULL).
        */
        ~MeshObj();
        /* ~MeshObj();
           Destructeur, lib�re toute la m�moire qui lui a �t� allou�e.
        */
        void charger_obj(std::string,MeshObj *first=NULL);
        /* void charger_obj(std::string,MeshObj *first=NULL);
           Charge un fichier OBJ et son MTL, prend en arguments le nom du mod�le � charger et le pointeur de la premi�re frame si le mod�le appartient � une animation (sinon laissez-le � NULL). Cette fonction est appel�e par le constructeur.
           Aucune valeur de retour.
        */
        void charger_mtl(std::string);
        /* void charger_mtl(std::string);
           Charge un fichier MTL, prend en argument le nom du fichier � charger. Cette fonction est appel�e par charger_obj.
           Aucune valeur de retour.
        */
        void draw_model(bool nor=true,bool tex=false);
        /* void draw_model(bool nor=true,bool tex=false);
           Dessine le mod�le, prend en arguments deux bool�ens repr�sentant respectivement les normales et la texture. Si nor vaut true alors on prend en compte les normales, et si tex vaut true alors on applique la texture.
           Aucune valeur de retour.
        */
        void setMaterialsAndTex(std::vector<Material*> mats,GLuint tex);
        /* void setMaterialsAndTex(std::vector<Material*> mats,GLuint tex);
           D�finit directement les mat�riaux et la texture du mod�le, prend en arguments un vector<Material*> et la texture. Cette fonction est appel�e par giveMaterialsAndTex.
           Aucune valeur de retour.
        */
        void giveMaterialsAndTex(MeshObj *target);
        /* void giveMaterialsAndTex(MeshObj *target);
           Modifie les mat�riaux et la texture de target en les rempla�ant par ses propres mat�riaux et sa texture. Cette fonction est appel�e par charger_obj uniquement lorsque first!=NULL.
           Aucune valeur de retour.
        */
    private:
        GLuint texture;
        int n_data;
        float *vertice,*normals,*textures,*colours;

        std::vector<Material*> materiaux;
};

class AnimMesh
{
    /*
    Classe AnimMesh : animation, contient chaque frame sous forme de mod�le statique.
    */
    public:
        AnimMesh(int anframes,std::string name);
        /* AnimMesh(int anframes,std::string name);
           Constructeur, prend pour arguments le nombre de frames et le nom d'une frame sans son num�ro et sans son extension (par exemple si c'est "personnage_000001.obj" on ne mettra que "personnage").
        */
        ~AnimMesh();
        /* ~AnimMesh();
           Destructeur, lib�re toutes les frames.
        */
        void draw(int fr,bool nor=true,bool tex=false);
        /* void draw(int fr,bool nor=true,bool tex=false);
           Dessine la frame fr, les deux autres arguments sont les m�mes que ceux de la fonction MeshObj::draw_model.
           Aucune valeur de retour.
        */
    private:
        int nframes;
        std::vector<MeshObj*> frames;
};

class VirtualAnim
{
    /*
    Class VirtualAnim : animation virtuelle stock�e uniquement sous la forme d'entiers.
    */
    public:
        VirtualAnim();
        /* VirtualAnim();
           Constructeur, met les attributs � une valeur par d�faut.
        */
        ~VirtualAnim();
        /* ~VirtualAnim();
           Destructeur, totalement inutile.
        */
        void start(int ab,int ae,int afps=25,bool back2beginning=false,int an_cycles=-1);
        /* void start(int ab,int ae,int afps=25,bool back2beginning=false,int an_cycles=-1);
           D�marre l'animation, prend pour arguments respectivement la frame de d�part, la frame de fin, le nombre de frames par seconde, un bool�en indiquant si une fois arriv� � la frame de fin on retourne � la frame 0 ou � la frame de d�but, et enfin le nombre de fois que cette animation doit �tre ex�cut�e (-1 pour jouer l'animation en boucle).
           Aucune valeur de retour.
        */
        void stop();
        /* void stop();
           Arr�te l'animation (la met en pause).
           Aucune valeur de retour.
        */
        void draw(AnimMesh *mesh,bool nor=true,bool tex=false);
        /* void draw(AnimMesh *mesh,bool nor=true,bool tex=false);
           Dessine la frame de mesh correspondant � la frame courante, les deux derniers arguments �tant les m�mes que ceux de MeshObj::draw_model.
           Aucune valeur de retour.
        */
    private:
        int b,e,fps,fr,n_cycles,cycles;
        unsigned int tps,inter;
        bool back,playing;
};

#endif
