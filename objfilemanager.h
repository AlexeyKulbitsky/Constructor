#ifndef OBJFILEMANAGER_H
#define OBJFILEMANAGER_H

#include "filemanager.h"
#include "roadelementobj.h"

struct MaterialInfo
{
    char name[256];
    float Ka[3] = {0.0f, 0.0f, 0.0f};
    float Kd[3] = {0.0f, 0.0f, 0.0f};
    float Ks[3] = {0.0f, 0.0f, 0.0f};
    float d = 1.0f; // Прозрачность 0..1
    float Ns = 1.0f; // Блеск
    int illum = 1;

    char map_Ka[256] = {0};
    char map_Kd[256] = {0};
    char map_Ks[256] = {0};

    unsigned Ka_ID = 0;
    unsigned Ks_ID = 0;
    unsigned Kd_ID = 0;

};

class OBJFileManager: public FileManager
{
public:
    OBJFileManager(Model* model);

    // FileManager interface
public:
    virtual ~OBJFileManager();
    virtual bool openFile(QString source);
    virtual bool saveFile(QString source);
    void readVertexInfo();
    void readIndexInfo();
    float *calculateNormals(int a, int b, int c);

    bool loadOBJ(const char *folder, const char* filename,
                 std::vector<Mesh *> &meshes, float width, float &scaleFactor);


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

private:
    inline void record(char *line);
    inline void readlVertices(char *line);
    inline void readTexturCord(char *line);
    inline void readNormals(char *line);
    void readSpaceVert(char *line);
    inline void readFace(char *line);
    inline void actionsFace(char *line);
    void readMtl(char *mtlSource, std::vector<MaterialInfo*>& materials);
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
