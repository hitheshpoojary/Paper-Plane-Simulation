

#include <stdlib.h>
#include <stdio.h>
#ifndef WIN32
#include <unistd.h>
#else
#define random rand
#define srandom srand
#endif
#include <math.h>
#include <GL/glut.h>
#include<bits/stdc++.h>

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265
#endif
#ifndef M_PI_2
#define M_PI_2 1.57079632
#endif

GLboolean moving = GL_FALSE;
int arr[5] = {0};

#define MAX_PLANES 15

struct
{
    float speed;          /* zero speed means not flying */
    GLfloat red, green, blue;
    float theta;
    float x, y, z, angle;
} planes[MAX_PLANES];

#define v3f glVertex3f  /* v3f was the short IRIS GL name for
                           glVertex3f */
void delay(){
    //for(int i=0;i<100000000;i++);
    Sleep(100);
}
void draw(void)
{
    GLfloat red, green, blue;
    int i;

    glClear(GL_DEPTH_BUFFER_BIT);
    /* paint black to blue smooth shaded polygon for background */
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.0);
    v3f(-20, 20, -19);
    v3f(20, 20, -19);
    glColor3f(0.0, 0.0, 1.0);
    v3f(20, -20, -19);
    v3f(-20, -20, -19);
    glEnd();
    /* paint planes */
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    for (i = 0; i < MAX_PLANES; i++)
        if (planes[i].speed != 0.0)
        {
            glPushMatrix();
            glTranslatef(planes[i].x, planes[i].y, planes[i].z);
            glRotatef(290.0, 1.0, 0.0, 0.0);
            glRotatef(planes[i].angle, 0.0, 0.0, 1.0);
            glScalef(1.0 / 3.0, 1.0 / 4.0, 1.0 / 4.0);
            glTranslatef(0.0, -4.0, -1.5);
            glBegin(GL_TRIANGLE_STRIP);
            /* left wing */
            v3f(-7.0, 0.0, 2.0);
            v3f(-1.0, 0.0, 3.0);
            glColor3f(red = planes[i].red, green = planes[i].green,
                      blue = planes[i].blue);
            v3f(-1.0, 7.0, 3.0);
            /* left side */
            glColor3f(0.6 * red, 0.6 * green, 0.6 * blue);
            v3f(0.0, 0.0, 0.0);
            v3f(0.0, 8.0, 0.0);
            /* right side */
            v3f(1.0, 0.0, 3.0);
            v3f(1.0, 7.0, 3.0);
            /* final tip of right wing */
            glColor3f(red, green, blue);
            v3f(7.0, 0.0, 2.0);
            glEnd();
            glPopMatrix();
        }
    glutSwapBuffers();
}



void
tick_per_plane(int i)
{
    float theta = planes[i].theta += planes[i].speed;
    planes[i].z = -9 + 4 * cos(theta);
    planes[i].x = 4 * sin(2 * theta);
    planes[i].y = sin(theta / 3.4) * 3;
    planes[i].angle = ((atan(2.0) + M_PI_2) * sin(theta) - M_PI_2) * 180 / M_PI;
    if (planes[i].speed < 0.0)
        planes[i].angle += 180;
}

void
add_plane(void)
{
    int i;

    for (i = 0; i < MAX_PLANES; i++)
        if (planes[i].speed == 0)
        {

#define SET_COLOR(r,g,b) \
	planes[i].red=r; planes[i].green=g; planes[i].blue=b;

            switch (rand() % 6)
            {
            case 0:
                SET_COLOR(1.0, 0.0, 0.0);  /* red */
                break;
            case 1:
                SET_COLOR(1.0, 1.0, 1.0);  /* white */
                break;
            case 2:
                SET_COLOR(0.0, 1.0, 0.0);  /* green */
                break;
            case 3:
                SET_COLOR(1.0, 0.0, 1.0);  /* magenta */
                break;
            case 4:
                SET_COLOR(1.0, 1.0, 0.0);  /* yellow */
                break;
            case 5:
                SET_COLOR(0.0, 1.0, 1.0);  /* cyan */
                break;
            }
            planes[i].speed = ((float) (rand() % 20)) * 0.001 + 0.02;
            if (rand() & 0x1)
                planes[i].speed *= -1;
            planes[i].theta = ((float) (rand() % 257)) * 0.1111;
            tick_per_plane(i);
            if (!moving)
                glutPostRedisplay();
            return;
        }
}

void
remove_plane(void)
{
    int i;

    for (i = MAX_PLANES - 1; i >= 0; i--)
        if (planes[i].speed != 0)
        {
            planes[i].speed = 0;
            if (!moving)
                glutPostRedisplay();
            return;
        }
}

void
tick(void)
{
    int i;

    for (i = 0; i < MAX_PLANES; i++)
        if (planes[i].speed != 0.0)
            tick_per_plane(i);
}

void
animate(void)
{
    tick();
    glutPostRedisplay();
}

void
visible(int state)
{
    if (state == GLUT_VISIBLE)
    {
        if (moving)
            glutIdleFunc(animate);
    }
    else
    {
        if (moving)
            glutIdleFunc(NULL);
    }
}

/* ARGSUSED1 */
void
keyboard(unsigned char ch, int x, int y)
{
    switch (ch)
    {
    case ' ':
        if (!moving)
        {
            tick();
            glutPostRedisplay();
        }
        break;
    case 27:             /* ESC */
        exit(0);
        break;
    }
}

#define ADD_PLANE	1
#define REMOVE_PLANE	2
#define MOTION_ON	3
#define MOTION_OFF	4
#define QUIT		5

void
menu(int item)
{
    switch (item)
    {
    case ADD_PLANE:
        add_plane();
        break;
    case REMOVE_PLANE:
        remove_plane();
        break;
    case MOTION_ON:
        moving = GL_TRUE;
        glutChangeToMenuEntry(3, "Motion off", MOTION_OFF);
        glutIdleFunc(animate);
        break;
    case MOTION_OFF:
        moving = GL_FALSE;
        glutChangeToMenuEntry(3, "Motion", MOTION_ON);
        glutIdleFunc(NULL);
        break;
    case QUIT:
        exit(0);
        break;
    }
}



int flag=0;
int px=0;
int px1=0;
int px2=0;
int px3=0;
int py=0;
int pyvmax=0;
int pyvmin=0;
int py1=0;
int py2=0;
int py3=0;
GLfloat	tx	=  0.0;
GLfloat	ty	=  0.0;

int bx=0;
int bxmax=0;
int bxmin=0;
int by=0;
int bymax=0;
int bymin=0;
int score=0;
int count_r=0;
int flag1=0;

static  float cloud1_run=0;

void *currentfont;

void setFont(void *font)
{
    currentfont=font;
}

void drawstring(float x,float y,float z,char *string)
{
    int len = (int) strlen(string);
    int i;
    glRasterPos3f(x,y,z);

    for(i = 0; i<len; i++)
    {
        glColor3f(0.0,0.0,0.0);
        glutBitmapCharacter(currentfont,string[i]);
    }
}

void frontscreen(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    setFont(GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1,0,0);
    drawstring(-25,20,0.0,"NMAM INSTITUTE OF TECHNOLOGY, NITTE");
    glColor3f(1,0,0);
    drawstring(-32,16,0.0,"DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING");
    glColor3f(0,1,1);
    drawstring(-13,8,0.0,"A MINI PROJECT ON");
    glColor3f(0,1,1);
    drawstring(-16,5,0.0,"PAPER PLANE SIMULATION");
    glColor3f(1,0.5,0);
    drawstring(10-50,-3,0.0,"BY:");
    glColor3f(1,1,0);
    drawstring(10-50,-8,0.0,"HITHESH M POOJARY");
    glColor3f(1,1,0);
    drawstring(10-50,-12,0.0,"K KIRAN HOLLA");
    glColor3f(1,1,0);
    drawstring(16,-8,0.0,"4NM18CS065");
    glColor3f(1,1,0);
    drawstring(16,-12,0.0,"4NM18CS070");
    glColor3f(1,1,1);
    drawstring(-15,-32,0.0,"PRESS ENTER TO START");
    glutSwapBuffers();
    glFlush();
}

void mydisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    if(flag==0)
        frontscreen();
    if(flag==1)
        // display()
        ;
}

void circle(GLfloat rx,GLfloat ry,GLfloat cx,GLfloat cy)//radius_x,radius_y,certre_position_x,centre_position_y
{

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx,cy);

    for(int i=0; i<=100; i++)

    {
        float angle = 2.0f * 3.1416f * i/100;

        float x = rx * cosf(angle);
        float y = ry * sinf(angle);

        glVertex2f((x+cx),(y+cy));
    }
    glEnd();



}






void sky()
{
//sky
//glPushMatrix();
    glColor3f(0.4,0.7,1.0);
    glBegin(GL_POLYGON);
    glVertex2f(0,0);
    glVertex2f(600,0);
    glVertex2f(600,400);
    glVertex2f(0,400);
    glEnd();

}


void ground()
{
//Ground

    glPushMatrix();
    glTranslatef(0.0f, 30.0f, 0.0f);
    glRotatef(0, 0.0f, 0.0f, -1.0f);

    glColor3f(0.0,0.6,0.0);
    glBegin(GL_POLYGON);
    glVertex2f(-100,-100);
    glVertex2f(600,-100);
    glVertex2f(600,100);
    glVertex2f(0,150);
    glEnd();

    glPopMatrix();

}



void Tree2()
{
//Tree2

    glColor3f( 1,0.33,0);
    glBegin(GL_POLYGON);
    glVertex2f(400,50);
    glVertex2f(420,50);
    glVertex2f(420,120);
    glVertex2f(400,120);
    glEnd();

    glColor3f( 0.0,1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(350,120);
    glVertex2f(470,120);
    glVertex2f(410,180);
    glEnd();

    glColor3f( 0.2,0.6, 0.5);
    glBegin(GL_POLYGON);
    glVertex2f(360,160);
    glVertex2f( 460,160);
    glVertex2f( 410,220);
    glEnd();


}


void cloud1(float xpos)
{
    //Cloud1
    glTranslatef(xpos+10,0,0);
    glPushMatrix();
    glTranslatef(-60.0f, 5.0f, 0.0f);
    glRotatef(0, 0.0f, 0.0f, -1.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(20,20,150,350);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(20,20,100,360);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(20,20,135,370);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(20,20,125,370);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(20,20,165,360);

    glPopMatrix();
//glutPostRedisplay();
}


void cloud2()
{

    glPushMatrix();
    glTranslatef(-60.0f, 5.0f, 0.0f);
    glRotatef(0, 0.0f, 0.0f, -1.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(20,20,550,350);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(20,20,500,360);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(20,20,535,370);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(20,20,525,370);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(20,20,565,360);
    glPopMatrix();

}

void different_plane(int x,int y,bool flag)
{
    glPushMatrix();
    if((x>=755+arr[3] && x<=985+arr[3] && y>=180 && y<=250) || arr[3]!=0)
    {
        glColor3f (0.7,0.56,0.2);
        if(x>=755+arr[3] && x<=985+arr[3] && flag)
            arr[3]+=10;
        if(x>=755+arr[3] && x<=985+arr[3] && !flag)
            arr[3]-=10;

        glBegin(GL_POLYGON);
        glVertex2i(300+arr[3],250);
        glVertex2i(385+arr[3],280);
        glVertex2i(305+arr[3],280);
        glEnd();
        glColor3f (0.4,0.1,0.3);
        glBegin(GL_POLYGON);
        glVertex2i(295+arr[3],270);
        glVertex2i(385+arr[3],280);
        glVertex2i(305+arr[3],280);
        glEnd();
    }
    else
    {
        glColor3f (0.7,0.56,0.2);
        glBegin(GL_POLYGON);
        glVertex2i(300,250);
        glVertex2i(385,280);
        glVertex2i(305,280);
        glEnd();
        glColor3f (0.4,0.1,0.3);
        glBegin(GL_POLYGON);
        glVertex2i(295,270);
        glVertex2i(385,280);
        glVertex2i(305,280);
        glEnd();
    }
    glPopMatrix();
    glutPostRedisplay();
    glutSwapBuffers();
}
static float	cloud_right =  0.0;

void hosa_plane(int x,int y,bool flag)
{


    if((x>=160-arr[0] && x<=375-arr[0] && y>=225 && y<=340) || (arr[0]!=0))
    {
        if(x<=375-arr[0] && flag)
            arr[0]+=10;

        if(x<=375-arr[0] && !flag)
            arr[0]-=10;
        glPushMatrix();

        glScalef(0.5,0.5,1);
        glTranslated(-150,450,1);
        glRotated(-30,0,0,1);

        glColor3f(1.0,0,1.0);
        glBegin(GL_POLYGON);
        glVertex2i(200-arr[0],200);
        glVertex2i(300-arr[0],200);
        glVertex2i(350-arr[0],280);
        glEnd();


        glColor3f(1.0,1.0,1);
        glBegin(GL_POLYGON);
        glVertex2i(200-arr[0],200);
        glVertex2i(300-arr[0],200);
        glVertex2i(400-arr[0],140);
        glEnd();
        glPopMatrix();
        glFlush();
    }
    else
    {
        glPushMatrix();

        glScalef(0.5,0.5,1);
        glTranslated(-150,450,1);
        glRotated(-30,0,0,1);

        glColor3f(1.0,0,1.0);
        glBegin(GL_POLYGON);
        glVertex2i(200,200);
        glVertex2i(300,200);
        glVertex2i(350,280);
        glEnd();


        glColor3f(1.0,1.0,1);
        glBegin(GL_POLYGON);
        glVertex2i(200,200);
        glVertex2i(300,200);
        glVertex2i(400,140);
        glEnd();
        glPopMatrix();
        glFlush();
    }
    glutPostRedisplay();
    glutSwapBuffers();
}


void jhinchak_plane(int x,int y,bool flag)
{

    glPushMatrix();
    glScaled(0.4,0.4,1);
    glTranslated(1000,150,1);
    glRotatef(30,0.0,0.0,1.0);

    if((x>=1195+arr[1] && x<=1400+arr[1] && y>=250 && y<340) || arr[1]!=0)
    {
        if(x<=1400+arr[1] && x>=1195+arr[1] && flag)
            arr[1]+=10;

        if(x<=1400+arr[1] && x>=1195+arr[1] && !flag)
            arr[1]-=10;
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_POLYGON);
        glVertex2f(500+arr[1],200);
        glVertex2f(500+arr[1],250);
        glVertex2f(550+arr[1],200);
        glEnd();

        glColor3f(0.5,0.5,0.2);
        glBegin(GL_POLYGON);
        glVertex2f(380+arr[1],250);
        glVertex2f(400+arr[1],200);
        glVertex2f(500+arr[1],200);
        glVertex2f(500+arr[1],250);
        glEnd();

        glColor3f(.8,0.0,0.4);
        glBegin(GL_POLYGON);
        glVertex2f(350+arr[1],250);
        glVertex2f(365+arr[1],300);
        glVertex2f(380+arr[1],250);
        glEnd();


        glColor3f(1.0,1.0,1.0);
        glBegin(GL_POLYGON);
        glVertex2f(325+arr[1],200);
        glVertex2f(350+arr[1],250);
        glVertex2f(380+arr[1],250);
        glVertex2f(400+arr[1],200);
        glEnd();

    }
    else
    {
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_POLYGON);
        glVertex2f(500,200);
        glVertex2f(500,250);
        glVertex2f(550,200);
        glEnd();

        glColor3f(0.5,0.5,0.2);
        glBegin(GL_POLYGON);
        glVertex2f(380,250);
        glVertex2f(400,200);
        glVertex2f(500,200);
        glVertex2f(500,250);
        glEnd();

        glColor3f(.8,0.0,0.4);
        glBegin(GL_POLYGON);
        glVertex2f(350,250);
        glVertex2f(365,300);
        glVertex2f(380,250);
        glEnd();


        glColor3f(1.0,1.0,1.0);
        glBegin(GL_POLYGON);
        glVertex2f(325,200);
        glVertex2f(350,250);
        glVertex2f(380,250);
        glVertex2f(400,200);
        glEnd();
    }

    glPopMatrix();
    glutPostRedisplay();
    glutSwapBuffers();

}



void cloud1_move()
{
    cloud_right=cloud_right+0.0005;
    if (cloud_right>100)
        cloud_right=-50;
    glutPostRedisplay();
}

void plane3(int x,int y,bool flag)
{
   glPushMatrix();
    glScalef(0.5,0.5, 0.5);
    glTranslated(600,-200,1);
    glRotatef(90.0,0.0,0.0,1.0);
    if((x>=380 && x<=480 && y>=405+arr[2] && y<=490+arr[2]) || arr[2]!=0)
    {
        if(y>=405+arr[2] && y<=490+arr[2] && flag)
            arr[2]+=10;
        if(y>=405+arr[2] && y<=490+arr[2] && !flag)
            arr[2]-=10;
        glColor3f(0.3, 1.0,1.0);
        glBegin(GL_POLYGON);
        glVertex2i(440,300+arr[2]);
        glVertex2i(440,250+arr[2]);
        glVertex2i(500,230+arr[2]);
        glEnd();

        glColor3f(0.1,0.8,0.79);
        glBegin(GL_POLYGON);
        glVertex2i(400,230+arr[2]);
        glVertex2i(440,250+arr[2]);
        glVertex2f(440,300+arr[2]);
        glEnd();
    }
    else
    {
        glColor3f(0.3, 1.0,1.0);
        glBegin(GL_POLYGON);
        glVertex2i(440,300);
        glVertex2i(440,250);
        glVertex2i(500,230);
        glEnd();

        glColor3f(0.1,0.8,0.79);
        glBegin(GL_POLYGON);
        glVertex2i(400,230);
        glVertex2i(440,250);
        glVertex2f(440,300);;
        glEnd();
    }
    glPopMatrix();
    glutPostRedisplay();
    glutSwapBuffers();




}




 void nobitaBody(){
  glPushMatrix();


  glColor3f(1.0,1.0,0.0);
  glBegin(GL_POLYGON);
  glVertex2f(85,127);
  glVertex2f(70,110);
  glVertex2f(85,110);
  glVertex2f(85,80);
  glVertex2f(115,80);
  glVertex2f(115,110);
  glVertex2f(130,110);
  glVertex2f(115,127);
 glEnd();
 glPopMatrix();
 glutSwapBuffers();


 }
 void nobitachaddi(){

 glPushMatrix();

 glColor3f(0.0,0.0,0.5);
 glBegin(GL_POLYGON);
 glVertex2f(85,80);
 glVertex2f(85,60);
 glVertex2f(95,60);
 glVertex2f(100,65);
 glVertex2f(105,60);
 glVertex2f(115,60);
 glVertex2f(115,80);
 glEnd();


  glPopMatrix();
 glutSwapBuffers();

 }
  void nobitatale(){
     glPushMatrix();
  glColor3f(0,0,0);
glTranslated(15,15,0);
 glBegin(GL_TRIANGLE_FAN);
    glVertex2f(85,127);
 for(int i=0; i<=100; i++)

    {
        float angle = 2.0f * 3.1416f * i/100;

        float x = 15 * cosf(angle);
        float y = 15 * sinf(angle);

        glVertex2f((x+85),(y+127));
    }
    glEnd();
  glPopMatrix();
  glutSwapBuffers();
  }

   void nobitaarms(){

   glPushMatrix();

 glScaled(1.2,1.2,1.2);
 glTranslated(-10,-30,0);
glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(75,127);
glVertex2f(80,122);
glVertex2f(70,100);
glVertex2f(65,100);



glEnd();

   glPopMatrix();
   glutSwapBuffers();

   }

void nobitaarms2(){
glPushMatrix();
glScaled(1.2,1.2,1.2);
 glTranslated(-23,-30,0);
glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(125,127);
glVertex2f(120,122);
glVertex2f(130,100);
glVertex2f(135,100);
glEnd();

glPopMatrix();
glutSwapBuffers();
}

void humannn(){
  glPushMatrix();



   glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(85,73);
glVertex2f(85,40);
glVertex2f(95,73);
glVertex2f(95,40);
glEnd();





glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(105,73);
glVertex2f(105,40);
glVertex2f(115,73);
glVertex2f(115,40);
glEnd();

glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(85,40);
glVertex2f(85,73);
glVertex2f(95,40);
glVertex2f(95,73);
glEnd();


glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(105,40);
glVertex2f(105,73);
glVertex2f(115,40);
glVertex2f(115,73);
glEnd();



glColor3f(0,0,0);
glBegin(GL_POLYGON);
glVertex2f(80,32);
glVertex2f(85,40);
glVertex2f(95,40);
glVertex2f(96,32 );
glEnd();

glColor3f(0,0,0);
glBegin(GL_POLYGON);
glVertex2f(105,40);
glVertex2f(104,32);
glVertex2f(120,32 );
glVertex2f(115,40);
glEnd();



/*glColor3f(0,0,0);
glBegin(GL_POLYGON);
glVertex2f(85,127);
glVertex2f(85,150);
glVertex2f(115,150);
glVertex2f(115,127);
glEnd();


glColor3f(0.3,0.6,0.9);
glBegin(GL_POLYGON);
glVertex2f(85,150);
glVertex2f(81,160);
glVertex2f(88,155);
glVertex2f(92,158);
glVertex2f(96,153);
glVertex2f(100,158);
glVertex2f(105,154);
glVertex2f(108,160);
glVertex2f(111,155);
glVertex2f(115,150);
glEnd();
*/






 glPopMatrix();
 glutSwapBuffers();

}



void hand1(){
       glPushMatrix();
glColor3f(0,0,0);
glTranslated(-51,31,0);
 glBegin(GL_TRIANGLE_FAN);
    glVertex2f(120,50);
 for(int i=0; i<=100; i++)

    {
        float angle = 2.0f * 3.1416f * i/100;

        float x =4* cosf(angle);
        float y = 4 * sinf(angle);

        glVertex2f((x+120),(y+50));
    }
    glEnd();
 glPopMatrix();
 glutSwapBuffers();

}
void hand2(){
   glPushMatrix();
glColor3f(0,0,0);
glTranslated(11.5,31,0);
 glBegin(GL_TRIANGLE_FAN);
    glVertex2f(120,50);
 for(int i=0; i<=100; i++)

    {
        float angle = 2.0f * 3.1416f * i/100;

        float x =4* cosf(angle);
        float y = 4 * sinf(angle);

        glVertex2f((x+120),(y+50));
    }
    glEnd();
 glPopMatrix();
 glutSwapBuffers();
}

void shin_tale(){
glColor3f(0,0,0);
circle(13,13,250,132);

}


void shin_arms(){
 glPushMatrix();
 glTranslated(155,-18,0);
glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(75,127);
glVertex2f(80,122);
glVertex2f(70,100);
glVertex2f(65,100);
glEnd();
   glPopMatrix();


  glPushMatrix();

 glTranslated(145,-18,0);
glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(125,127);
glVertex2f(120,122);
glVertex2f(130,100);
glVertex2f(135,100);
glEnd();

glPopMatrix();

circle(5,5,220,80);
circle(5,5,280,80);
glutSwapBuffers();

}


void shin_angi(){
 glPushMatrix();

glTranslated(160,5,0);
glScaled(0.9,0.9,0.9);
  glColor3f(1.0,0.0,0.0);
  glBegin(GL_POLYGON);
  glVertex2f(85,127);
  glVertex2f(70,110);
  glVertex2f(85,110);
  glVertex2f(85,80);
  glVertex2f(115,80);
  glVertex2f(115,110);
  glVertex2f(130,110);
  glVertex2f(115,127);
 glEnd();
 glPopMatrix();
 glutSwapBuffers();



}
void shin_legs(){

    glPushMatrix();
    glScaled(0.9,0.9,1);
    glTranslated(177.5,0,0);



   glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(85,73);
glVertex2f(85,40);
glVertex2f(95,73);
glVertex2f(95,40);
glEnd();





glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(105,73);
glVertex2f(105,40);
glVertex2f(115,73);
glVertex2f(115,40);
glEnd();

glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(85,40);
glVertex2f(85,73);
glVertex2f(95,40);
glVertex2f(95,73);
glEnd();


glColor3f(0.85,0.72,0.6);
glBegin(GL_POLYGON);
glVertex2f(105,40);
glVertex2f(105,73);
glVertex2f(115,40);
glVertex2f(115,73);
glEnd();



glColor3f(0,0,0);
glBegin(GL_POLYGON);
glVertex2f(80,32);
glVertex2f(85,40);
glVertex2f(95,40);
glVertex2f(96,32 );
glEnd();

glColor3f(0,0,0);
glBegin(GL_POLYGON);
glVertex2f(105,40);
glVertex2f(104,32);
glVertex2f(120,32 );
glVertex2f(115,40);
glEnd();


glPopMatrix();
glutSwapBuffers();
}

void shin_chaddi(){

    glPushMatrix();
    glScaled(0.9,0.9,1);
glTranslated(177.5,6,0);
 glColor3f(1.0,1.0,0.0);
 glBegin(GL_POLYGON);
 glVertex2f(85,80);
 glVertex2f(85,60);
 glVertex2f(95,60);
 glVertex2f(100,65);
 glVertex2f(105,60);
 glVertex2f(115,60);
 glVertex2f(115,80);
 glEnd();


  glPopMatrix();
 glutSwapBuffers();

}


void gudde(){
glPushMatrix();
glColor3f(0.59,0.3,0.2);
glBegin(GL_POLYGON);
glVertex2f(0,295);
glVertex2f(150,395);
glVertex2f(300,295);
glEnd();

glColor3f(0.59,0.3,0.2);
glBegin(GL_POLYGON);
glVertex2f(290,295);
glVertex2f(400,380);
glVertex2f(600,295);
glEnd();


glPopMatrix();
glutSwapBuffers();


}

void sunny(){
glPushMatrix();
glColor3f(1.0,1.0,0.0);
glBegin(GL_TRIANGLE_FAN);
    glVertex2f(300,360);
 for(int i=0; i<=100; i++)

    {
        float angle = 2.0f * 3.1416f * i/100;

        float x =25* cosf(angle);
        float y = 25 * sinf(angle);

        glVertex2f((x+300),(y+360));
    }
    glEnd();
 glPopMatrix();
 glutSwapBuffers();

}
void mane(){


glPushMatrix();
glScaled(1.2,1.2,1.2);
glTranslated(-45,0,0);


glColor3f(0.9,0.4,0.1);
glBegin(GL_POLYGON);
glVertex2f(440,100);
glVertex2f(460,150);
glVertex2f(480,100);
glEnd();

glColor3f(0.9,0.4,0.1);
glBegin(GL_POLYGON);
glVertex2f(480,100);
glVertex2f(460,150);
glVertex2f(510,150);
glVertex2f(530,100);
glEnd();




glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(490,100);
glVertex2f(470,150);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(500,100);
glVertex2f(480,150);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(510,100);
glVertex2f(490,150);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(520,100);
glVertex2f(500,150);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(480,100);
glVertex2f(460,150);
glEnd();


glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(440,100);
glVertex2f(480,100);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(480,100);
glVertex2f(530,100);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(476,110);
glVertex2f(526,110);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(472,120);
glVertex2f(522,120);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(468,130);
glVertex2f(518,130);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(464,140);
glVertex2f(514,140);
glEnd();



glColor3f(0.6,0,0.2);
glBegin(GL_POLYGON);
glVertex2f(440,100);
glVertex2f(440,30);
glVertex2f(480,30);
glVertex2f(480,100);
glEnd();

glColor3f(0.3,0.0,0.3);
glBegin(GL_POLYGON);
glVertex2f(480,30);
glVertex2f(480,100);
glVertex2f(530,100);;
glVertex2f(530,30);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(480,100);
glVertex2f(480,30);
glEnd();

glColor3f(1,1,1);
glBegin(GL_POLYGON);
glVertex2f(450,30);
glVertex2f(450,65);
glVertex2f(470,65);
glVertex2f(470,30);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(450,30);
glVertex2f(450,65);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(450,65);
glVertex2f(470,65);
glEnd();


glColor3f(0.7,0.4,0.1);
glBegin(GL_POLYGON);
glVertex2f(470,65);
glVertex2f(455,60);
glVertex2f(455,30);
glVertex2f(470,30);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(470,65);
glVertex2f(455,60);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(455,60);
glVertex2f(455,30);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(470,65);
glVertex2f(470,30);
glEnd();


glColor3f(1,1,1);
glBegin(GL_POLYGON);
glVertex2f(485,65);
glVertex2f(485,80);
glVertex2f(500,80);
glVertex2f(500,65);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(485,65);
glVertex2f(485,80);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(485,80);
glVertex2f(500,80);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(500,80);
glVertex2f(500,65);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(485,65);
glVertex2f(500,65);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(492,65);
glVertex2f(492,80);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(485,72);
glVertex2f(500,72);
glEnd();

glColor3f(1,1,1);
glBegin(GL_POLYGON);
glVertex2f(525,65);
glVertex2f(525,80);
glVertex2f(510,80);
glVertex2f(510,65);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(517,65);
glVertex2f(517,80);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(510,72);
glVertex2f(525,72);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(525,65);
glVertex2f(525,80);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(525,80);
glVertex2f(510,80);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(510,80);
glVertex2f(510,65);
glEnd();
glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(525,65);
glVertex2f(510,65);
glEnd();


glPopMatrix();
glutSwapBuffers();
}
 void manekandi(){

 glPushMatrix();
glColor3f(1,1,1);
glTranslated(376,54,0);
 glBegin(GL_TRIANGLE_FAN);
    glVertex2f(120,50);
 for(int i=0; i<=100; i++)

    {
        float angle = 2.0f * 3.1416f * i/100;

        float x =5.5* cosf(angle);
        float y = 5.5* sinf(angle);

        glVertex2f((x+120),(y+50));
    }
    glEnd();
 glPopMatrix();
 glutSwapBuffers();

 }


 void gaalipata(float pos){
     glTranslated(pos,0,0);
 glPushMatrix();
glTranslated(250,300,0);
glRotated(-40,0,0,1);
glScaled(0.7,0.7,0.7);

 glColor3f(1.0,0.1,0.5);
 glBegin(GL_POLYGON);
 glVertex2f(200,200);
 glVertex2f(230,250);
 glVertex2f(260,200);
 glVertex2f(230,150);
 glEnd();

 glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(230,150);
glVertex2f(240,-220);
glEnd();



glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(230,150);
glVertex2f(230,250);
glEnd();


glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2f(200,200);
glVertex2f(260,200);
glEnd();


 glPopMatrix();
 glutSwapBuffers();


 }


 void gaalipata1(){
  glPushMatrix();



 glPopMatrix();
 glutSwapBuffers();
 }
 void pakki(float pos)
 {  glPushMatrix();
 glTranslated(pos,0,0);
 glTranslated(100,300,0);
    glColor3f(0,0,0);
     glBegin(GL_LINES);
     glVertex2f(0,0);
     glVertex2f(15,15);
     glVertex2f(0,0);
     glVertex2f(-10,15);
     glEnd();
     glPopMatrix();
 }
float cloudx,kitex;
void day(int x,int y,bool flag)
{
    glClear(GL_COLOR_BUFFER_BIT);

    sky();
     gudde();
     sunny();
  /*   if(flag1==1){
   glPushMatrix();
   glColor3f(0.0,1.0,0.0);
   glBegin(GL_POLYGON);
   glVertex2f(250,250);
      glVertex2f(250,300);
         glVertex2f(300,300);
            glVertex2f(300,250);
          //   glColor3f(1,1,0);
   // drawstring(0,0,0.0,"4NM18CS065");
            glEnd();
            glPopMatrix();
            glutSwapBuffers();
            glFlush();

  }
*/
glPushMatrix();
    //delay();

    cloudx+=1;
    if(cloudx==10)
        cloudx=-10;
    cloud1(cloudx);
glPopMatrix();
//  cloud1_move();
    /*
      for(int i=0; i<1; i++)
        {
            glPushMatrix();
            glTranslatef(cloud1_run+i,0,0);
            cloud1();
            glPopMatrix();
        }*/


    cloud2();
    ground();
    Tree2();
    mane();
    manekandi();
    plane3(x,y,flag);
    different_plane(x,y,flag);
    hosa_plane(x,y,flag);
    jhinchak_plane(x,y,flag);
    humannn();
    glPushMatrix();
    for (int i=0;i<10;i++)
    {
         pakki(cloudx);
         if(i%2==0)
         glTranslatef(30,0,0);
         else
             glTranslatef(20,20,0);
    }
    glPopMatrix();
    hand1();
    hand2();
    nobitaarms();
    nobitaarms2();
nobitaBody();
nobitachaddi();
nobitatale();
glPushMatrix();
kitex+=1;
if(kitex==10)
    kitex=-2;
gaalipata(kitex);

glPopMatrix();
gaalipata1();


shin_arms();
shin_angi();
shin_tale();
shin_legs();
shin_chaddi();





    glPushMatrix();
    px1=250+px;
    py1=340+py;
    pyvmax=py1;
    pyvmin=310+py;
    px2=165+px;
    py2=pyvmin;
    px3=170+px;
    py3=py1;
    if((x>=405+arr[4] && x<=645+arr[4] && y>=75 && y<=150) || arr[4]!=0)
    {

        glColor3f (1.0,0,0);
        if(x>=405+arr[4] && x<=645+arr[4] && flag)
            arr[4]+=10;
        if(x>=405+arr[4] && x<=645+arr[4] && !flag)
            arr[4]-=10;
        glBegin(GL_POLYGON);
        glVertex2i(px2+arr[4],pyvmin);
        glVertex2i(px1+arr[4],py1);
        glVertex2i(170+px+arr[4],340+py);
        glEnd();
        glColor3f (0.847,0.847,0.847);
        glBegin(GL_POLYGON);
        glVertex2i(160+px+arr[4],330+py);
        glVertex2i(250+px+arr[4],340+py);
        glVertex2i(px3+arr[4],340+py);

        glEnd();
    }
    else
    {
        glColor3f (1.0,0,0);
        glBegin(GL_POLYGON);
        glVertex2i(px2,pyvmin);
        glVertex2i(px1,py1);
        glVertex2i(170+px,340+py);
        glEnd();
        glColor3f (0.847,0.847,0.847);
        glBegin(GL_POLYGON);
        glVertex2i(160+px,330+py);
        glVertex2i(250+px,340+py);
        glVertex2i(px3,340+py);

        glEnd();
    }
    glPopMatrix();
    /* cloud1_move();
    for(int i=0; i<10; i++)
    {
       glPushMatrix();
       glTranslatef(cloud_right+i,0,0);
       cloud1();
       glPopMatrix();
    }*/

    glutSwapBuffers();

    //
}


void myInit1 (void)
{
    glClearColor(0,0,0,0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(4.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,600.0,0.0,400.0);
}

void mykey2(unsigned char key, int x,int y)
{
    switch(key)
    {
    case 27:             /* ESC */
        exit(0);
        break;

    case ' ':
        glutKeyboardFunc(mykey2);
        if (!moving)
        {
            tick();
            glutPostRedisplay();
        }
    }
}

void myinit2()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutCreateWindow("glutplane");
    glutDisplayFunc(draw);

    glutVisibilityFunc(visible);
    glutCreateMenu(menu);
    glutAddMenuEntry("Add plane", ADD_PLANE);
    glutAddMenuEntry("Remove plane", REMOVE_PLANE);
    glutAddMenuEntry("Motion", MOTION_ON);
    glutAddMenuEntry("Quit", QUIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    /* setup OpenGL state */
    glClearDepth(1.0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 20);
    glMatrixMode(GL_MODELVIEW);
    /* add three initial random planes */

    srand(4);
    add_plane();
    add_plane();
    add_plane();
    day(0,0,true);
    glutKeyboardFunc(mykey2);
}




void Rectangle(){
   glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
glScaled(1.2,1.2,1.2);
    glColor3f(1.0,1.0,1.0);
     glTranslated(0,-130,0);
    glBegin(GL_POLYGON);
    glVertex2f(200,200);
    glVertex2f(200,400);
   glVertex2f(300,200);
    glVertex2f(300,400);
    glEnd();


 glColor3f(1.0,1.0,1.0);
       glBegin(GL_POLYGON);
           glVertex2f(200,400);
            glVertex2f(250,300);
            glVertex2f(300,400);
            glEnd();

        glPopMatrix();
    glutSwapBuffers();

}

void triangle_over_rect(){
     glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();
glScaled(1.2,1.2,1.2);
glColor3f(1.0,1.0,1.0);
  glTranslated(0,-130,0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(200,325);
glVertex2f(300,200);
glVertex2f(300,325);

glEnd();

glColor3f(1.0,0.0,0.0);
glBegin(GL_POLYGON);
glVertex2f(250,400);
glVertex2f(200,325);
glVertex2f(300,325);
glEnd();



 glColor3f(1.0,1.0,1.0);
       glBegin(GL_POLYGON);
           glVertex2f(200,325);
            glVertex2f(250,250);
            glVertex2f(300,325);
            glEnd();



glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(250,400);
glVertex2d(250,325);
glEnd();



glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(200,325);
glVertex2d(300,325);
glEnd();


glPopMatrix();
glutSwapBuffers();

}
void fold_triangle(){
     glClear(GL_COLOR_BUFFER_BIT);

glPushMatrix();

glScaled(1.2,1.2,1.2);
glColor3f(1.0,1.0,1.0);
 glTranslated(0,-130,0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(200,325);
glVertex2f(250,250);
glVertex2f(300,200);
glVertex2f(300,325);
glEnd();

glColor3f(1.0,0.0,0.0);
glBegin(GL_POLYGON);
glVertex2f(200,325);
glVertex2f(250,250);
glVertex2f(300,325);
glEnd();


glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(200,325);
glVertex2d(250,250);
glEnd();


glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(250,250);
glVertex2d(300,325);
glEnd();


glPopMatrix();
glutSwapBuffers();
}

void folded_plane(){
     glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();
glScaled(1.2,1.2,1.2);
 glTranslated(0,-130,0);

glColor3f(0.9,0.0,0.0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(240,400);
glVertex2f(250,400);
glVertex2f(250,250);
glEnd();

glColor3f(0.9,0.0,0.0);
glBegin(GL_POLYGON);
glVertex2f(250,250);
glVertex2f(250,400);
glVertex2f(260,400);
glVertex2f(300,200);
glEnd();



glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(250,250);
glVertex2d(250,400);

glEnd();

glPopMatrix();
glutSwapBuffers();
}


void Rectangle2(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glScaled(1.2,1.2,1.2);
    glColor3f(1.0,1.0,1.0);
     glTranslated(0,-130,0);
    glBegin(GL_POLYGON);
    glVertex2f(200,200);
    glVertex2f(200,400);
   glVertex2f(300,200);
    glVertex2f(300,400);
    glEnd();

 glColor3f(1.0,1.0,1.0);
       glBegin(GL_POLYGON);
           glVertex2f(200,400);
            glVertex2f(250,300);
            glVertex2f(300,400);
            glEnd();


        glPopMatrix();
    glutSwapBuffers();

}

void triangle_over_rect2(){
 glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();
glScaled(1.2,1.2,1.2);


glColor3f(1.0,1.0,1.0);
   glTranslated(0,-130,0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(200,325);
glVertex2f(300,200);
glVertex2f(300,325);
glEnd();

glColor3f(0.0,0.7,0.8);
glBegin(GL_POLYGON);
glVertex2f(200,325);
glVertex2f(250,400);
glVertex2f(300,325);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(250,400);
glVertex2d(250,325);
glEnd();


 glColor3f(1.0,1.0,1.0);
       glBegin(GL_POLYGON);
           glVertex2f(200,325);
            glVertex2f(250,250);
            glVertex2f(300,325);
            glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(200,325);
glVertex2d(300,325);
glEnd();


glPopMatrix();
glutSwapBuffers();

}

void step3(){
    glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();
glScaled(1.1,1.1,1.1);
glColor3f(0.0,0.7,0.8);
 glTranslated(0,-130,0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(250,300);
glVertex2f(250,450);
glEnd();

glColor3f(0.0,0.7,0.8);
glBegin(GL_POLYGON);
glVertex2f(300,200);
glVertex2f(250,300);
glVertex2f(250,450);
glEnd();

glColor3f(1.0,1.0,1.0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(250,300);
glVertex2f(300,200);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(250,450);
glVertex2d(250,300);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(200,200);
glVertex2d(250,300);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(300,200);
glVertex2d(250,300);
glEnd();


glPopMatrix();
glutSwapBuffers();
}
void step4(){
 glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();
glScaled(1.1,1.1,1.1);
glColor3f(0.0,0.7,0.8);
 glTranslated(0,-130,0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(250,300);
glVertex2f(250,450);
glEnd();

glColor3f(1.0,1.0,1.0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(250,300);
glVertex2f(250,200);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(200,200);
glVertex2d(250,300);
glEnd();

glPopMatrix();
glutSwapBuffers();

}

void step5()
{    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
glScaled(1.2,1.2,1.2);
    glColor3f(1.0,1.0,1.0);
     glTranslated(0,-130,0);
    glBegin(GL_POLYGON);
    glVertex2f(180,200);
    glVertex2f(235,200);
    glVertex2f(250,400);
    glEnd();

    glColor3f(0.0,0.7,0.8);
    glBegin(GL_POLYGON);
    glVertex2f(230, 180);
    glVertex2f(235,200);
    glVertex2f(250,400);
    glVertex2f(285,150);
   glEnd();

   glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(230,180);
glVertex2d(250,400);
glEnd();

    glPopMatrix();
   glutSwapBuffers();



}

void step6(){
 glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();
glScaled(1.3,1.3,1.3);
glColor3f(0.0,0.7,0.8);
 glTranslated(0,-130,0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(237,250);
glVertex2f(237,400);
glEnd();

glColor3f(0.0,0.7,0.8);
glBegin(GL_POLYGON);
glVertex2f(237,250);
glVertex2f(237,400);
glVertex2f(275,200);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(237,250);
glVertex2d(237,400);
glEnd();
glPopMatrix();
glutSwapBuffers();

}


void special1(){
   glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
glScaled(1.2,1.2,1.2);
    glColor3f(1.0,1.0,1.0);
     glTranslated(0,-130,0);
    glBegin(GL_POLYGON);
    glVertex2f(200,200);
    glVertex2f(200,400);
       glVertex2f(300,400);
   glVertex2f(300,200);

    glEnd();

        glPopMatrix();
    glutSwapBuffers();
}
void special2(){
  glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();


    glColor3f(1.0,1.0,1.0);
    glBegin(GL_POLYGON);
     glScaled(1.9,1.9,1.9);
       glTranslated(0,-130,0);
        glVertex2f(325,200);
        glVertex2f(350,250);
        glVertex2f(380,250);
        glVertex2f(400,200);
        glEnd();


  glColor3f(1.0,1.0,1.0);
  glBegin(GL_POLYGON);
  glScaled(1.9,1.9,1.9);
   glTranslated(0,130,0);
   glVertex2f(200,200);
   glVertex2f(200,250);
   glVertex2f(250,200);
   glEnd();

        glPopMatrix();
    glutSwapBuffers();


}

void special3(){
   glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
glScaled(1.7,1.7,1.7);
    glColor3f(0.8,0.0,0.4);
     glTranslated(-50,-80,0);
    glBegin(GL_POLYGON);
    glVertex2f(200,200);
      glVertex2f(225, 250);
  glVertex2f(250,200);
  glEnd();

        glPopMatrix();
    glutSwapBuffers();
}

void special4(){
   glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
glScaled(1.4,1.4,1.4);
    glColor3f(0.5,0.5,0.2);
     glTranslated(-200,-75,0);
    glBegin(GL_POLYGON);
   glVertex2f(380,250);
        glVertex2f(400,200);
        glVertex2f(500,200);
        glVertex2f(500,250);
        glEnd();

        glPopMatrix();
    glutSwapBuffers();

}

void special5(){
  glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
glScaled(1.5,1.5,1.5);
     glTranslated(-220,-75,0);
      glColor3f(1.0,1.0,1.0);
        glBegin(GL_POLYGON);
        glVertex2f(500,200);
        glVertex2f(500,250);
        glVertex2f(550,200);
        glEnd();
        glColor3f(0.5,0.5,0.2);
        glBegin(GL_POLYGON);
        glVertex2f(380,250);
        glVertex2f(400,200);
        glVertex2f(500,200);
        glVertex2f(500,250);
        glEnd();

        glColor3f(.8,0.0,0.4);
        glBegin(GL_POLYGON);
        glVertex2f(350,250);
        glVertex2f(365,300);
        glVertex2f(380,250);
        glEnd();


        glColor3f(1.0,1.0,1.0);
        glBegin(GL_POLYGON);
        glVertex2f(325,200);
        glVertex2f(350,250);
        glVertex2f(380,250);
        glVertex2f(400,200);
        glEnd();

    glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(350,250);
glVertex2d(380,250);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(380,250);
glVertex2d(400,200);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(500,200);
glVertex2d(500,250);
glEnd();



    glPopMatrix();
    glutSwapBuffers();

}


void copy1(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glScaled(1.2,1.2,1.2);
    glColor3f(1.0,1.0,1.0);
     glTranslated(0,-130,0);
    glBegin(GL_POLYGON);
    glVertex2f(200,200);
    glVertex2f(200,400);
   glVertex2f(300,400);
   glVertex2f(300,200);

    glEnd();
    glPopMatrix();
    glutSwapBuffers();
}
void copy2(){
  glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();
glScaled(1.2,1.2,1.2);


glColor3f(1.0,1.0,1.2);
   glTranslated(0,-130,0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(200,325);
glVertex2f(300,200);
glVertex2f(300,325);
glEnd();

glColor3f(0.7,0.56,0.2);
glBegin(GL_POLYGON);
glVertex2f(200,325);
glVertex2f(250,400);
glVertex2f(300,325);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(250,400);
glVertex2d(250,325);
glEnd();


 glColor3f(1.0,1.0,1.0);
       glBegin(GL_POLYGON);
           glVertex2f(200,325);
            glVertex2f(250,250);
            glVertex2f(300,325);
            glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(200,325);
glVertex2d(300,325);
glEnd();


glPopMatrix();
glutSwapBuffers();

}
void copy3(){
    glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();
glScaled(1.1,1.1,1.1);
glColor3f(0.7,0.56,0.2);
 glTranslated(0,-130,0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(250,300);
glVertex2f(250,450);
glEnd();

glColor3f(0.7,0.56,0.2);
glBegin(GL_POLYGON);
glVertex2f(300,200);
glVertex2f(250,300);
glVertex2f(250,450);
glEnd();

glColor3f(1.0,1.0,1.0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(250,300);
glVertex2f(300,200);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(250,450);
glVertex2d(250,300);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(200,200);
glVertex2d(250,300);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(300,200);
glVertex2d(250,300);
glEnd();


glPopMatrix();
glutSwapBuffers();

}
void copy4(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
glScaled(1.2,1.2,1.2);
    glColor3f(0.7,0.56,0.2);
     glTranslated(0,-130,0);
    glBegin(GL_POLYGON);
    glVertex2f(180,200);
    glVertex2f(235,200);
    glVertex2f(250,400);
    glEnd();

    glColor3f(0.4,0.1,0.3);
    glBegin(GL_POLYGON);
    glVertex2f(230, 180);
    glVertex2f(235,200);
    glVertex2f(250,400);
    glVertex2f(285,150);
   glEnd();

   glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(230,180);
glVertex2d(250,400);
glEnd();

    glPopMatrix();
   glutSwapBuffers();




}
void copy5(){
     glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();
glScaled(1.2,1.2,1.2);
 glTranslated(0,-130,0);

glColor3f(0.4,0.1,0.3);
glBegin(GL_POLYGON);
glVertex2f(200,200);

glVertex2f(250,400);
glVertex2f(250,250);
glEnd();

glColor3f(0.7,0.56,0.2);
glBegin(GL_POLYGON);
glVertex2f(250,250);
glVertex2f(250,400);

glVertex2f(300,200);
glEnd();



glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(250,250);
glVertex2d(250,400);

glEnd();

glPopMatrix();
glutSwapBuffers();
}


void  final1(){
 glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glScaled(1.2,1.2,1.2);
    glColor3f(1.0,1.0,1.0);
     glTranslated(0,-130,0);
    glBegin(GL_POLYGON);
    glVertex2f(200,200);
    glVertex2f(200,400);
   glVertex2f(300,400);
   glVertex2f(300,200);

    glEnd();
    glPopMatrix();
    glutSwapBuffers();
}
void final2(){
glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();
glScaled(1.2,1.2,1.2);


glColor3f(1.0,1.0,1.2);
   glTranslated(0,-130,0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(200,325);
glVertex2f(300,200);
glVertex2f(300,325);
glEnd();

glColor3f(1.0,0,1);
glBegin(GL_POLYGON);
glVertex2f(200,325);
glVertex2f(250,400);
glVertex2f(300,325);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(250,400);
glVertex2d(250,325);
glEnd();


 glColor3f(1.0,1.0,1.0);
       glBegin(GL_POLYGON);
           glVertex2f(200,325);
            glVertex2f(250,250);
            glVertex2f(300,325);
            glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(200,325);
glVertex2d(300,325);
glEnd();


glPopMatrix();
glutSwapBuffers();
}
void final3(){

      glClear(GL_COLOR_BUFFER_BIT);

glPushMatrix();

glScaled(1.2,1.2,1.2);
glColor3f(1.0,1.0,1.0);
 glTranslated(0,-130,0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(200,325);
glVertex2f(250,250);
glVertex2f(300,200);
glVertex2f(300,325);
glEnd();

glColor3f(1.0,0.0,1.0);
glBegin(GL_POLYGON);
glVertex2f(200,325);
glVertex2f(250,250);
glVertex2f(300,325);
glEnd();


glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(200,325);
glVertex2d(250,250);
glEnd();


glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(250,250);
glVertex2d(300,325);
glEnd();


glPopMatrix();
glutSwapBuffers();
}
void final4(){
    glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();
glScaled(1.3,1.3,1.3);
glColor3f(1,1,1.0);
 glTranslated(0,-130,0);
glBegin(GL_POLYGON);
glVertex2f(200,200);
glVertex2f(237,250);
glVertex2f(237,400);
glEnd();

glColor3f(1.0,0.0,1);
glBegin(GL_POLYGON);
glVertex2f(237,250);
glVertex2f(237,400);
glVertex2f(275,200);
glEnd();

glLineWidth(1.5);
glColor3f(0,0,0);
glBegin(GL_LINES);
glVertex2d(237,250);
glVertex2d(237,400);
glEnd();
glPopMatrix();
glutSwapBuffers();

}


void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch(key)
    {

    case 13:
        if(!flag)
        {
            flag=1;
            myInit1();
            day(0,0,true);

            glutKeyboardFunc(myKeyboardFunc);

            glutPostRedisplay();


        }
        break;
    case 'm':
        myinit2();
        break;

    case 'q' :

        Rectangle();
        glutPostRedisplay();
        break;


    case 'w' :


        triangle_over_rect();
        glutPostRedisplay();
        break;



    case 'e' :

        fold_triangle();
        glutPostRedisplay();
        break;


    case 'r' :

        folded_plane();
        glutPostRedisplay();
        break;

    case '1' :


        Rectangle2();
        glutPostRedisplay();
        break;

    case '2' :


        triangle_over_rect2();
        glutPostRedisplay();
        break;

    case '3' :


       step3();
        glutPostRedisplay();
        break;
case '4':


       step4();
        glutPostRedisplay();
        break;
case '5' :


       step5();
        glutPostRedisplay();
        break;
case '6' :

    step6();
        glutPostRedisplay();
        break;

case 'a':
    special1();
       glutPostRedisplay();
        break;
case 's':
    special2();
       glutPostRedisplay();
        break;
case 'd':
    special3();
       glutPostRedisplay();
        break;
case 'f':
    special4();
       glutPostRedisplay();
        break;
case 'g':
    special5();
       glutPostRedisplay();
        break;

case 'y':
    copy1();
glutPostRedisplay();
        break;
        case 'u':
    copy2();
glutPostRedisplay();
        break;
        case 'i':
    copy3();
glutPostRedisplay();
        break;
        case 'o':
    copy4();
glutPostRedisplay();
        break;
        case 'p':
    copy5();
glutPostRedisplay();
        break;


        case 'h':
            final1();
            glutPostRedisplay();
        break;
            case 'j':
            final2();
            glutPostRedisplay();
        break;
            case 'k':
            final3();
            glutPostRedisplay();
        break;
            case 'l':
            final4();
            glutPostRedisplay();
        break;



case 'b':
    day(0,0,true);
    glutPostRedisplay();
    break;
    }


}


void mouse(int btn,int state,int x,int y)
{
    if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        printf("%d\t%d\n",x,y);

        day(x,y,true);
          flag1=0;
    }
   else  if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){

    day(x,y,false);

    }
}

void myinit()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50,50,-50,25,-25,5);
}



int
main(int argc, char *argv[])
{

    /* use multisampling if available */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(1800,650);
    glutInitWindowPosition(0,0);
    glutCreateWindow("PAPER PLANE SIMULATION");
    myinit();
    glutDisplayFunc(mydisplay);
    glutKeyboardFunc(myKeyboardFunc);
    glutMouseFunc(mouse);
    /* start event processing */
    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}
