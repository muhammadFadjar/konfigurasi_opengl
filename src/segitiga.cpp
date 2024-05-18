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
    unsigned char* data = stbi_load("images/untirta.png", &width, &height, &nrChannels, 0);
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
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Set up lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Posisi lighting
    GLfloat light0_position[] = { -1.0f, -2.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    // Tipe lighting
    GLfloat ambient_light[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat diffuse_light[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    // Keterangan material
    GLfloat mat_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    loadTexture();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,   // Eye position
              0.0, 0.0, 0.0,   // Look at position
              0.0, 1.0, 0.0);  // Up direction

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

    glBegin(GL_TRIANGLES);

    // Front face
    glTexCoord2f(0.5, 1.0); glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5f, 0.0f, baseHeight / 3);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5f, 0.0f, baseHeight / 3);

    // Right face
    glTexCoord2f(0.5, 1.0); glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.5f, 0.0f, baseHeight / 3);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0f, 0.0f, -2 * baseHeight / 3);

    // Left face
    glTexCoord2f(0.5, 1.0); glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0f, 0.0f, -2 * baseHeight / 3);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5f, 0.0f, baseHeight / 3);

    glEnd();

    glBegin(GL_POLYGON);

    // Bottom face (base)
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5f, 0.0f, baseHeight / 3);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5f, 0.0f, baseHeight / 3);
    glTexCoord2f(0.5, 1.0); glVertex3f(0.0f, 0.0f, -2 * baseHeight / 3);

    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 200.0);
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
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Textured Equilateral Triangular Pyramid with Rotation");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}