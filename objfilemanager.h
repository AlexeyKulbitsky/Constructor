#ifndef OBJFILEMANAGER_H
#define OBJFILEMANAGER_H

#include "filemanager.h"
#include "roadelementobj.h"

struct MaterialInfo
{
    char name[256];
    float Ka[3];
    float Kd[3];
    float Ks[3];
    float d; // Прозрачность 0..1
    float Ns; // Блеск
    int illum;

    char map_Ka[256];
    char map_Kd[256];
    char map_Ks[256];

    unsigned Ka_ID;
    unsigned Ks_ID;
    unsigned Kd_ID;
    MaterialInfo()
    {
        Ka[0] = Ka[1] = Ka[2] = 0.0f;
        Kd[0] = Kd[1] = Kd[2] = 0.0f;
        Ks[0] = Ks[1] = Ks[2] = 0.0f;
        d = 1.0f;
        Ns = 1.0f;
        illum = 1;
        Ka_ID = Ks_ID = Kd_ID = 0;
        map_Ka[0] = map_Ks[0] = map_Kd[0] = 0;
    }

};

class OBJFileManager: public FileManager
{
public:
    OBJFileManager(Model* model);

    // FileManager interface
public:
    virtual ~OBJFileManager();
    virtual bool openFile(QString);
    virtual bool saveFile(QString);
    void readVertexInfo();
    void readIndexInfo();
    float *calculateNormals(int a, int b, int c);

    bool loadOBJ(const char *folder, const char* filename,
                 std::vector<Mesh *> &meshes, float velocity, float &scaleFactor, int axis = 0);
    bool loadOBJ(const char *folder, const char* filename, const char* textureName,
                 std::vector<Mesh *> &meshes, float velocity, float &scaleFactor, int axis = 0);
    bool loadOBJ(const QString& folder, const QString& filename,
                 std::vector<Mesh *> &meshes, float velocity, float &scaleFactor, int axis = 0);


    void readVertex(char* line);
    void readTexture(char* line);
    void triangulate(std::vector<char *> in, std::vector<char*>& out);
public:
    RoadElementOBJ* element;
    FILE* file;

    QVector<float> vertexArray;
    QVector<float> textureArray;
    QVector<float> normalArray;
    QVector<GLubyte> indexArray;

    bool hasTexture;
    bool hasNormal;
    QString fileFolder;

    QString textureName;
private:
    inline void record(char *line);
    inline void readlVertices(char *line);
    inline void readTexturCord(char *line);
    inline void readNormals(char *line);
    void readSpaceVert(char *line);
    inline void readFace(char *line);
    inline void actionsFace(char *line);
    void readMtl(char *mtlSource, std::vector<MaterialInfo*>& materials);
    void readMtl(QString& mtlSource, std::vector<MaterialInfo*>& materials);
    void readUseMtl(char *materialName, std::vector<MaterialInfo*>& materials, Mesh *currentMesh);
    void getTexture(QString textureSource, unsigned &textureID);
    void recordverts();


private:
    /*
    std::vector<vec3> list_vertices;       // v ... ... ...
    std::vector<vec2> textur_coordinates; // vt
    std::vector<vec3> normals;                // vn
    std::vector<vec3> space_vertices;         // vp
    std::vector<std::string> mtl;                   // mtlib
    std::vector<std::string> usemtl;                // usemtl
    std::vector<Vertex> verts;
    std::vector<int> indices;                   // index
    */

};

#endif // OBJFILEMANAGER_H
