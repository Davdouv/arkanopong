#include <GL/gl.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <stdio.h>

#include "bool.h"
#include "primitives.h"

#define SEGMENTS 32

/** Fonctions de dessin canonique. **/

void drawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);

        glVertex2f(x1, y1);
        glVertex2f(x2, y2);

    glEnd();
}

void drawSquare()
{
    glBegin(GL_QUADS);

        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, 0.5);
        glVertex2f(0.5, -0.5);
        glVertex2f(-0.5, -0.5);

    glEnd();
}

void drawSquareTexture()
{
    glBegin(GL_QUADS);

        glTexCoord2f(0, 0);
        glVertex2f(-0.5,0.5);

        glTexCoord2f(1, 0);
        glVertex2f(0.5,0.5);

        glTexCoord2f(1, 1);
        glVertex2f(0.5,-0.5);

        glTexCoord2f(0, 1);
        glVertex2f(-0.5,-0.5);

    glEnd();
}

void drawSquareBorder()
{
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, 0.5);

        glVertex2f(0.5, -0.5);
        glVertex2f(-0.5, -0.5);

    glEnd();
}

void drawRoundedSquare(bool full, float radius)
{
    float i;
    glBegin(full ? GL_POLYGON : GL_LINE_LOOP);
    for (i = 0; i <= (SEGMENTS/4); i++)
    {
        glVertex2f(0.5-radius+cos(i*(M_PI/(SEGMENTS/2)))*radius, 0.5-radius+sin(i*(M_PI/(SEGMENTS/2)))*radius);
    }
    for (i = (SEGMENTS/4); i <= 2*(SEGMENTS/4); i++)
    {
        glVertex2f(-0.5+radius+cos(i*(M_PI/(SEGMENTS/2)))*radius, 0.5-radius+sin(i*(M_PI/(SEGMENTS/2)))*radius);
    }
    for (i = 2*(SEGMENTS/4); i <= 3*(SEGMENTS/4); i++)
    {
        glVertex2f(-0.5+radius+cos(i*(M_PI/(SEGMENTS/2)))*radius, -0.5+radius+sin(i*(M_PI/(SEGMENTS/2)))*radius);
    }
    for (i = 3*(SEGMENTS/4); i <= SEGMENTS; i++)
    {
        glVertex2f(0.5-radius+cos(i*(M_PI/(SEGMENTS/2)))*radius, -0.5+radius+sin(i*(M_PI/(SEGMENTS/2)))*radius);
    }
    glEnd();
}

void drawCircle()
{
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i <= SEGMENTS; i++)
    {
        glVertex2f(cos(i*(M_PI/(SEGMENTS/2))), sin(i*(M_PI/(SEGMENTS/2))));
    }
    glEnd();
}

void drawCircleTexture()
{
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i <= SEGMENTS; i++)
    {
        glVertex2f(cos(i*(M_PI/(SEGMENTS/2))), sin(i*(M_PI/(SEGMENTS/2))));
    }
    glEnd();
}

int textLength(char * text)
{
    int length = 0;
    while(text[length])
        length++;
    return length;
}

/* Size correspond à la police (de 0 à 6) */
void drawText(int x, int y, char* txt, unsigned int size)
{
    int i;
    if(txt!=NULL)
    {   
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glLoadIdentity();
    
        int len = textLength(txt);
        glRasterPos2f(8+x-5*len, y+5);

        void * font[7] = {
                    GLUT_BITMAP_8_BY_13,
                    GLUT_BITMAP_9_BY_15,
                    GLUT_BITMAP_TIMES_ROMAN_10,
                    GLUT_BITMAP_TIMES_ROMAN_24,
                    GLUT_BITMAP_HELVETICA_10,
                    GLUT_BITMAP_HELVETICA_12,
                    GLUT_BITMAP_HELVETICA_18
                };

        for (i = 0; i < len; i++)
        {
            glutBitmapCharacter(font[size], txt[i]);
        }
    
        glPopMatrix();
        glMatrixMode( GL_MODELVIEW );
    }
}

void drawButton(int x, int y, char* txt, bool select)
{
    glPushMatrix();
        glTranslatef(x, y, 1);
        glScalef(200, 100, 1);
        float color = select ? 1.0 : 0.0;
        glColor3f(color, color, color);
        drawRoundedSquare(1, 0.05);
        glColor3f(1.0, 0.0, 0.0);
        drawRoundedSquare(0, 0.05);
        drawText(x,y,txt, 1);
    glPopMatrix();
    glColor3f(1.0, 1.0, 1.0);
}

void drawGradientSquare(int timer)
{
    float time = (float) timer/100.0;
    glBegin(GL_QUADS);
    glColor4f(1.0,0.0,0.0,0.0);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, 0.5);
    glColor4f(1.0,0.0,0.0,time);
        glVertex2f(0.5, -0.5);
        glVertex2f(-0.5, -0.5);

    glEnd();
}

// Create color to change brick texture color */
Color createColor(int code)
{
  Color color;
  switch (code)
  {
    case 1 :    // RED
      color.r = 255.0/255.0;
      color.g = 51.0/255.0;
      color.b = 51.0/255.0;
      break;
    case 2 :    // GREEN
      color.r = 0.0;
      color.g = 204.0/255.0;
      color.b = 0.0;
      break;
    case 3 :    // BLUE
      color.r = 0.0;
      color.g = 102.0/255.0;
      color.b = 204.0/255.0;
      break;
    case 4 :    // PURPLE
      color.r = 127.0/255.0;
      color.g = 0.0;
      color.b = 255.0/255.0;
      break;
    case 5 :    // YELLOW
      color.r = 255.0/255.0;
      color.g = 255.0/255.0;
      color.b = 0.0/255.0;
      break;
    default :
        color.r = 1.0;
        color.g = 1.0;
        color.b = 1.0;
        break;
  }
  return color;
}
