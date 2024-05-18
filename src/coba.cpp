#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>

// Sudut rotasi
float angle = 0.0f;
GLuint texture1, texture2;

void loadTexture(const char* filename, GLuint& texture) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        std::cout << "Texture loaded: " << filename << " " << width << "x" << height << ", channels: " << nrChannels << std::endl;
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
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }
}

// Fungsi untuk menginisialisasi OpenGL
void init() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //background window
    glEnable(GL_DEPTH_TEST);          // Aktifkan depth testing
    glEnable(GL_TEXTURE_2D);          // Aktifkan texturing

    // Pengaturan pencahayaan
    glEnable(GL_LIGHTING);                    // Aktifkan pencahayaan
    glEnable(GL_LIGHT0);                      // Aktifkan sumber cahaya 0

    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 2.0, 5.0, 5.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_COLOR_MATERIAL);              // Aktifkan pengaturan material
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 50);

    // Muat kedua tekstur
    loadTexture("images/untirta.png", texture1);
    loadTexture("images/informatika.jpg", texture2);
}

// Fungsi untuk menggambar limas dengan alas segitiga sama sisi
void drawEquilateralTriangularPyramid() {
    glBindTexture(GL_TEXTURE_2D, texture1);

    glBegin(GL_TRIANGLES);

    // Sisi depan
    glNormal3f(0.0, 0.5, 0.5); // Normal untuk sisi depan
    glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.866, 0.0, 0.5); // -sqrt(3)/2, 0, 0.5
    glTexCoord2f(1.0, 0.0); glVertex3f(0.866, 0.0, 0.5);  // sqrt(3)/2, 0, 0.5

    // Sisi kanan
    glNormal3f(0.5, 0.5, -0.5); // Normal untuk sisi kanan
    glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.866, 0.0, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, -1.0); // 0, 0, -1

    // Sisi kiri
    glNormal3f(-0.5, 0.5, -0.5); // Normal untuk sisi kiri
    glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.866, 0.0, 0.5);

    glEnd();

    glBegin(GL_TRIANGLES);
    // Alas segitiga sama sisi
    glNormal3f(0.0, -1.0, 0.0); // Normal untuk alas
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.866, 0.0, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.866, 0.0, 0.5);
    glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 0.0, -1.0);
    glEnd();
}

// Fungsi untuk menggambar balok dengan atap trapesium
void drawTrapezoidOnBlock() {
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBegin(GL_QUADS);

    // Balok bagian bawah
    // Sisi depan
    glNormal3f(0.0, 0.0, 1.0); // Normal untuk sisi depan
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -0.5, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.0, -0.5, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 0.5, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 0.5, 1.0);

    // Sisi belakang
    glNormal3f(0.0, 0.0, -1.0); // Normal untuk sisi belakang
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -0.5, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.0, -0.5, -1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 0.5, -1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 0.5, -1.0);

    // Sisi kiri
    glNormal3f(-1.0, 0.0, 0.0); // Normal untuk sisi kiri
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -0.5, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-2.0, -0.5, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(-2.0, 0.5, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 0.5, -1.0);

    // Sisi kanan
    glNormal3f(1.0, 0.0, 0.0); // Normal untuk sisi kanan
    glTexCoord2f(0.0, 0.0); glVertex3f(2.0, -0.5, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.0, -0.5, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 0.5, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(2.0, 0.5, -1.0);

    // Sisi atas
    glNormal3f(0.0, 1.0, 0.0); // Normal untuk sisi atas
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, 0.5, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.0, 0.5, -1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 0.5, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 0.5, 1.0);

    // Sisi bawah
    glNormal3f(0.0, -1.0, 0.0); // Normal untuk sisi bawah
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -0.5, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.0, -0.5, -1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.0, -0.5, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, -0.5, 1.0);

    glEnd();

    // Atap trapesium
    glBegin(GL_QUADS);

    // Sisi depan atap trapesium
    glNormal3f(0.0, 0.5, 1.0); // Normal untuk sisi depan atap trapesium
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.5, 0.5, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.5, 0.5, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.5, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.5, 1.0);

    // Sisi belakang atap trapesium
    glNormal3f(0.0, 0.5, -1.0); // Normal untuk sisi belakang atap trapesium
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.5, 0.5, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.5, 0.5, -1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.5, -1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.5, -1.0);

    // Sisi kiri atap trapesium
    glNormal3f(-1.0, 0.5, 0.0); // Normal untuk sisi kiri atap trapesium
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.5, 0.5, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.5, 0.5, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 1.5, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.5, -1.0);

    // Sisi kanan atap trapesium
    glNormal3f(1.0, 0.5, 0.0); // Normal untuk sisi kanan atap trapesium
    glTexCoord2f(0.0, 0.0); glVertex3f(1.5, 0.5, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.5, 0.5, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.5, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.5, -1.0);

    glEnd();

    glBegin(GL_TRIANGLES);

    // Sisi atas atap trapesium
    glNormal3f(0.0, 1.0, 0.0); // Normal untuk sisi atas atap trapesium
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.5, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.5, -1.0);
    glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 2.0, 0.0);

    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.5, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.5, 1.0);
    glTexCoord2f(0.5, 1.0); glVertex3f(0.0, 2.0, 0.0);

    glEnd();
}

// Fungsi display untuk menggambar ke layar
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 2.0, 5.0,   // Posisi kamera
              0.0, 0.0, 0.0,   // Titik yang dilihat
              0.0, 1.0, 0.0);  // Arah atas

    glRotatef(angle, 0.0, 1.0, 0.0); // Rotasi objek

    // Gambar limas segitiga sama sisi
    glPushMatrix();
    glTranslatef(-2.0, 0.0, 0.0); // Pindahkan posisi ke kiri
    drawEquilateralTriangularPyramid();
    glPopMatrix();

    // Gambar balok dengan atap trapesium
    glPushMatrix();
    glTranslatef(2.0, 0.0, 0.0); // Pindahkan posisi ke kanan
    drawTrapezoidOnBlock();
    glPopMatrix();

    glutSwapBuffers();
}

// Fungsi reshape untuk menangani perubahan ukuran jendela
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Fungsi timer untuk animasi
void timer(int value) {
    angle += 0.5f;
    if (angle > 360.0f) {
        angle -= 360.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // Memanggil kembali fungsi timer dalam 16 milidetik
}

// Fungsi utama
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rotating 3D Models");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}