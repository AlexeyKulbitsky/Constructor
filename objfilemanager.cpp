#include "objfilemanager.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <QDebug>
#include <QMessageBox>

OBJFileManager::OBJFileManager(Model *model)
{
    this->model = model;
    this->file = NULL;
    this->element = NULL;
}

OBJFileManager::~OBJFileManager()
{

}

bool OBJFileManager::openFile(QString source)
{
    return true;
}

bool OBJFileManager::saveFile(QString source)
{
    return true;
}


void OBJFileManager::readVertexInfo()
{

}

void OBJFileManager::readIndexInfo()
{

}

float* OBJFileManager::calculateNormals(int a, int b, int c)
{
    float va[3], vb[3], vr[3], val;

    //va[0] = coord1[0] - coord2[0];
    va[0] = vertexArray[a * 3] - vertexArray[b * 3];
    //va[1] = coord1[1] - coord2[1];
    va[1] = vertexArray[a * 3 + 1] - vertexArray[b * 3 + 1];
    //va[2] = coord1[2] - coord2[2];
    va[2] = vertexArray[a * 3 + 2] - vertexArray[b * 3 + 2];

    //vb[0] = coord1[0] - coord3[0];
    vb[0] = vertexArray[a * 3] - vertexArray[c * 3];
    //vb[1] = coord1[1] - coord3[1];
    vb[1] = vertexArray[a * 3 + 1] - vertexArray[c * 3 + 1];
    //vb[2] = coord1[2] - coord3[2];
    vb[2] = vertexArray[a * 3 + 2] - vertexArray[c * 3 + 2];

    /* cross product */
    vr[0] = va[1] * vb[2] - vb[1] * va[2];
    vr[1] = vb[0] * va[2] - va[0] * vb[2];
    vr[2] = va[0] * vb[1] - vb[0] * va[1];

    /* normalization factor */
    val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );

    float norm[3];
    norm[0] = vr[0]/val;
    norm[1] = vr[1]/val;
    norm[2] = vr[2]/val;


    return norm;

}



bool OBJFileManager::loadOBJ(const char *folder, const char *filename, std::vector<Mesh*>& meshes, float velocity, float& scaleFactor, int axis)
{
    std::vector< vec3 > temp_vertices;
    std::vector< vec2 > temp_uvs;
    std::vector< vec3 > temp_normals;

    std::vector<MaterialInfo*> materials;
    //std::vector<Mesh*> meshes;
    char mtlSource[1024];
    char materialName[256];
    char fullPath[1024];
    strcpy(fullPath, folder);
    strcat(fullPath, filename);
    fileFolder = folder;
    strcpy(mtlSource, folder);
    Mesh *currentMesh = NULL;
    std::vector<Vertex> vertices_out;
    vertices_out.clear();
    bool hasNormals = false;
    bool hasTextures = false;
    float minX = 0.0f, maxX = 0.0f;
    float minZ = 0.0f, maxZ = 0.0f;
    float minY = 0.0f, maxY = 0.0f;
    FILE * file = fopen(fullPath, "r");
    if( file == NULL ){
        ////qDebug() << "Impossible to open the .obj file!";
        return false;
    }
    else
    {
        ////qDebug() << "File .obj successfully opened";
    }
    char buffer[256];
    while (!feof(file))
    {
        fgets(buffer, 256, file);

        if (buffer[0] == 'v' && buffer[1] == ' ')
        {
            hasTextures = false;
            hasNormals = false;
            vec3 vert(0.0f, 0.0f, 0.0f);
            sscanf(buffer, "v %f %f %f", &vert.x, &vert.y, &vert.z);
            if (minX > vert.x) minX = vert.x;
            else
                if (maxX < vert.x) maxX = vert.x;
            if (minY > vert.y) minY = vert.y;
            else
                if (maxY < vert.y) maxY = vert.y;
            if (minZ > vert.z) minZ = vert.z;
            else
                if (maxZ < vert.z) maxZ = vert.z;
            temp_vertices.push_back(vert);
        }
        else if (buffer[0] == 'v' && buffer[1] == 't')
        {
            hasTextures = true;
            vec2 text(0.0f, 0.0f);
            sscanf(buffer, "vt %f %f", &text.x, &text.y);
            temp_uvs.push_back(text);
        }
        else if (buffer[0] == 'v' && buffer[1] == 'n')
        {
            hasNormals = true;
            vec3 norm(0.0f, 0.0f, 0.0f);
            sscanf(buffer, "vn %f %f %f", &norm.x, &norm.y, &norm.z);
            temp_normals.push_back(norm);
        } if (buffer[0] == 'm' && buffer[1] == 't' && buffer[2] == 'l')
        {
            char buf[256];
            sscanf(buffer, "mtllib %s",buf);
            strcat(mtlSource, buf);
            readMtl(mtlSource,materials);
        }
        else if (buffer[0] == 'u' && buffer[1] == 's' && buffer[2] == 'e')
        {

            if (currentMesh != NULL)
                meshes.push_back(currentMesh);
            currentMesh = new Mesh();
            sscanf(buffer, "usemtl %s",materialName);
            if (materials.size() > 0)
                readUseMtl(materialName,materials,currentMesh);

        }
        else if (buffer[0] == 'f' && buffer[1] == ' ')
        {
            std::vector<char*> temp;
            char separator[] = " \n";
            char *pch = strtok (buffer, separator);
            pch = strtok (NULL, separator);
            while (pch != NULL)
            {
                temp.push_back(pch);
                pch = strtok (NULL, separator);
            }
            if (temp.size() > 3)
            {
                std::vector<char*> t;
                triangulate(temp, t);
                temp.clear();
                for (int i = 0; i < t.size(); ++i)
                    temp.push_back(t[i]);
            }
            if (!hasTextures && !hasNormals)
            {
                int a = 0;
                for (int i = 0; i < temp.size(); ++i)
                {
                    sscanf(temp[i],"%d", &a);
                    if (a < 0)
                        a = temp_vertices.size() - ( a*(-1) - 1);
                    Vertex vertex;
                    vertex.position[0] = temp_vertices[a - 1].x;
                    vertex.position[1] = temp_vertices[a - 1].y;
                    vertex.position[2] = temp_vertices[a - 1].z;
                    //vertices_out.push_back(vertex);

                    currentMesh->vertices.push_back(vertex);
                }

            }else if (hasTextures && !hasNormals)
            {
                int a = 0, b = 0;
                for (int i = 0; i < temp.size(); ++i)
                {
                    sscanf(temp[i],"%d/%d", &a, &b);
                    if (a < 0)
                        a = temp_vertices.size() - ( a*(-1) - 1);
                    if (b < 0)
                        b = temp_uvs.size() - ( b*(-1) - 1);
                    Vertex vertex;
                    vertex.position[0] = temp_vertices[a - 1].x;
                    vertex.position[1] = temp_vertices[a - 1].y;
                    vertex.position[2] = temp_vertices[a - 1].z;
                    vertex.texture[0] = temp_uvs[b - 1].x;
                    vertex.texture[1] = temp_uvs[b - 1].y;
                    //vertices_out.push_back(vertex);

                    currentMesh->vertices.push_back(vertex);
                }
            }else if (!hasTextures && hasNormals)
            {
                int a = 0, b = 0;
                for (int i = 0; i < temp.size(); ++i)
                {
                    sscanf(temp[i],"%d//%d", &a, &b);
                    if (a < 0)
                        a = temp_vertices.size() - ( a*(-1) - 1);
                    if (b < 0)
                        b = temp_normals.size() - ( b*(-1) - 1);
                    Vertex vertex;
                    vertex.position[0] = temp_vertices[a - 1].x;
                    vertex.position[1] = temp_vertices[a - 1].y;
                    vertex.position[2] = temp_vertices[a - 1].z;
                    vertex.normal[0] = temp_normals[b - 1].x;
                    vertex.normal[1] = temp_normals[b - 1].y;
                    vertex.normal[2] = temp_normals[b - 1].z;
                    //vertices_out.push_back(vertex);

                    currentMesh->vertices.push_back(vertex);
                }
            }else if (hasTextures && hasNormals)
            {
                int a = 0, b = 0, c = 0;
                for (int i = 0; i < temp.size(); ++i)
                {
                    sscanf(temp[i],"%d/%d/%d", &a, &b, &c);
                    if (a < 0)
                        a = temp_vertices.size() - ( a*(-1) - 1);
                    if (b < 0)
                        b = temp_uvs.size() - ( b*(-1) - 1);
                    if (c < 0)
                        c = temp_normals.size() - ( c*(-1) - 1);
                    Vertex vertex;
                    vertex.position[0] = temp_vertices[a - 1].x;
                    vertex.position[1] = temp_vertices[a - 1].y;
                    vertex.position[2] = temp_vertices[a - 1].z;
                    vertex.texture[0] = temp_uvs[b - 1].x;
                    vertex.texture[1] = temp_uvs[b - 1].y;
                    vertex.normal[0] = temp_normals[c - 1].x;
                    vertex.normal[1] = temp_normals[c - 1].y;
                    vertex.normal[2] = temp_normals[c - 1].z;
                    //vertices_out.push_back(vertex);

                    currentMesh->vertices.push_back(vertex);
                }
            }
        }else continue;
    }
    fclose(file);
    switch (axis)
    {
    case 1:
        scaleFactor = velocity / (maxX - minX);
        break;
    case 2:
        scaleFactor = velocity / (maxY - minY);
        break;
    case 3:
        scaleFactor = velocity / (maxZ - minZ);
        break;
    default:
        scaleFactor = 1.0f;
        break;
    }
    meshes.push_back(currentMesh);
}

bool OBJFileManager::loadOBJ(const char *folder, const char *filename, const char *textureName, std::vector<Mesh *> &meshes, float velocity, float &scaleFactor, int axis)
{
    this->textureName = textureName;
    loadOBJ(folder, filename, meshes, velocity, scaleFactor, axis);
}

bool OBJFileManager::loadOBJ(const QString& folder, const QString& filename, std::vector<Mesh *> &meshes, float velocity, float &scaleFactor, int axis)
{
    std::vector< vec3 > temp_vertices;
    std::vector< vec2 > temp_uvs;
    std::vector< vec3 > temp_normals;

    std::vector<MaterialInfo*> materials;
    QString mtlSource = folder;
    char materialName[256];
    QString fullPath = folder + filename;
    fileFolder = folder;
    Mesh *currentMesh = NULL;
    std::vector<Vertex> vertices_out;
    vertices_out.clear();
    bool hasNormals = false;
    bool hasTextures = false;
    float minX = 0.0f, maxX = 0.0f;
    float minZ = 0.0f, maxZ = 0.0f;
    float minY = 0.0f, maxY = 0.0f;
    QFile file(fullPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ////qDebug() << "Impossible to open the .obj file!";
        return false;
    }
    else
    {
        ////qDebug() << "File .obj successfully opened";
    }

    QTextStream in(&file);
    QString buffer = in.readLine();
    while (!buffer.isNull())
    {

        buffer = in.readLine();

        //fgets(buffer, 256, file);

        if (buffer[0] == 'v' && buffer[1] == ' ')
        {
            hasTextures = false;
            hasNormals = false;
            vec3 vert(0.0f, 0.0f, 0.0f);
            sscanf(buffer.toStdString().c_str(), "v %f %f %f", &vert.x, &vert.y, &vert.z);
            if (minX > vert.x) minX = vert.x;
            else
                if (maxX < vert.x) maxX = vert.x;
            if (minY > vert.y) minY = vert.y;
            else
                if (maxY < vert.y) maxY = vert.y;
            if (minZ > vert.z) minZ = vert.z;
            else
                if (maxZ < vert.z) maxZ = vert.z;
            temp_vertices.push_back(vert);
        }
        else if (buffer[0] == 'v' && buffer[1] == 't')
        {
            hasTextures = true;
            vec2 text(0.0f, 0.0f);
            sscanf(buffer.toStdString().c_str(), "vt %f %f", &text.x, &text.y);
            temp_uvs.push_back(text);
        }
        else if (buffer[0] == 'v' && buffer[1] == 'n')
        {
            hasNormals = true;
            vec3 norm(0.0f, 0.0f, 0.0f);
            sscanf(buffer.toStdString().c_str(), "vn %f %f %f", &norm.x, &norm.y, &norm.z);
            temp_normals.push_back(norm);
        } if (buffer[0] == 'm' && buffer[1] == 't' && buffer[2] == 'l')
        {
            char buf[256];
            sscanf(buffer.toStdString().c_str(), "mtllib %s",buf);
            //strcat(mtlSource, buf);
            mtlSource += buf;
            readMtl(mtlSource,materials);
        }
        else if (buffer[0] == 'u' && buffer[1] == 's' && buffer[2] == 'e')
        {

            if (currentMesh != NULL)
                meshes.push_back(currentMesh);
            currentMesh = new Mesh();
            sscanf(buffer.toStdString().c_str(), "usemtl %s",materialName);
            if (materials.size() > 0)
                readUseMtl(materialName,materials,currentMesh);

        }
        else if (buffer[0] == 'f' && buffer[1] == ' ')
        {
            std::vector<char*> temp;
            char separator[] = " \n";
            char t[1024];
            strcpy(t, buffer.toStdString().c_str());
            char *pch = strtok (t, separator);
            pch = strtok (NULL, separator);
            while (pch != NULL)
            {
                temp.push_back(pch);
                pch = strtok (NULL, separator);
            }
            if (temp.size() > 3)
            {
                std::vector<char*> t;
                triangulate(temp, t);
                temp.clear();
                for (int i = 0; i < t.size(); ++i)
                    temp.push_back(t[i]);
            }
            if (!hasTextures && !hasNormals)
            {
                int a = 0;
                for (int i = 0; i < temp.size(); ++i)
                {
                    sscanf(temp[i],"%d", &a);
                    if (a < 0)
                        a = temp_vertices.size() - ( a*(-1) - 1);
                    Vertex vertex;
                    vertex.position[0] = temp_vertices[a - 1].x;
                    vertex.position[1] = temp_vertices[a - 1].y;
                    vertex.position[2] = temp_vertices[a - 1].z;
                    //vertices_out.push_back(vertex);

                    currentMesh->vertices.push_back(vertex);
                }

            }else if (hasTextures && !hasNormals)
            {
                int a = 0, b = 0;
                for (int i = 0; i < temp.size(); ++i)
                {
                    sscanf(temp[i],"%d/%d", &a, &b);
                    if (a < 0)
                        a = temp_vertices.size() - ( a*(-1) - 1);
                    if (b < 0)
                        b = temp_uvs.size() - ( b*(-1) - 1);
                    Vertex vertex;
                    vertex.position[0] = temp_vertices[a - 1].x;
                    vertex.position[1] = temp_vertices[a - 1].y;
                    vertex.position[2] = temp_vertices[a - 1].z;
                    vertex.texture[0] = temp_uvs[b - 1].x;
                    vertex.texture[1] = temp_uvs[b - 1].y;
                    //vertices_out.push_back(vertex);

                    currentMesh->vertices.push_back(vertex);
                }
            }else if (!hasTextures && hasNormals)
            {
                int a = 0, b = 0;
                for (int i = 0; i < temp.size(); ++i)
                {
                    sscanf(temp[i],"%d//%d", &a, &b);
                    if (a < 0)
                        a = temp_vertices.size() - ( a*(-1) - 1);
                    if (b < 0)
                        b = temp_normals.size() - ( b*(-1) - 1);
                    Vertex vertex;
                    vertex.position[0] = temp_vertices[a - 1].x;
                    vertex.position[1] = temp_vertices[a - 1].y;
                    vertex.position[2] = temp_vertices[a - 1].z;
                    vertex.normal[0] = temp_normals[b - 1].x;
                    vertex.normal[1] = temp_normals[b - 1].y;
                    vertex.normal[2] = temp_normals[b - 1].z;
                    //vertices_out.push_back(vertex);

                    currentMesh->vertices.push_back(vertex);
                }
            }else if (hasTextures && hasNormals)
            {
                int a = 0, b = 0, c = 0;
                for (int i = 0; i < temp.size(); ++i)
                {
                    sscanf(temp[i],"%d/%d/%d", &a, &b, &c);
                    if (a < 0)
                        a = temp_vertices.size() - ( a*(-1) - 1);
                    if (b < 0)
                        b = temp_uvs.size() - ( b*(-1) - 1);
                    if (c < 0)
                        c = temp_normals.size() - ( c*(-1) - 1);
                    Vertex vertex;
                    vertex.position[0] = temp_vertices[a - 1].x;
                    vertex.position[1] = temp_vertices[a - 1].y;
                    vertex.position[2] = temp_vertices[a - 1].z;
                    vertex.texture[0] = temp_uvs[b - 1].x;
                    vertex.texture[1] = temp_uvs[b - 1].y;
                    vertex.normal[0] = temp_normals[c - 1].x;
                    vertex.normal[1] = temp_normals[c - 1].y;
                    vertex.normal[2] = temp_normals[c - 1].z;
                    //vertices_out.push_back(vertex);

                    currentMesh->vertices.push_back(vertex);
                }
            }
        }else continue;
    }
    file.close();

    qDebug() << filename;
    if (filename[0] == 'm' &&
            filename[1] == 'a' &&
            filename[2] == 'n')
    {
        scaleFactor = 1.75f / (maxZ - minZ);
    }
    else
    if (filename[0] == 'w' &&
            filename[1] == 'o' &&
            filename[2] == 'm' &&
            filename[3] == 'a' &&
            filename[4] == 'n')
    {
        scaleFactor = 1.6f / (maxZ - minZ);
    }
    else
    {
        switch (axis)
        {
        case 1:
            scaleFactor = velocity / (maxX - minX);
            break;
        case 2:
            scaleFactor = velocity / (maxY - minY);
            break;
        case 3:
            scaleFactor = velocity / (maxZ - minZ);
            break;
        default:
            scaleFactor = 1.0f;
            break;
        }
    }
    meshes.push_back(currentMesh);
}



void OBJFileManager::readVertex(char *line)
{
    float x, y, z;
    sscanf(line, "v %f %f %f", &x, &y, &z);
}

void OBJFileManager::readTexture(char *line)
{
    float u, v;
    sscanf(line, "vt %f %f", &u, &v);
}

void OBJFileManager::triangulate(std::vector<char *> in, std::vector<char *> &out)
{
    out.clear();
    for (int i = 2; i < in.size(); ++i)
    {
        char temp[256];
        strcpy(temp,in[0]);
        out.push_back(temp);
        strcpy(temp,in[i - 1]);
        out.push_back(temp);
        strcpy(temp,in[i]);
        out.push_back(temp);
    }

}

void OBJFileManager::readMtl(char *mtlSource, std::vector<MaterialInfo*>& materials)
{
    MaterialInfo *info = NULL;

    FILE *mtlFile = fopen(mtlSource, "r");
    if (mtlFile == NULL)
    {
        //qDebug() << "Impossible to open the .mtl file !\n";
        return;
    }
    else
    {
        //qDebug() << "File .mtl successfully opened";
    }
    char mtlBuffer[256];

    while (!feof(mtlFile))
    {
        fgets(mtlBuffer, 256, mtlFile);
        // Новый материал
        if (strstr(mtlBuffer, "newmtl") != NULL)
        {
            if (info != NULL)
                materials.push_back(info);
            info = new MaterialInfo();
            sscanf(mtlBuffer,"newmtl %s",info->name);
            ////qDebug() << "Reading " << info->name;
        }
        else if (mtlBuffer[0] == 'K' && mtlBuffer[1] == 'a')
        {
            float r, g, b;
            sscanf(mtlBuffer, "Ka  %f %f %f", &r, &g, &b);
            info->Ka[0] = r;
            info->Ka[1] = g;
            info->Ka[2] = b;
        }
        else if (mtlBuffer[0] == 'K' && mtlBuffer[1] == 'd')
        {
            float r, g, b;
            sscanf(mtlBuffer, "Kd  %f %f %f", &r, &g, &b);
            info->Kd[0] = r;
            info->Kd[1] = g;
            info->Kd[2] = b;
        }
        else if (mtlBuffer[0] == 'K' && mtlBuffer[1] == 's')
        {
            float r, g, b;
            sscanf(mtlBuffer, "Ks  %f %f %f", &r, &g, &b);
            info->Ks[0] = r;
            info->Ks[1] = g;
            info->Ks[2] = b;
        }
        else if (mtlBuffer[0] == 'd')
        {
            float d;
            sscanf(mtlBuffer, "d  %f", &d);
            info->d = d;
        }
        else if (mtlBuffer[0] == 'N' && mtlBuffer[1] == 's')
        {
            float Ns;
            sscanf(mtlBuffer, "Ns  %f", &Ns);
            info->Ns = Ns;
        }
        else if (strstr(mtlBuffer, "illum") != NULL)
        {
            int illum;
            sscanf(mtlBuffer, "illum %d", &illum);
            info->illum = illum;
        }
        else if (strstr(mtlBuffer, "map_Ka") != NULL)
        {
            sscanf(mtlBuffer, "map_Ka %s", info->map_Ka);
            if (!strcmp(info->map_Ka, "texture"))
                getTexture(fileFolder + textureName, info->Ka_ID);
            else
                getTexture(fileFolder + info->map_Ka, info->Ka_ID);
        }
        else if (strstr(mtlBuffer, "map_Kd") != NULL)
        {
            sscanf(mtlBuffer, "map_Kd %s", info->map_Kd);
            if (!strcmp(info->map_Kd, "texture"))
                getTexture(fileFolder + textureName, info->Ka_ID);
            else
                getTexture(fileFolder + info->map_Kd, info->Kd_ID);
        }
        else if (strstr(mtlBuffer, "map_Ks") != NULL)
        {
            sscanf(mtlBuffer, "map_Ks %s", info->map_Ks);
            if (!strcmp(info->map_Ks, "texture"))
                getTexture(fileFolder + textureName, info->Ka_ID);
            else
                getTexture(fileFolder + info->map_Ks, info->Ks_ID);
        }
    }
    materials.push_back(info);
    info = NULL;
    ////qDebug() << "Reading .mtl finished";
    fclose(mtlFile);
}

void OBJFileManager::readMtl(QString &mtlSource, std::vector<MaterialInfo *> &materials)
{
    MaterialInfo *info = NULL;

    QFile file(mtlSource);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ////qDebug() << "Impossible to open the .obj file!";

    }
    else
    {
        ////qDebug() << "File .obj successfully opened";
    }

    QTextStream in(&file);
    QString mtlBuffer = in.readLine();
    while (!mtlBuffer.isNull())
    {
        mtlBuffer = in.readLine();
        // Новый материал
        if (strstr(mtlBuffer.toStdString().c_str(), "newmtl") != NULL)
        {
            if (info != NULL)
                materials.push_back(info);
            info = new MaterialInfo();
            sscanf(mtlBuffer.toStdString().c_str(),"newmtl %s",info->name);
            ////qDebug() << "Reading " << info->name;
        }
        else if (mtlBuffer[0] == 'K' && mtlBuffer[1] == 'a')
        {
            float r, g, b;
            sscanf(mtlBuffer.toStdString().c_str(), "Ka  %f %f %f", &r, &g, &b);
            info->Ka[0] = r;
            info->Ka[1] = g;
            info->Ka[2] = b;
        }
        else if (mtlBuffer[0] == 'K' && mtlBuffer[1] == 'd')
        {
            float r, g, b;
            sscanf(mtlBuffer.toStdString().c_str(), "Kd  %f %f %f", &r, &g, &b);
            info->Kd[0] = r;
            info->Kd[1] = g;
            info->Kd[2] = b;
        }
        else if (mtlBuffer[0] == 'K' && mtlBuffer[1] == 's')
        {
            float r, g, b;
            sscanf(mtlBuffer.toStdString().c_str(), "Ks  %f %f %f", &r, &g, &b);
            info->Ks[0] = r;
            info->Ks[1] = g;
            info->Ks[2] = b;
        }
        else if (mtlBuffer[0] == 'd')
        {
            float d;
            sscanf(mtlBuffer.toStdString().c_str(), "d  %f", &d);
            info->d = d;
        }
        else if (mtlBuffer[0] == 'N' && mtlBuffer[1] == 's')
        {
            float Ns;
            sscanf(mtlBuffer.toStdString().c_str(), "Ns  %f", &Ns);
            info->Ns = Ns;
        }
        else if (strstr(mtlBuffer.toStdString().c_str(), "illum") != NULL)
        {
            int illum;
            sscanf(mtlBuffer.toStdString().c_str(), "illum %d", &illum);
            info->illum = illum;
        }
        else if (strstr(mtlBuffer.toStdString().c_str(), "map_Ka") != NULL)
        {
            sscanf(mtlBuffer.toStdString().c_str(), "map_Ka %s", info->map_Ka);
            if (!strcmp(info->map_Ka, "texture"))
                getTexture(fileFolder + textureName, info->Ka_ID);
            else
                getTexture(fileFolder + info->map_Ka, info->Ka_ID);
        }
        else if (strstr(mtlBuffer.toStdString().c_str(), "map_Kd") != NULL)
        {
            sscanf(mtlBuffer.toStdString().c_str(), "map_Kd %s", info->map_Kd);
            if (!strcmp(info->map_Kd, "texture"))
                getTexture(fileFolder + textureName, info->Ka_ID);
            else
                getTexture(fileFolder + info->map_Kd, info->Kd_ID);
        }
        else if (strstr(mtlBuffer.toStdString().c_str(), "map_Ks") != NULL)
        {
            sscanf(mtlBuffer.toStdString().c_str(), "map_Ks %s", info->map_Ks);
            if (!strcmp(info->map_Ks, "texture"))
                getTexture(fileFolder + textureName, info->Ka_ID);
            else
                getTexture(fileFolder + info->map_Ks, info->Ks_ID);
        }
    }
    materials.push_back(info);
    info = NULL;
    file.close();
}

void OBJFileManager::readUseMtl(char *materialName, std::vector<MaterialInfo *> &materials, Mesh *currentMesh)
{
    ////qDebug() << "Reading material: " << materialName;
    for (int i = 0; i < materials.size(); ++i)
    {
        if (!strcmp(materialName,materials[i]->name))
        {
            ////qDebug() << "Finded material" << materialName;
            currentMesh->Ka[0] = materials[i]->Ka[0];
            currentMesh->Ka[1] = materials[i]->Ka[1];
            currentMesh->Ka[2] = materials[i]->Ka[2];

            currentMesh->Ks[0] = materials[i]->Ks[0];
            currentMesh->Ks[1] = materials[i]->Ks[1];
            currentMesh->Ks[2] = materials[i]->Ks[2];

            currentMesh->Kd[0] = materials[i]->Kd[0];
            currentMesh->Kd[1] = materials[i]->Kd[1];
            currentMesh->Kd[2] = materials[i]->Kd[2];

            currentMesh->Ns = materials[i]->Ns;
            currentMesh->illum = materials[i]->illum;
            currentMesh->d = materials[i]->d;

            /*
            if (strlen(materials[i]->map_Ka) > 0)
            {
                strcpy(currentMesh->map_Ka, materials[i]->map_Ka);
                //qDebug() << materials[i]->map_Ka;
                getTexture(materials[i]->map_Ka, currentMesh->map_Ka_ID);
            }
            if (strlen(materials[i]->map_Ks) > 0)
            {
                strcpy(currentMesh->map_Ks, materials[i]->map_Ks);
                //qDebug() << materials[i]->map_Ks;
                getTexture(materials[i]->map_Ks, currentMesh->map_Ks_ID);
            }
            if (strlen(materials[i]->map_Kd) > 0)
            {
                strcpy(currentMesh->map_Kd, materials[i]->map_Kd);
                //qDebug() << "Texture KD ID" << materials[i]->map_Kd;
                getTexture(materials[i]->map_Kd, currentMesh->map_Kd_ID);
            }*/
            currentMesh->map_Ka_ID = materials[i]->Ka_ID;
            currentMesh->map_Kd_ID = materials[i]->Kd_ID;
            currentMesh->map_Ks_ID = materials[i]->Ks_ID;
            break;
            ////qDebug() << "Finished editing mesh: " << materialName;
        }
    }
    ////qDebug() << "Finished material";
}

void OBJFileManager::getTexture(QString textureSource, unsigned& textureID)
{
    QImage image1;
    if (!image1.load(QString(textureSource)))
    {
        textureID = 0;
        // int r = QMessageBox::warning(0,"Spreadsheet",
        //                                 "Unable to load texture: " + QString(textureSource),
        //                             QMessageBox::Yes | QMessageBox::No|
        //                              QMessageBox::Cancel);

        //qDebug() << "Unable to load texture from" << textureSource;
        return;
    }
    image1 = QGLWidget::convertToGLFormat(image1);
    glGenTextures(1, &textureID);
    // создаём и связываем 1-ый текстурный объект с последующим состоянием текстуры
    glBindTexture(GL_TEXTURE_2D, textureID);
    // связываем текстурный объект с изображением
    glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei)image1.width(), (GLsizei)image1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.bits());

    // задаём линейную фильтрацию вблизи:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // задаём линейную фильтрацию вдали:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // задаём: при фильтрации игнорируются тексели, выходящие за границу текстуры для s координаты
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // задаём: при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // задаём: цвет текселя полностью замещает цвет фрагмента фигуры
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    ////qDebug() << "Finished Loading tecture: " << textureSource;
}




