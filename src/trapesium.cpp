#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>

GLuint texture;
float horizontalAngle = 0.0f;
float verticalAngle = 0.0f;
bool horizontalRotation = true;
bool pauseRotation = false;
int pauseCounter = 0;
const int pauseDuration = 60; // duration to pause in frames

const float sqrt3 = std::sqrt(3.0f);
const float halfSqrt3 = sqrt3 / 2.0f;
const float baseHeight = halfSqrt3; // Height of equilateral triangle with side length 1

void loadTexture() {
    int width, height, nrChannels;
    unsigned char* data = stbi_load("images/informatika.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        std::cout << "Texture loaded: " << width << "x" << height << ", channels: " << nrChannels << std::endl;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
}


void init() {
    glClearColor(0.5, 0.5, 0.5, 1.0); // Background color

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glEnable(GL_TEXTURE_2D); // Enable texturing

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Define light properties
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // Define material properties
    GLfloat materialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat materialShininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

    loadTexture();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 5.0, 10.0,  // Eye position
              0.0, 0.0, 0.0,   // Look at position
              0.0, 1.0, 0.0);  // Up vector

    if (!pauseRotation) {
        if (horizontalRotation) {
            horizontalAngle += 1.0f;
            if (horizontalAngle >= 360.0f) {
                horizontalAngle = 0.0f;
                horizontalRotation = false;
                pauseRotation = true;
            }
        } else {
            verticalAngle += 1.0f;
            if (verticalAngle >= 360.0f) {
                verticalAngle = 0.0f;
                horizontalRotation = true;
                pauseRotation = true;
            }
        }
    } else {
        pauseCounter++;
        if (pauseCounter >= pauseDuration) {
            pauseCounter = 0;
            pauseRotation = false;
        }
    }

    glRotatef(horizontalAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(verticalAngle, 1.0f, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture);

    // Set material properties (again, in case they were reset)
    GLfloat materialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat materialShininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

    // Draw base of the house (a rectangular prism)
    glBegin(GL_QUADS);
        // Front face
        glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -0.5, 1.5);
        glTexCoord2f(1.0, 0.0); glVertex3f(2.0, -0.5, 1.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 0.5, 1.5);
        glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 0.5, 1.5);

        // Back face
        glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -0.5, -1.5);
        glTexCoord2f(1.0, 0.0); glVertex3f(-2.0, 0.5, -1.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 0.5, -1.5);
        glTexCoord2f(0.0, 1.0); glVertex3f(2.0, -0.5, -1.5);

        // Top face
        glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, 0.5, -1.5);
        glTexCoord2f(1.0, 0.0); glVertex3f(-2.0, 0.5, 1.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 0.5, 1.5);
        glTexCoord2f(0.0, 1.0); glVertex3f(2.0, 0.5, -1.5);

        // Bottom face
        glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -0.5, -1.5);
        glTexCoord2f(1.0, 0.0); glVertex3f(2.0, -0.5, -1.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(2.0, -0.5, 1.5);
        glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, -0.5, 1.5);

        // Right face
        glTexCoord2f(0.0, 0.0); glVertex3f(2.0, -0.5, -1.5);
        glTexCoord2f(1.0, 0.0); glVertex3f(2.0, 0.5, -1.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 0.5, 1.5);
        glTexCoord2f(0.0, 1.0); glVertex3f(2.0, -0.5, 1.5);

        // Left face
        glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -0.5, -1.5);
        glTexCoord2f(1.0, 0.0); glVertex3f(-2.0, -0.5, 1.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(-2.0, 0.5, 1.5);
        glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 0.5, -1.5);
    glEnd();

    // Draw roof of the house (a trapezoidal prism)
    glBegin(GL_QUADS);
        // Front face
        glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, 0.5, 1.5);
        glTexCoord2f(1.0, 0.0); glVertex3f(2.0, 0.5, 1.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.5, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.5, 0.0);

        // Back face
        glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, 0.5, -1.5);
        glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, 1.5, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.5, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(2.0, 0.5, -1.5);

        // Left face
        glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, 0.5, -1.5);
        glTexCoord2f(1.0, 0.0); glVertex3f(-2.0, 0.5, 1.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 1.5, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.5, 0.0);

        // Right face
        glTexCoord2f(0.0, 0.0); glVertex3f(2.0, 0.5, 1.5);
        glTexCoord2f(1.0, 0.0); glVertex3f(2.0, 0.5, -1.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.5, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.5, 0.0);
    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // Approximately 60 frames per second
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D House Model");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0); // Start the timer

    glutMainLoop();
    return 0;
}