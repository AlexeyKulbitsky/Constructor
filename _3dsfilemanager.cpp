#include "_3dsfilemanager.h"
#include <io.h>
_3DsFileManager::_3DsFileManager()
{

}

_3DsFileManager::~_3DsFileManager()
{

}


bool _3DsFileManager::openFile(QString)
{
    return true;
}



bool _3DsFileManager::saveFile(QString)
{
    return true;
}

bool _3DsFileManager::load3DS(const char *folder, const char *filename, std::vector<Mesh_3DS *> &meshes, QMap<QString, Material> &materials)
{
    //Mesh_3DS* mesh = new Mesh_3DS();

    unsigned char ch;
    //unsigned short qty;
    //unsigned short faceFlags;

    fileFolder = folder;
    char fullPath[1024];
    strcpy(fullPath, folder);
    strcat(fullPath, filename);

    FILE * file = fopen(fullPath, "rb");
    if( file == NULL ){
        ////qDebug() << "Impossible to open the .obj file!";
        return false;
    }
    else
    {
        ////qDebug() << "File .obj successfully opened";
    }

    unsigned short int chunkID;          // ID chunk-а
    unsigned int chunkLength;

    while (ftell (file) < filelength (fileno(file))) //Loop to scan the whole file
    {

        fread (&chunkID, 2, 1, file); //Read the chunk header
        fread (&chunkLength, 4, 1, file);

        switch(chunkID)
        {
        case M3DMAGIC: // Проверка подлинности 3ds файла
            ////qDebug() << "M3D_MAGIC";
            break;
        case MDATA://3D EDITOR CHUNK
            ////qDebug() << "MDATA";
            break;
        case NAMED_OBJECT: // Имя объекта
        {
            ////qDebug() << "NAMED_OBJECT";
            Mesh_3DS *mesh = new Mesh_3DS();
            int i = 0;
            do
            {
                fread (&ch, 1, 1, file);
                mesh->name[i] = ch;
                i++;
            }while(ch != '\0' && i<20);
            ////qDebug() << "Object name: " << mesh->name << "\n";
            ////qDebug() << "Object size: " << chunkLength;
            if (readObject(file,mesh,chunkLength - 6))
                meshes.push_back(mesh);
            ////qDebug() << meshes.size();
        }
            break;

        case MAT_ENTRY:
        {
            ////qDebug() << "MAT_ENTRY";
            Material material;
            material.textureID = 0;
            if (readMaterial(file, material, chunkLength - 6))
                materials[material.name] = material;
        }
            break;

            /*
        case MATAMBIENT:	//Ambient Color
        {
            short red;
            short green;
            short blue;
            double redAmbient;
            double greenAmbient;
            double blueAmbient;

            fread (&red, sizeof (byte), 1, file);
            fread (&green, sizeof (byte), 1, file);
            fread (&blue, sizeof (byte), 1, file);

            redAmbient = (1/255)*red;
            greenAmbient = (1/255)*green;
            blueAmbient = (1/255)*blue;
            //qDebug() << "Ambient color: " << red << " " << green << " " << blue;
        }
            break;

        case MATDIFFUSE:	//Diffuse Color
        {
            short red;
            short green;
            short blue;
            double redDiffuse;
            double greenDiffuse;
            double blueDiffuse;

            fread (&red, sizeof (byte), 1, file);
            fread (&green, sizeof (byte), 1, file);
            fread (&blue, sizeof (byte), 1, file);

            redDiffuse = (1/255)*red;
            greenDiffuse = (1/255)*green;
            blueDiffuse = (1/255)*blue;
            //qDebug() << "Diffuse color: " << red << " " << green << " " << blue;

        }
            break;

        case MATSPECULAR:	//Specular Color
        {
            short red;
            short green;
            short blue;
            double redSpecular;
            double greenSpecular;
            double blueSpecular;

            fread (&red, sizeof (byte), 1, file);
            fread (&green, sizeof (byte), 1, file);
            fread (&blue, sizeof (byte), 1, file);

            redSpecular = (1/255)*red;
            greenSpecular = (1/255)*green;
            blueSpecular = (1/255)*blue;

            //qDebug() << "Specular color: " << red << " " << green << " " << blue;
        }
            break;
        */

            /*
        case 0xA354:	//V Scale
        {
        fread (&vscale, sizeof (float), 1, bin3ds);
        }

        case 0xA356:	//U Scale
        {
        fread (&uscale, sizeof (float), 1, bin3ds);
        }

        case 0xA358:	//U Offset
        {
        fread (&uoffset, sizeof (float), 1, bin3ds);
        }

        case 0xA35A:	//V Offset
        {
        fread (&voffset, sizeof (float), 1, bin3ds);
        }

        case 0xA35C:	//Rotation Angle
        {
        fread (&rotationangle, sizeof (float), 1, bin3ds);
        }
            */



            /*
        case 0xafff:
            //ProcessMaterialChunk( &buffer , i );
            break;
        case 0xa000:
            //exit( 0 );
            //ProcessMaterialNameChunk( &buffer , i );
            break;
            */
        default:
            fseek(file, chunkLength-6, SEEK_CUR);
            break;
        }
    }
    fclose(file);
    return true;
}

int _3DsFileManager::getTextures(QString textureSource)
{
    QImage image1;
    GLuint textureID;
    if (!image1.load(textureSource))
        return -1;
    image1 = QGLWidget::convertToGLFormat(image1);
    glGenTextures(1, &textureID);
    if (textureID <= 0)
        return -1;
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
    return textureID;
}

bool _3DsFileManager::readChunk(FILE *file)
{
    if (fread (&chunk.ID, 2, 1, file) < 1) //Read the chunk header
        return false;
    if (fread (&chunk.length, 4, 1, file) < 1) //Read the length of the chunk
        return false;
    return true;
}

bool _3DsFileManager::readMaterial(FILE *file, Material &material, unsigned int bytesRead)
{
    unsigned short int chunkID;          // ID chunk-а
    unsigned int chunkLength;

    unsigned int readed = 0;
    while (readed < bytesRead)
    {
        fread (&chunkID, 2, 1, file); //Read the chunk header
        fread (&chunkLength, 4, 1, file);

        switch (chunkID)
        {
        case MAT_NAME: //Material Name
        {
            int i = 0;
            char ch;
            char materialname[256];
            while ((ch = fgetc (file)) != 0)
            {
                materialname [i] = ch;
                i++;
            }
            materialname [i] = '\0';
            material.name = materialname;
            ////qDebug() << "Material name: " << materialname;
            readed += chunkLength;
        }
            break;
        case MAT_AMBIENT: // цвет
        {
            readed += chunkLength;
            fread (&chunkID, 2, 1, file); //Read the chunk header
            fread (&chunkLength, 4, 1, file);
            unsigned char r, g, b;
            fread(&r, 1, 1, file);
            fread(&g, 1, 1, file);
            fread(&b, 1, 1, file);
            material.ambientColor[0] = (float)r/256.0f;
            material.ambientColor[1] = (float)g/256.0f;
            material.ambientColor[2] = (float)b/256.0f;
        }
            break;

        case MAT_DIFFUSE: // цвет
        {
            readed += chunkLength;
            fread (&chunkID, 2, 1, file); //Read the chunk header
            fread (&chunkLength, 4, 1, file);
            unsigned char r, g, b;
            fread(&r, 1, 1, file);
            fread(&g, 1, 1, file);
            fread(&b, 1, 1, file);
            material.diffuseColor[0] = (float)r/256.0f;
            material.diffuseColor[1] = (float)g/256.0f;
            material.diffuseColor[2] = (float)b/256.0f;
        }
            break;

        case MAT_SPECULAR: // цвет
        {
            readed += chunkLength;
            fread (&chunkID, 2, 1, file); //Read the chunk header
            fread (&chunkLength, 4, 1, file);
            unsigned char r, g, b;
            fread(&r, 1, 1, file);
            fread(&g, 1, 1, file);
            fread(&b, 1, 1, file);
            material.specularColor[0] = (float)r/256.0f;
            material.specularColor[1] = (float)g/256.0f;
            material.specularColor[2] = (float)b/256.0f;
        }
            break;

        case MAP_TEXTMAP:
            ////qDebug() << "MAP_TEXTMAP";
            readMaterial(file, material, chunkLength - 6);
            readed += chunkLength;
            break;
        case MAT_MAPNAME:
        {
            ////qDebug() << "MAT_MAPNAME";
            char c;
            char text[256];
            int i = 0;
            do
            {
                fread (&c, 1, 1, file);
                text[i] = c;
                i++;
            }while(c != '\0');
            ////qDebug() << "Steps: " << i;
            material.textureSource = text;
            //if (!RoadElement::texturesPool.contains(material.textureSource))
            //    RoadElement::texturesPool[material.textureSource] = getTextures(fileFolder + "/" + material.textureSource);
            //material.textureID = RoadElement::texturesPool[material.textureSource];
            material.textureID = getTextures(QString(fileFolder + material.textureSource));
            ////qDebug() << "Texture: " << text;
            readed += chunkLength;
        }
            break;
        /*
        case MAT_MAP_USCALE:
            //qDebug() << "MAT_MAP_USCALE";
            readed += chunk.length;
            break;
        case MAT_MAP_VSCALE:
            //qDebug() << "MAT_MAP_VSCALE";
            readed += chunk.length;
            break;
        case MAT_MAP_UOFFSET:
            //qDebug() << "MAT_MAP_UOFFSET";
            readed += chunk.length;
            break;
        case MAT_MAP_ANG:
            //qDebug() << "MAT_MAP_ANG";
            readed += chunk.length;
            break;
        case MAT_MAP_TILING:
            //qDebug() << "MAT_MAP_TILING";
            readed += chunk.length;
            break;
        */
        default:
            fseek(file, chunkLength-6, SEEK_CUR);
            readed += chunkLength;
            break;
        }
    }

    return true;
}

bool _3DsFileManager::readObject(FILE *file, Mesh_3DS *mesh, unsigned int bytesRead)
{
    //unsigned char ch;
    unsigned short qty;
    //unsigned short faceFlags;

    unsigned short int chunkID;          // ID chunk-а
    unsigned int chunkLength;

    unsigned int readed = 0;
    while (readed < bytesRead)
    {
        fread (&chunkID, 2, 1, file); //Read the chunk header
        fread (&chunkLength, 4, 1, file);

        switch (chunkID)
        {
        case N_TRI_OBJECT:
            readObject(file,mesh,chunkLength - 6);
            readed += chunkLength;
            break;

        case POINT_ARRAY: // Список вершин меша
        {
            ////qDebug() << "Reading vertices";
            fread (&qty, 2, 1, file);
            ////qDebug() << "Number of vertices: " << qty;
            float x, y, z;
            for (int i = 0; i < qty; i++)
            {
                fread (&x, sizeof(float), 1, file);
                fread (&y, sizeof(float), 1, file);
                fread (&z, sizeof(float), 1, file);
                mesh->vertices.push_back(x);
                mesh->vertices.push_back(y);
                mesh->vertices.push_back(z);

                mesh->colors.push_back(0.0f);
                mesh->colors.push_back(0.0f);
                mesh->colors.push_back(0.0f);
            }
            readed += chunkLength;
        }
            break;

        case TEXT_VERTS: // Текстурные координаты
        {
            ////qDebug() << "Reading textures";

            fread (&qty, sizeof (unsigned short), 1, file);
            ////qDebug() << "Number of texture groups" << qty;
            float u, v;
            for (int i = 0; i < qty; i++)
            {
                fread (&u, sizeof (float), 1, file);
                fread (&v, sizeof (float), 1, file);
                mesh->textures.push_back(u);
                mesh->textures.push_back(v);
            }
            readed += chunkLength;
        }
            break;

        case FACE_ARRAY: // Читаем фейсы
        {
            unsigned int readedBytes = 0;
            ////qDebug() << "Reading faces";
            fread (&qty, sizeof (unsigned short), 1, file);
            readedBytes += sizeof (unsigned short);
            ////qDebug() << "Number of faces"<< qty;
            unsigned short v1, v2, v3;
            unsigned short flags;
            for (int i = 0; i < qty; i++)
            {
                fread (&v1, sizeof (unsigned short), 1, file);
                fread (&v2, sizeof (unsigned short), 1, file);
                fread (&v3, sizeof (unsigned short), 1, file);
                fread (&flags, sizeof (unsigned short), 1, file);
                mesh->faces.push_back(v1);
                mesh->faces.push_back(v2);
                mesh->faces.push_back(v3);
                readedBytes += (sizeof (unsigned short)) * 4;
            }
            readObject(file,mesh,chunkLength - 6 - readedBytes);
            readed += chunkLength;
        }
            break;
        case MSH_MAT_GROUP:
        {
            char ch;
            char matName[256];
            int i = 0;
            do
            {
                fread(&ch, 1, 1, file);
                matName[i]=ch;
                i++;
            }while(ch != '\0' && i<20);
            ////qDebug() << "Material name: " << matName;
            MaterialMesh matMesh;
            matMesh.materialName = matName;
            fread (&qty, sizeof (unsigned short), 1, file);
            unsigned short tmp;
            for (i=0; i< qty; i++)
            {
                fread (&tmp, sizeof (unsigned short), 1, file);
                matMesh.faces.push_back(mesh->faces[tmp * 3]);
                matMesh.faces.push_back(mesh->faces[tmp * 3 + 1]);
                matMesh.faces.push_back(mesh->faces[tmp * 3 + 2]);

                ////qDebug() << "Polygone number: " << tmp;
            }
            mesh->materialMeshes.push_back(matMesh);
            ////qDebug() << "Number of faces, that use material: " << qty;
            readed += chunkLength;
        }
            break;
        case SMOOTH_GROUP:
            ////qDebug() << "SMOOTH GROUP (size): " << chunkLength;
            fseek(file, chunkLength-6, SEEK_CUR);
            readed += chunkLength;
            break;
        default:
            fseek(file, chunkLength-6, SEEK_CUR);
            readed += chunkLength;
            break;
        }


    }
    return true;
}
