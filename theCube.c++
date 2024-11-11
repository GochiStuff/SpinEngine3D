#include <iostream>
#include <cmath>
#include <cstring>
#include <chrono>
#include <thread>

// Variables
int screenWidth = 60;
int screenHeight = 28;
float depth_buffer[60*28];
char display_buffer[60*28];

float cube_size = 1.5;
float precision = 0.1; // precision

float A = 0, B = 0; // angles

// Light
int lightPos[3] = {0 , 3, 0};
unsigned char intensity_levels[] = {176, 177, 178, 178, 219}; 
// unsigned char intensity_levels[] = {'!' ,'$' ,'%' , '#' ,'*'}; 

void getCubeNormals(float face_normal[6][3] , float cubesize) {

    face_normal[0][0] = 1.0; face_normal[0][1] = 0.0; face_normal[0][2] = 0.0;  // right
    face_normal[1][0] = -1.0; face_normal[1][1] = 0.0; face_normal[1][2] = 0.0; // left
    face_normal[2][0] = 0.0; face_normal[2][1] = 1.0; face_normal[2][2] = 0.0;  // top
    face_normal[3][0] = 0.0; face_normal[3][1] = -1.0; face_normal[3][2] = 0.0; // bottom
    face_normal[4][0] = 0.0; face_normal[4][1] = 0.0; face_normal[4][2] = 1.0;  // front
    face_normal[5][0] = 0.0; face_normal[5][1] = 0.0; face_normal[5][2] = -1.0; // back
}

float dotProduct(float ax, float ay, float az, float bx, float by, float bz) {
    return ax * bx + ay * by + az * bz;
}

void makeCube() {
    std::memset(display_buffer, ' ', sizeof(display_buffer));
    std::fill_n(depth_buffer, screenWidth * screenHeight, 0.0f);

    float face_normal[6][3];
    getCubeNormals(face_normal, cube_size);

    for (float cube_x = -cube_size; cube_x < cube_size; cube_x += precision) {
        for (float cube_y = -cube_size; cube_y < cube_size; cube_y += precision) {
            for (float cube_z = -cube_size; cube_z < cube_size; cube_z += precision) {
                // Rotation
                float sinA = sin(A), cosA = cos(A);
                float sinB = sin(B), cosB = cos(B);

                float x = cube_x * cosA - cube_z * sinA;
                float z = cube_x * sinA + cube_z * cosA;
                float y = cube_y * cosB - z * sinB;
                z = cube_y * sinB + z * cosB;

                // Light calculation
                float dx = lightPos[0] - x;
                float dy = lightPos[1] - y;
                float dz = lightPos[2] - z;

                float mag = std::sqrt(dx*dx + dy*dy + dz*dz);
                dx /= mag;
                dy /= mag;
                dz /= mag;

                char printch = ' ';

                // Determine intensity level based on distance to light
                if (mag < 2.0) 
                    printch = intensity_levels[4];
                else if (mag < 4.0) 
                    printch = intensity_levels[3];
                else if (mag < 6.0) 
                    printch = intensity_levels[2];
                else if (mag < 8.0) 
                    printch = intensity_levels[1];
                else 
                    printch = intensity_levels[0];

                // Check each face of the cube
                bool on_face = false;
                for (int i = 0; i < 6; i++) {
                    float dot = dotProduct(face_normal[i][0], face_normal[i][1], face_normal[i][2], 
                                           cube_x/cube_size, cube_y/cube_size, cube_z/cube_size);
                    if (std::abs(dot) > 0.99) {
                        on_face = true;
                        break;
                    }
                }

                if (on_face) {
                    // Projection and rendering
                    float disCamera = 4.5;
                    float inv_z = 1/(z + disCamera);
                    int x_screen = (int)(screenWidth/2 + 30*x*inv_z);
                    int y_screen = (int)(screenHeight/2 + 15*y*inv_z);

                    // Buffer 
                    int buffer_index = x_screen + screenWidth * y_screen;
                //     if (buffer_index >= 0 && buffer_index < screenHeight * screenWidth && inv_z > depth_buffer[buffer_index]) {
                //     }
                        depth_buffer[buffer_index] = inv_z;
                        display_buffer[buffer_index] = printch;
                }
            }
        }
    }

    // Output                
    std::cout << "\x1b[H"; 
    for (int i = 0; i < screenWidth * screenHeight; i++) {
        putchar(i % screenWidth ? display_buffer[i] : '\n');
    }
}

int main() {
    std::cout << "\x1b[2J"; 
    std::cout << "\x1b[?25l";

    while (true) {
        makeCube();
        A += 0.05;
        B += 0.03;

        // Add a small delay to control frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(33));  // Approx. 30 FPS
    }

    return 0;
}
