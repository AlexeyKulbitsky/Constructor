#ifndef _3DSFILEMANAGER_H
#define _3DSFILEMANAGER_H

#include "roadelement3d.h"

#include "filemanager.h"
#include "roadelement.h"

//>------ Primary Chunk, at the beginning of each file
#define PRIMARY       0x4D4D

//>------ Main Chunks
#define OBJECTINFO    0x3D3D				// This gives the version of the mesh and is found right before the material and object information
#define VERSION       0x0002				// This gives the version of the .3ds file
#define EDITKEYFRAME  0xB000				// This is the header for all of the key frame info

//>------ sub defines of OBJECTINFO
#define MATERIAL	  0xAFFF				// This stored the texture info
#define OBJECT		  0x4000				// This stores the faces, vertices, etc...

//>------ sub defines of MATERIAL
#define MATNAME       0xA000
#define MATAMBIENT    0xA010
#define MATDIFFUSE    0xA020
#define MATSPECULAR   0xA030
#define MATSHININESS  0xA040                // This holds the color of the object/material
#define MATMAP        0xA200				// This is a header for a new material
#define MATMAPFILE    0xA300				// This holds the file name of the texture

#define OBJECT_MESH   0x4100				// This lets us know that we are reading a new object

//>------ sub defines of OBJECT_MESH
#define OBJECT_VERTICES     0x4110			// The objects vertices
#define OBJECT_FACES		0x4120			// The objects faces
#define OBJECT_MATERIAL		0x4130			// This is found if the object has a material, either texture map or color
#define OBJECT_UV			0x4140			// The UV texture coordinates



#define     M3DMAGIC                            0x4D4D
#define         M3DVERSION                      0x2     // size 10
#define         MDATA                           0x3D3D  //
#define             AMBIENT_LIGHT               0x2100  // size 15
#define                 COLOR_24                0x11    // size 9
                            // R G B (1/1/1 byte) value
#define             MAT_ENTRY                   0xAFFF
#define                 MAT_NAME                0xA000  // size 11
                            // material name
#define                 MAT_AMBIENT             0xA010  // size 15
                            // COLOR_24 chunk
#define                 MAT_DIFFUSE             0xA020  // size 15
                            // COLOR_24 chunk
#define                 MAT_SPECULAR            0xA030  // size 15
                            // COLOR_24 chunk
#define                 MAT_SHINNESS            0xA040  // size 14
#define                     INT_PERCENTAGE      0x30    // size 8
                                // percentage value
#define                 SHIN2PCT                0xA041  // size 14
                            // INT_PERCENTAGE chunk
#define                 MAT_TRANSPARENCY        0xA050  // size 14
                            // INT_PERCENTAGE chunk
#define                 MAT_XPFALL              0xA052  // size 14
                            // INT_PERCENTAGE chunk
#define                 MAT_REFBLUR             0xA053  // size 14
                            // INT_PERCENTAGE chunk
#define                 MAT_SHADING             0xA100  // size 8
                            // Shading value 1,2,3,...
#define                 MAT_SELF_ILPCT          0xA084  // size 14
                            // INT_PERCENTAGE chunk
#define                 MAP_TEXTMAP             0xA200
                            // INT_PERCENTAGE chunk
#define                     MAT_MAPNAME         0xA300  // size 17
                                // map name
#define                     MAT_MAP_USCALE      0xA354  // size 10
                                // scale value 1.00 for example
#define                     MAT_MAP_VSCALE      0xA356  // size 10
                                // scale value 1.00 for example
#define                     MAT_MAP_UOFFSET     0xA358  // size 10
                            // MAT_MAP_UOFFSET chunk
                                // offset value 0.00 for example
#define                     MAT_MAP_ANG         0xA35C  // size 10
                                // rotation angle 0.00 for example
#define                     MAT_MAP_TILING      0xA351  // size 8
                                // tiling flag
#define                 MAP_BUMPMAP             0xA230
                            // INT_PERCENTAGE chunk
#define                     MAT_MAPNAME         0xA300  // size 17
                                // map name
#define                     MAT_BUMP_PERCENT    0xA252  // size 8
                                // percent value
#define                     MAT_MAP_USCALE      0xA354  // size 10
                                // scale value 1.00 for example
#define                     MAT_MAP_VSCALE      0xA356  // size 10
                                // scale value 1.00 for example
#define                     MAT_MAP_UOFFSET     0xA358  // size 10
                            // MAT_MAP_UOFFSET chunk
                                // offset value 0.00 for example
#define                     MAT_MAP_ANG         0xA35C  // size 10
                                // rotation angle 0.00 for example
#define                     MAT_MAP_TILING      0xA351  // size 8
                                // tiling flag
#define                 MAP_SPECMAP             0xA204
                            // INT_PERCENTAGE chunk
#define                     MAT_MAPNAME         0xA300  // size 17
                                // map name
#define                     MAT_MAP_USCALE      0xA354  // size 10
                                // scale value 1.00 for example
#define                     MAT_MAP_VSCALE      0xA356  // size 10
                                // scale value 1.00 for example
#define                     MAT_MAP_UOFFSET     0xA358  // size 10
                            // MAT_MAP_UOFFSET chunk
                                // offset value 0.00 for example
#define                     MAT_MAP_ANG         0xA35C  // size 10
                                // rotation angle 0.00 for example
#define                     MAT_MAP_TILING      0xA351  // size 8
                                // tiling flag
                    // Another material
                    //.................
#define             NAMED_OBJECT                0x4000
                        // object name
#define                 N_TRI_OBJECT            0x4100
#define                     POINT_ARRAY         0x4110
                                // number of vertices
                                // vertices
#define                     MESH_MATRIX         0x4160 // size 54
                                // 1.0 1.0 1.0
                                // 1.0 1.0 1.0
                                // 1.0 1.0 1.0
                                // 1.0 1.0 1.0
#define                     TEXT_VERTS          0x4140
                                // number of vertices
                                // vertices
#define                     FACE_ARRAY         0x4120
                                // number of faces
                                // faces
#define                         MSH_MAT_GROUP   0x4130
                                    // material name
                                    // faces connected to material
                                // Another MSH_MAT_GROUP chunk
                                //.....................
#define                     SMOOTH_GROUP        0x4150

struct Chunk
{
    unsigned short int ID;          // ID chunk-а
    unsigned int length;            // Длинна chunk-а
    unsigned int bytesRead;         // Число читаемых байт для этого chunk-а
};




class _3DsFileManager: public FileManager
{

public:
    _3DsFileManager();
    ~_3DsFileManager();
    // FileManager interface
public:
    virtual bool openFile(QString source);
    virtual bool saveFile(QString source);
    bool load3DS(const char *folder, const char *filename, std::vector<Mesh_3DS *> &meshes, QMap<QString, Material> &materials);
private:
    int getTextures(QString textureSource);
    bool readChunk(FILE* file);
    bool readMaterial(FILE* file, Material &material, unsigned int bytesRead);
    bool readObject(FILE* file, Mesh_3DS* mesh, unsigned int bytesRead);

private:
    Chunk chunk;
    QString fileFolder;
    QVector<Material> materials;

};

#endif // _3DSFILEMANAGER_H
