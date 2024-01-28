#include <glad/glad.h>
#define  GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <C:\Users\Diana\source\repos\opengl\stb_image.h>


#define _USE_MATH_DEFINES 
#include <math.h>


float A = 0.7, N = 50;

int keyNum = 0;
int counter = 0;
int viewCounter = 0;

float theta = 20;

float vert[] = { 1,1,0,  1,-1,0,  -1,-1,0,  -1,1,0 };


//угол обзора для камеры
float xAlfa = 0;
float zAlfa = 0;
float angle = -zAlfa / 180 * M_PI;

//угол обзора для корабля
float xAlfaShip = 0;
float zAlfaShip = 0;
float angleShip = -zAlfaShip / 180 * M_PI;
float speedShip = 0;
float speed = 0;


double t = 0;
double zoom = 6;
POINTFLOAT pos = { 0,0 };
POINTFLOAT pos1 = { 0,0 };

float angleMill = 0;
float angleBallon = 0;
POINTFLOAT Ballon = { -5,0 };


float posSun[4] = { 20, -29, -18, 0.5 };
float posSun1[4] = { 20, -29, -15, 0.5 };
POINTFLOAT posShip = { 0,0 };

GLUquadricObj* quadricObj;
GLUquadricObj* quadricObjDisk;
GLuint	texture;
GLuint	texture1;
GLuint	texture2;
GLuint	textureHouseWall;

GLuint testCube;



const char* skybox[6] = { "image/Left.bmp",
                           "image/Right.bmp",
                           "image/Top.bmp",
                           "image/Bottom.bmp",
                           "image/Front.bmp",
                           "image/Back.bmp" };
const char* house[6] = { "image/wall.jpg",
                           "image/wall.jpg",
                           "image/wall.jpg",
                           "image/wall.jpg",
                           "image/wall.jpg",
                           "image/wall.jpg", };

float cube[] = { -1,-1,1, 1,-1,1, 1,1,1, -1,1,1,
                -1,-1,-1, 1,-1,-1, 1,1,-1, -1,1,-1 };
unsigned int cubeInd[] = { 0,1,2,2,3,0, 4,5,6,6,7,4,
                          1,5,6,6,2,1, 0,4,7,7,3,0,
                          3,2,6,6,7,3, 0,1,5,5,4,0 };
int cubeIndCnt = sizeof(cubeInd) / sizeof(cubeInd[0]);

//координаты дома и текстуры
float vertex[] = { -1,-1,0, 1,-1,0, 1,1,0, -1,1,0 };
float texCoordHouseWall[] = { 0,1, 1,1, 1,0, 0,0 };


void LoadTextureCube(const char* filename[6], GLuint* target)
{
    //  glEnable(GL_TEXTURE_2D);
    glGenTextures(1, target);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *target);
 //  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char* data = stbi_load(filename[i], &width, &height, &nrChannels, 0);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    //  glDisable(GL_TEXTURE_2D);

}

void Load_Texture(const char* filename, GLuint* target)
{
    glColor3f(1.0, 1.0, 1.0);
    int width, height, cnt;
    unsigned char* data = stbi_load(filename, &width, &height, &cnt, 0);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glGenTextures(1, target);
    glBindTexture(GL_TEXTURE_2D, *target);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    

}

void ShowCube(GLuint* target)
{
  //  glDisable(GL_LIGHT0);
  //  glDisable(GL_LIGHTING);
    // glUseProgram(0);
   //  glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *target);
  //  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glVertexPointer(3, GL_FLOAT, 0, cube);
    glTexCoordPointer(3, GL_FLOAT, 0, cube);

   // glDisable(GL_DEPTH_TEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawElements(GL_TRIANGLES, cubeIndCnt, GL_UNSIGNED_INT, cubeInd);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

   // glEnable(GL_DEPTH_TEST);

    glDisable(GL_TEXTURE_CUBE_MAP);
   // glEnable(GL_LIGHTING);
  //  glEnable(GL_LIGHT0);
    // glDisable(GL_TEXTURE_2D);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_UP && action != GLFW_RELEASE)
    {
        xAlfa -= 1.9;
    }
    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE)
    {
        xAlfa += 1.9;
    }
    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
    {
        zAlfa += 1.9;
    }
    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
    {
        zAlfa -= 1.9;
    }


}


void DrawCharacter()
{
    glPushMatrix();

        glTranslatef(0,0,-3);

        //метла
        glPushMatrix();
        glColor3f(0.647, 0.411, 0.309);
        //glColor3f(0.721, 0.450, 0.2);
        glRotatef(-20, 1, 0, 0);
        gluCylinder(quadricObj, 0.25, 0, 0.6, 50, 50);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0, 0.17, 0.45);
        glColor3f(0.270, 0.196, 0.180);
        //glColor3f(0.207, 0.09, 0.047);
        glRotatef(-20, 1, 0, 0);
        gluCylinder(quadricObj, 0.04, 0.04, 1.3, 50, 50);
        glPopMatrix();

        //платье
        glPushMatrix();
        glTranslatef(0, 0.27, 1);
        glColor3f(0.58, 0, 0.827);
        glRotatef(-75, 1, 0, 0);
        gluCylinder(quadricObj, 0.25, 0, 0.7, 50, 50);
        glPopMatrix();

        //голова
        glPushMatrix();
        glTranslatef(0, 1, 1.2);
        glColor3f(0.96, 0.87, 0.70);
        gluSphere(quadricObj, 0.18, 50, 50);
        glPopMatrix();

        //шляпка
        glPushMatrix();
        glTranslatef(0, 1.0, 1.20);
        glColor3f(0.4, 0, 0.6);
            glPushMatrix();
            glRotatef(-110, 1, 0, 0);
            gluCylinder(quadricObj, 0.4, 0, 0.3, 50, 50);
            glPopMatrix();
            glPushMatrix();
            glTranslatef(0, 0.15, -0.08);
            glRotatef(-120, 1, 0, 0);
            gluCylinder(quadricObj, 0.15, 0, 0.6, 50, 50);
            glPopMatrix();
        glPopMatrix();

        //руки
        glPushMatrix();
        glTranslatef(-0.05, 0.55, 1.5);
        glColor3f(0.96, 0.87, 0.70);
        glRotatef(-150, 1, 0, 0);
        gluCylinder(quadricObj, 0.04, 0.03, 0.4, 50, 50);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.04, 0.55, 1.5);
        glColor3f(0.96, 0.87, 0.70);
        glRotatef(-150, 1, 0, 0);
        gluCylinder(quadricObj, 0.04, 0.03, 0.4, 50, 50);
        glPopMatrix();

        //ноги
        glPushMatrix();
        glTranslatef(-0.055, 0.27, 1);
        glColor3f(0.96, 0.87, 0.70);
        glRotatef(110, 1, 0, 0);
        gluCylinder(quadricObj, 0.05, 0.04, 0.2, 50, 50);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.055, 0.13, 0.65);
        glColor3f(0.96, 0.87, 0.70);
        glRotatef(10, 1, 0, 0);
        gluCylinder(quadricObj, 0.04, 0.03, 0.3, 50, 50);
        glPopMatrix();

        //ноги
        glPushMatrix();
        glTranslatef(0.045, 0.27, 1);
        glColor3f(0.96, 0.87, 0.70);
        glRotatef(130, 1, 0, 0);
        gluCylinder(quadricObj, 0.05, 0.03, 0.2, 50, 50);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.045, -0.06, 0.70);
        glColor3f(0.96, 0.87, 0.70);
        glRotatef(-45, 1, 0, 0);
        gluCylinder(quadricObj, 0.04, 0.02, 0.3, 50, 50);
        glPopMatrix();
        

        



    glPopMatrix();

}

GLvoid DrawSphere(const char* name, float r)
{
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);

    glEnable(GL_TEXTURE_2D);
    int width, height, numComponents;
    unsigned char* data = stbi_load(name, &width, &height, &numComponents, 0);

    glGenTextures(1, &texture);

     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  //  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 
    //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
     // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     // glGenerateMipmap(GL_TEXTURE_2D);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //  glGenerateMipmap(GL_TEXTURE_2D);


    gluQuadricTexture(quadricObj, GL_TRUE);
  //  gluCylinder(quadricObj, 1, 0, 4, 50, 50);
    glColor3f(1.0, 1.0, 1.0);
 //   glColor3f(0.529, 0.807, 0.921);
    gluDisk(quadricObj, 0, r, 50, 50);
  //  gluSphere(quadricObj, r, 20, 20);
    gluQuadricTexture(quadricObj, GL_FALSE);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

}
GLvoid DrawCylinder(const char* name, float r, float  R)
{
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);

    glEnable(GL_TEXTURE_2D);
    int width, height, numComponents;
    unsigned char* data = stbi_load(name, &width, &height, &numComponents, 0);

    glGenTextures(1, &texture2);

     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 
    

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    

    gluQuadricTexture(quadricObj, GL_TRUE);
    glColor3f(1.0, 1.0, 1.0);
      gluCylinder(quadricObj, R, r, 4, 50, 50);
    //  gluDisk(quadricObj, 0, r, 50, 50);
    //  gluSphere(quadricObj, r, 20, 20);
      gluQuadricTexture(quadricObj, GL_FALSE);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

}

//инициализация окна
GLFWwindow* Init()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1000, 800, "Solar System", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowPos(window, 40, 30);
    glfwMakeContextCurrent(window);
    gladLoadGL();
    LoadTextureCube(skybox, &testCube);
   // LoadTextureCube(house, &textureHouseWall);
    Load_Texture("image/wall.jpg", &textureHouseWall);

    return window;
}
void Setting()
{
    glLoadIdentity();
    //glOrtho(-1, 1, -1, 1, 1, -1);
    //glFrustum(-1, 1, -1, 1, 1, 40);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

}


//свои 
//массив вершин для пола



void DrawHouse()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureHouseWall);
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glTexCoordPointer(2, GL_FLOAT, 0, texCoordHouseWall);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
void MoveCamera()
{
    //движения "персонажа"
    angle = -zAlfa / 180 * M_PI;
    speed = 0;

    if (GetKeyState('W') < 0) speed = 0.09;
    if (GetKeyState('S') < 0) speed = -0.09;
    if (GetKeyState('A') < 0) { speed = 0.09; angle += M_PI * 0.5; }
    if (GetKeyState('D') < 0) { speed = 0.09; angle -= M_PI * 0.5; }
    if (speed != 0)
    {
        //  pos.x += sin(angle) * speed;
        //  pos.y += cos(angle) * speed;
        posBall.x -= sin(angle) * speed;
        posBall.y -= cos(angle) * speed;
        //  pos.x += sin(angle) * speed + posBall.x;
        //  pos.y += cos(angle) * speed + posBall.y;

    }

    gluLookAt(0, -15, -7,  //местоположение камеры
        0, 0, 1,  //камера смотрит в эту точку
        0, 1, 0);  //направление вектора «вверх»

    glRotatef(xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);
    glPushMatrix();
    glTranslatef(-posBall.x, -posBall.y, 1);
    glColor3f(1, 1, 1);

    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glRotatef(-90, 1, 0, 0);
    DrawShip();
    glPopMatrix();

    glPopMatrix();


} */

void MoveCamera()
{
    gluLookAt(0, -14, -7,  //местоположение камеры
        0, 0, 1,  //камера смотрит в эту точку
        0, 1, 0);  //направление вектора «вверх»

    glRotatef(xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);
    
}

void MoveCharacter()
{
    //движения "персонажа"
    angle = -angleShip / 180 * M_PI;
    //  angle = -zAlfa / 180 * M_PI;
    speed = 0;

    if (GetKeyState('W') < 0) speed = 0.6; //0.1 0.5
    if (speed != 0)
    {
        posShip.x -= sin(angle) * speed;
        posShip.y -= cos(angle) * speed;

    }

    if (GetKeyState('A') < 0) { angle -= M_PI * 0.5; angleShip -= M_PI * 6; } //0.5 3
    if (GetKeyState('D') < 0) { angle += M_PI * 0.5; angleShip += M_PI * 6; }


    glPushMatrix();
    glTranslatef(-posShip.x, -posShip.y, 0);

    glPushMatrix();
    glScalef(0.5, 0.5, 0.5);
    glRotatef(-90, 1, 0, 0);
    glRotatef(-angleShip, 0, 1, 0);
    DrawCharacter();
    glPopMatrix();

    glPopMatrix();
}

GLvoid DrawCircle(float radius, float size) //Drawing the orbits of the planets
{
    float angle = 0.0f;
    glPointSize(size);
    glBegin(GL_POINTS);
    for (int i = 0; i < 200; i++)
    {
        angle = 2 * 3.1416 * i / 200;
        glVertex3f(radius * cos(angle), radius * sin(angle), 0);
    }
    glEnd();
}



void ShowWorld()
{
    glScalef(1.5, 1.5, 1);
   
    /*glPushMatrix();
    glScalef(3.3, 2.9, 1);
    glColor3f(0.19, 0.4, 0.31);
    
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 1; i < N +1; i++)
        {      
          //  glColor3f(0.235/i, 0.7, 0.44);
            glVertex2f(A * sin(6.28 / N * i), (A + 0.1) * cos(6.28 / N * i));
        }
            glEnd();
    glPopMatrix(); */
    glColor3f(1, 1, 1);

   /* glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glColor3f(0, 0.411, 0.243);
   gluDisk(quadricObj, 0, 2.3, 50, 50);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);*/

  
    DrawSphere("image/15.jpg", 2.3);


    glPushMatrix();
    glColor3f(1, 1, 1);
    glScalef(2.3, 2.3, 1); //glScalef(2.3, 2.6, 1);
    DrawCylinder("image/grass2.jpg", 0, 1);
  //  gluCylinder(quadricObj, 1, 0, 4, 50, 50);
    glPopMatrix();
    glColor3f(1.0, 1.0, 1.0);

}


//дерево
void DrawTree1()
{
    //	glNormal3f(0, 0, 1);
    glPushMatrix();

    //glTranslatef(-2.5, 2.5, 0);
    //ствол
    glColor3d(0.45, 0.33, 0.26);
    gluQuadricDrawStyle(quadricObj, GLU_FILL);
    gluCylinder(quadricObj, 0.35, 0.25, 1, 10, 10);

    //нижний шарик листвы		
    glPushMatrix();
    glColor3d(0, 0.41, 0.135);
    glTranslatef(0.0, 0.0, 2.5);
    //DrawSphere(1.6);
    gluSphere(quadricObj, 1.6, 20, 20);
    glPopMatrix();
    //второй уровень листвы		
    glPushMatrix();
    glColor3d(0, 0.41, 0.235);
    glTranslatef(0.6, -0.7, 3.0);
    gluSphere(quadricObj, 1.4, 20, 20);
    glPopMatrix();
    //голубой шарик	
    glPushMatrix();
    glColor3d(0, 0.41, 0.435);
    glTranslatef(-1, -1, 3.9);
    gluSphere(quadricObj, 1, 20, 20);
    glPopMatrix();
    //		
    glPushMatrix();
    glColor3d(0, 0.41, 0.335);
    glTranslatef(-0.7, 0.5, 4);
    gluSphere(quadricObj, 1.3, 20, 20);
    glPopMatrix();
    //	
    glPushMatrix();
    glColor3d(0, 0.41, 0.10);
    glTranslatef(1, 0.5, 4.4);
    gluSphere(quadricObj, 1.2, 20, 20);
    glPopMatrix();
    //верхний шарик листвы		
    glPushMatrix();
    glColor3d(0, 0.41, 0.155);
    glTranslatef(0, -0.2, 5.5);
    gluSphere(quadricObj, 1.4, 20, 20);
    glPopMatrix();

    glPopMatrix();

    ////////////////////////////////////////////////	
}

//елка
void DrawTree2()
{
    //glNormal3f(0, 0, 1);
    glPushMatrix();

    //glTranslatef(2.5, 2.5, 0);
    //ствол
    glColor3d(0.45, 0.33, 0.26);
    gluQuadricDrawStyle(quadricObj, GLU_FILL);
    gluCylinder(quadricObj, 0.5, 0.4, 1, 10, 10);
    //нижний уровень листвы		
    glPushMatrix();
    glColor3d(0, 0.41, 0.135);
    glTranslatef(0.0, 0.0, 1);
    gluCylinder(quadricObj, 2, 0, 2.5, 10, 10);
    glPopMatrix();
    //средний уровень листвы		
    glPushMatrix();
    glTranslatef(0.0, 0.0, 2.5);
    gluCylinder(quadricObj, 1.5, 0, 2, 10, 10);
    glPopMatrix();
    //верхний уровень листвы		
    glPushMatrix();
    glTranslatef(0.0, 0.0, 4);
    gluCylinder(quadricObj, 1, 0, 1.5, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

void DrawBallon()
{
    glPushMatrix();

        glPushMatrix();
        glTranslatef(0.0, 0.0, 0.4);
      //  glColor3d(0.501, 0, 1);
      //  glColor3d(0.964, 0.462, 0.556);
        glColor3d(0.964, 0.392, 0.686);
        gluCylinder(quadricObj, 0.523, 0, 0.8, 20, 20);
        glPopMatrix();

        glPushMatrix();
        glScalef(1, 1, 1.3);
        gluSphere(quadricObj, 0.6, 20, 20);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0, 0.0, 1.5);
        glColor3d(0.47, 0.36, 0.28);
        gluCylinder(quadricObj, 0.1, 0.1, 0.15, 20, 20);
        glPopMatrix();

    glPopMatrix();
}

void SecondTree()
{
    glPushMatrix();
    glTranslatef(-3, -3, 0.3);
    glScalef(0.11, 0.11, 0.11);
    glRotatef(-180, 1, 0, 0);
    DrawTree2();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.99, -2.2, 0.3);
    glScalef(0.15, 0.15, 0.15);
    glRotatef(-180, 1, 0, 0);

    DrawTree2();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.4, -1.8, 0.3);
    glScalef(0.16, 0.16, 0.17);
    glRotatef(180, 0, 0, 1);
    glRotatef(-180, 1, 0, 0);
    DrawTree1();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.3, -3.4, 0.3);
    glScalef(0.09, 0.09, 0.10);
    glRotatef(-180, 1, 0, 0);
    DrawTree2();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-4.1, -1.8, 0.3);
    glScalef(0.19, 0.19, 0.20);
    glRotatef(-180, 1, 0, 0);
    DrawTree2();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.8, -0.8, 0.3);
    glScalef(0.23, 0.23, 0.23);
    glRotatef(-180, 1, 0, 0);
    DrawTree2();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.5, -2.8, 0.3);
    glScalef(0.14, 0.14, 0.15);
    glRotatef(90, 0, 0, 1);
    glRotatef(-180, 1, 0, 0);
    DrawTree1();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.8, -3.8, 0.3);
    glScalef(0.07, 0.07, 0.07);
    glRotatef(140, 0, 0, 1);
    glRotatef(-180, 1, 0, 0);
    DrawTree1();
    glPopMatrix();
}

void ThirdTree()
{
    glPushMatrix();
    glTranslatef(2.5, -2.5, 1.5);
    glScalef(0.2, 0.2, 0.2);
    glRotatef(-180, 1, 0, 0);
    DrawTree1();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.1, -3, 1.5);
    glScalef(0.1, 0.1, 0.1);
    glRotatef(150, 0, 0, 1);
    glRotatef(-180, 1, 0, 0);
    DrawTree1();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, -2.1, 1.5);
    glScalef(0.15, 0.15, 0.15);
    glRotatef(-180, 1, 0, 0);
    DrawTree2();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4, -3, 1.5);
    glScalef(0.25, 0.2, 0.2);
    glRotatef(-180, 1, 0, 0);
    DrawTree2();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.8, -2.1, 1.5);
    glScalef(0.14, 0.14, 0.15);
    glRotatef(90, 0, 0, 1);
    glRotatef(-180, 1, 0, 0);
    DrawTree1();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.4, -2.5, 1.5);
    glScalef(0.11, 0.11, 0.11);
    glRotatef(-180, 1, 0, 0);
    DrawTree2();
    glPopMatrix();
}

void FirstTree()
{
    glPushMatrix();
        
        glPushMatrix();
        glTranslatef(-1.7, 1, 0);
        glScalef(0.2, 0.2, 0.2);
        glRotatef(-180, 1, 0, 0);
        DrawTree1();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.5, 3.2, 0);
        glScalef(0.27, 0.27, 0.28);
        glRotatef(90, 0, 0, 1);
        glRotatef(-180, 1, 0, 0);
        DrawTree1();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.2, 2.5, 0);
        glScalef(0.3, 0.3, 0.3);
        glRotatef(-180, 1, 0, 0);
        DrawTree2();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1.3, 2.2, 0);
        glScalef(0.25, 0.25, 0.26);
        glRotatef(-180, 1, 0, 0);
        DrawTree2();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-2.2, 0.2, 0);
        glScalef(0.15, 0.15, 0.17);
        glRotatef(150, 0, 0, 1);
        glRotatef(-180, 1, 0, 0);
        DrawTree1();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-2.7, 1.3, 0);
        glScalef(0.33, 0.33, 0.35);
        glRotatef(-180, 1, 0, 0);
        DrawTree2();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-2.8, -0.2, 0);
        glScalef(0.12, 0.12, 0.12);
        glRotatef(180, 0, 0, 1);
        glRotatef(-180, 1, 0, 0);
        DrawTree1();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.8, 2.0, 0);
        glScalef(0.17, 0.17, 0.18);
        glRotatef(40, 0, 0, 1);
        glRotatef(-180, 1, 0, 0);
        DrawTree1();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(1.5, 2.5, 0);
        glScalef(0.25, 0.25, 0.25);
        glRotatef(-180, 1, 0, 0);
        DrawTree2();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(2.7, 0.7, 0);
        glScalef(0.25, 0.25, 0.25);
        glRotatef(-180, 1, 0, 0);
        DrawTree2();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(3.2, 0.0, 0);
        glScalef(0.16, 0.16, 0.17);
        glRotatef(90, 0, 0, 1);
        glRotatef(-180, 1, 0, 0);
        DrawTree1();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(2.7, -0.4, 0);
        glScalef(0.13, 0.13, 0.13);
        glRotatef(-180, 1, 0, 0);
        DrawTree2();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(2.9, -1.1, 0);
        glScalef(0.11, 0.11, 0.11);
        glRotatef(-180, 1, 0, 0);
        DrawTree2();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(2.2, 1.3, 0);
        glScalef(0.13, 0.13, 0.13);
        glRotatef(230, 0, 0, 1);
        glRotatef(-180, 1, 0, 0);
        DrawTree1();
        glPopMatrix();
        

    glPopMatrix();
}

void DrawLake()
{
    glPushMatrix();

        glPushMatrix();
        glTranslatef(1.2, -0.2, -0.01);
        DrawSphere("image/water3.jpg", 1.3);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(2.3, 0.3, 0.2);
        glColor3d(0.262, 0.294, 0.301);
        glScalef(0.6, 0.7, 1.2);
        gluSphere(quadricObj, 0.4, 50, 50);
        glPopMatrix();

        glPushMatrix();
        glColor3d(0.298, 0.317, 0.290);
        glTranslatef(2.4, -0.1, 0.2);
        glScalef(0.5, 0.8, 0.8);
        gluSphere(quadricObj, 0.4, 50, 50);
        glPopMatrix();

        glPushMatrix();
        glColor3d(0.392, 0.419, 0.388);
        glTranslatef(2.0, 0.8, 0.2);
        glScalef(0.9, 0.9, 1.4);
        gluSphere(quadricObj, 0.4, 50, 50);
        glPopMatrix();      

        glPushMatrix();
        glColor3d(0.262, 0.294, 0.301);
        glTranslatef(1.65, 0.9, 0.2);
        glScalef(0.7, 0.7, 0.9);
        gluSphere(quadricObj, 0.4, 50, 50);
        glPopMatrix();

        glPushMatrix();
        glColor3d(0.262, 0.294, 0.301);
        glTranslatef(0.6, 0.8, 0.1);
        glRotatef(120, 0, 0, 1);
       // glRotatef(-180, 1, 0, 0);
        glScalef(0.65, 0.95, 1.0);
        gluSphere(quadricObj, 0.4, 50, 50);
        glPopMatrix();

        glPushMatrix();
        glColor3d(0.411, 0.411, 0.411);
        glTranslatef(0.15, 0.35, 0.0);
        glRotatef(150, 0, 0, 1);
        // glRotatef(-180, 1, 0, 0);
        glScalef(0.45, 0.65, 0.5);
        gluSphere(quadricObj, 0.4, 50, 50);
        glPopMatrix();


        //лодка
        glPushMatrix();
        glColor3d(0.803, 0.603, 0.482);
        glTranslatef(0.8, -0.5, -0.02);
        glRotatef(-45, 0, 0, 1);
        glRotatef(-180, 1, 0, 0);
        glScalef(0.6, 0.3, 0.5);
        gluDisk(quadricObj, 0, 0.35, 50, 50);
        gluCylinder(quadricObj, 0.35, 0.5, 0.4, 50, 50);
        glPopMatrix();

    glPopMatrix();
}

void DrawScene(GLFWwindow* window)
{
    // glUseProgram(0);
    keyNum = 0;
    while (keyNum != GLFW_KEY_ESCAPE)
    {

        glfwPollEvents();

        //	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION); 	//видовая матри-ца
        glLoadIdentity();     //установим единичную матрицу
        
        glFrustum(-1, 1, -1, 1, 2, 80);

        GLfloat light[4] = { 0.1,0.1,0.1,1 };
        //ShowCube();
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light);

        glLightfv(GL_LIGHT0, GL_POSITION, posSun);

        glMatrixMode(GL_MODELVIEW); 	//видовая матри-ца
        glLoadIdentity();     //установим единичную матрицу

        float s = 0; float z = 0;

        quadricObj = gluNewQuadric();


           MoveCamera();


           glPushMatrix();

        //   MoveCamera();
           
           glColor3d(1, 1, 1);
           glScalef(20, 20, 20);
           glRotatef(-90, 1, 0, 0);
           glDisable(GL_DEPTH_TEST);
           glDisable(GL_LIGHT0);
           glDisable(GL_LIGHTING);
           ShowCube(&testCube);
           glEnable(GL_LIGHTING);
           glEnable(GL_LIGHT0);
           glEnable(GL_DEPTH_TEST);
           glPopMatrix();

           MoveCharacter();
     //      MoveCamera();

           glPushMatrix();

           glColor3d(1, 1, 1);
           glScalef(0.5, 0.5, 0.5);
          // glTranslatef(-1, 1.5, 10);
           glTranslatef(-1, 1.5, 1);
           glRotatef(45, 0, 0, 1);
         //  DrawHouse();
         //  glRotatef(-90, 1, 0, 0);
          // ShowCube(&textureHouseWall);
           glPopMatrix();

           glPushMatrix();

           glColor3d(1, 1, 1);
           glScalef(0.5, 0.5, 0.5);
           // glTranslatef(-1, 1.5, 10);
           glTranslatef(-1, 1.5, 1);
           glRotatef(90, 1, 0, 0);
        //   DrawHouse();
           //  glRotatef(-90, 1, 0, 0);
            // ShowCube(&textureHouseWall);
           glPopMatrix();
            
     //   MoveCamera();
        
        glPushMatrix();
     
            glPopMatrix();
           
            glTranslatef(0, 0, 1);

          //  glNormal3f(0, 0, 1);
            glPushMatrix();
            glScalef(1, 1, 1.4);
            ShowWorld();
          //  DrawSphere("image/0.jpg");
            glPopMatrix();

            DrawLake();
 
        //    glNormal3f(0, 0, 1);
            glPushMatrix();
            glTranslatef(-2, -2, 0.3);
            glScalef(0.7, 0.7, 0.7);
            ShowWorld();
            glPopMatrix();


          //  glNormal3f(0, 0, 1);
            glPushMatrix();
            glTranslatef(3, -3, 1.5);
            glScalef(0.4, 0.4, 0.6);
            ShowWorld();
            glPopMatrix();

            ///////////////////////третий остров/////////////////

            ThirdTree();
            
            /////////////////// второй остров ////////////////////////////
            
            SecondTree();

            /////////////////// первый остров ////////////////////////////

            FirstTree();
          
            //////////// мельница ///////////////////////////////////
            glPushMatrix();
                glScalef(1.4, 1.4, 1.4);
                glPushMatrix();
                    glTranslatef(-0.5, 1, -2);

                    glPushMatrix();
                        glTranslatef(0, 0.2, 0);
                        glPushMatrix();
                            glTranslatef(0, 0, -0.1);
                            glColor3d(0.305, 0.341, 0.329);
                            gluDisk(quadricObj, 0, 0.08, 40, 40);

                            glColor3d(1, 1, 1);
                            glScalef(1, 1, 0.7);
                            DrawCylinder("image/wall.jpg", 0.35, 0.08);
                        glPopMatrix();
                    glPopMatrix();
                    glRotatef(35, 0, 1, 0);      
                    glRotatef(90, 1, 0, 0);
                      
                    glPushMatrix();
                        glRotatef(angleMill, 0, 0, 1);
                        glColor3d(0.823, 0.705, 0.549);
                       // glColor3d(0.596, 0.462, 0.329);
                      //  glColor3d(0.815, 0.815, 0.815);
                        gluPartialDisk(quadricObj,0,1,  30,1,  0,30);
                        gluPartialDisk(quadricObj, 0, 1, 30, 1, 90, 30);
                        gluPartialDisk(quadricObj, 0, 1, 30, 1, 180, 30);
                        gluPartialDisk(quadricObj, 0, 1, 30, 1, 270, 30);
                    glPopMatrix();
            
                glPopMatrix();
            glPopMatrix();
            /////////////////////////////////////////////////


            glNormal3f(0, 0, 1);
            glPushMatrix();
            glTranslatef(-2, -2, -0.05);
            glScalef(0.4, 0.4, 0.6);
           // DrawSphere("image/water.jpg", 2);
            glPopMatrix();
            



            glPushMatrix();
            Ballon.x += sin(angleBallon / 180 * M_PI) * 0.13; //0.05 1.5
            Ballon.y += cos(angleBallon / 180 * M_PI) * 0.13; //0.08 1.5
            glTranslatef(Ballon.x, Ballon.y, -5.5);
            glRotatef(theta, 0.0f, 0.0f, 1.0f);
            DrawBallon();
           // gluDisk(quadricObj, 0, 1, 4, 4);
           // DrawSphere("image/grass4.jpg", 1);
            glPopMatrix();


          //  DrawSphere("image/grass.png", 1);

        glPopMatrix();
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
        gluDeleteQuadric(quadricObj);
        glDisable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);

        angleMill+=2;
        angleBallon += 1.5;
        theta += 1.0f;
        Sleep(1);
    }
    keyNum = 0;
}




int main()
{
    GLFWwindow* window = Init();
    if (!window)
    {
        return -1;
    }
    Setting();

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        DrawScene(window);


        glfwSwapBuffers(window);

    }
    glfwTerminate();
    return 0;
}
