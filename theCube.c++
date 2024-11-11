#include <iostream>
#include <cmath>
#include <cstring>

// Screen size parameters
int screen_width = 80;
int screen_height = 24;
float z_buffer[1760];    // Depth buffer
char display_buffer[1760]; // Character buffer

// Rotation angles
float A = 0.0, B = 0.0;

// Cube size and scale
float cube_size = 1.5; // Scale of the cube

// Trigonometric functions
double sin(), cos();

// Function to render the cube
void renderCube() 
{
    // Hide the cursor using ANSI escape sequence
    std::cout << "\x1b[?25l";
    
    // Main loop for rendering
    while (true) {
        // Clear buffers
        std::memset(display_buffer, ' ', sizeof(display_buffer));
        std::memset(z_buffer, 0, sizeof(z_buffer));

        // Loop through cube faces (front, back, top, bottom, left, right)
        for (float cube_x = -cube_size; cube_x < cube_size; cube_x += 0.1) {
            for (float cube_y = -cube_size; cube_y < cube_size; cube_y += 0.1) {
                
                // Loop through depth (front and back of the cube)
                for (float cube_z = -cube_size; cube_z < cube_size; cube_z += 0.1) {

                    // Rotation equations (around Y and X axes)
                    float sinA = sin(A), cosA = cos(A);
                    float sinB = sin(B), cosB = cos(B);

                    // Apply 3D rotation around the Y and X axes
                    float x = cube_x * cosA - cube_z * sinA;
                    float z = cube_x * sinA + cube_z * cosA;
                    float y = cube_y * cosB - z * sinB;
                    z = cube_y * sinB + z * cosB;

                    // Projection (3D to 2D)
                    float distance = 5.0;  // Distance from the viewer
                    float inv_z = 1 / (z + distance); // Perspective division

                    int x_screen = (int)(screen_width / 2 + 30 * x * inv_z); 
                    int y_screen = (int)(screen_height / 2 + 15 * y * inv_z);

                    // Depth check and character assignment
                    int buffer_index = x_screen + screen_width * y_screen;
                    if (buffer_index >= 0 && buffer_index < 1760 && inv_z > z_buffer[buffer_index]) {
                        z_buffer[buffer_index] = inv_z;
                        display_buffer[buffer_index] = '*';  // Cube surface symbol
                    }
                }
            }
        }

        // Print the frame
        std::cout << "\x1b[H"; // Move cursor to the top of the screen
        for (int k = 0; k < 1760; k++) {
            putchar(k % screen_width ? display_buffer[k] : 10); // Print characters or newlines
        }

        // Increment rotation angles for next frame
        A += 0.04;
        B += 0.03;
    }

    // Show the cursor back after the loop
    std::cout << "\x1b[?25h";
}

int main() {
    // Clear the screen
    std::cout << "\x1b[2J";
    // Start the cube rendering loop
    renderCube();
    return 0;
}
