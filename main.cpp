#include <cmath>

#include "canvas.h"
#include "open_gl.h"

const int WIDTH = 256;
const int HEIGHT = 256;
const float PIXEL_SIZE = 3;

Canvas canvas = Canvas(WIDTH, HEIGHT);
OpenGL openGL = OpenGL(&canvas, PIXEL_SIZE, WINDOWED);

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(openGL.window, true);
    }
}

int main()
{
    glfwSetWindowPos(openGL.window, 100, 50);
    glfwSetKeyCallback(openGL.window, keyCallback);

    Canvas original_image = Canvas("images/andi.png");

    float step = (float)original_image.height / canvas.height;

    int* r = new int[canvas.width * canvas.height];
    int* g = new int[canvas.width * canvas.height];
    int* b = new int[canvas.width * canvas.height];

    for (int y = 0; y < canvas.height; y++) {
        for (int x = 0; x < canvas.width; x++) {
            Color pixel(original_image.pixels[int(x * step) + int(y * step) * original_image.width]);

            pixel = Color(pixel.grayscale());

            int index = x + y * canvas.width;
            r[index] = pixel.getRed();
            g[index] = pixel.getGreen();
            b[index] = pixel.getBlue();
        }
    }

    int factor = 1;

    while (!glfwWindowShouldClose(openGL.window)) {
        canvas.clearCanvas();

        for (int y = 0; y < canvas.height - 1; y++) {
            for (int x = 1; x < canvas.width - 1; x++) {
                int index = x + y * canvas.width;

                float old_r = r[index];
                float old_g = g[index];
                float old_b = b[index];

                int new_r = int(round(factor * old_r / 255) * (255 / factor));
                int new_g = int(round(factor * old_g / 255) * (255 / factor));
                int new_b = int(round(factor * old_b / 255) * (255 / factor));

                r[index] = new_r;
                g[index] = new_g;
                b[index] = new_b;

                int err_r = old_r - new_r;
                int err_g = old_g - new_g;
                int err_b = old_b - new_b;

                index = (x + 1) + (y) * canvas.width;
                r[index] = int(r[index] + err_r * 7 / 16.0f);
                g[index] = int(g[index] + err_g * 7 / 16.0f);
                b[index] = int(b[index] + err_b * 7 / 16.0f);

                index = (x - 1) + (y + 1) * canvas.width;
                r[index] = int(r[index] + err_r * 3 / 16.0f);
                g[index] = int(g[index] + err_g * 3 / 16.0f);
                b[index] = int(b[index] + err_b * 3 / 16.0f);

                index = (x) + (y + 1) * canvas.width;
                r[index] = int(r[index] + err_r * 5 / 16.0f);
                g[index] = int(g[index] + err_g * 5 / 16.0f);
                b[index] = int(b[index] + err_b * 5 / 16.0f);

                index = (x + 1) + (y + 1) * canvas.width;
                r[index] = int(r[index] + err_r * 1 / 16.0f);
                g[index] = int(g[index] + err_g * 1 / 16.0f);
                b[index] = int(b[index] + err_b * 1 / 16.0f);
            }
        }

        for (int i = 0; i < canvas.width * canvas.height; i++) {
            canvas.pixels[i] = Color(r[i], g[i], b[i]).value;
        }

        openGL.update(&canvas);
    }

    return EXIT_SUCCESS;
}
