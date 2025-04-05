#include "canvas.h"

void Canvas::floodFill(int x, int y, Color color)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return;

    Color targetColor, currPixel;
    if (!getPixel(x, y, &targetColor) || targetColor.value == color.value) return;

    int *next = new int[width * height * 2];
    int index = 0;

    int dir[] = {0,1, 1,0, 0,-1, -1,0};

    setPixel(x, y, color);
    while (true) {
        for (int i = 0; i < 8; i += 2) {
            if (getPixel(x + dir[i], y + dir[i + 1], &currPixel) && currPixel.value == targetColor.value) {
                setPixel(x + dir[i], y + dir[i + 1], color);
                next[index] = x + dir[i];
                next[index + 1] = y + dir[i + 1];
                index += 2;
            }
        }

        if (index == 0) break;
        index -= 2;
        x = next[index];
        y = next[index + 1];
    }

    delete[] next;
}

void Canvas::spanFill(int x, int y, Color color)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return;

    Color targetColor, currPixel;
    if (!getPixel(x, y, &targetColor) || targetColor.value == color.value) return;

    int *next = new int[width * height * 2];
    int index = 0;

    next[index] = x;
    next[index + 1] = x;
    next[index + 2] = y;
    next[index + 3] = 1;
    index += 4;

    int x1 = x;
    int x2 = x;
        y  = y - 1;
    int dy = -1;

    while (true) {
        x = x1;
        if (getPixel(x, y, &currPixel) && currPixel.value == targetColor.value) {
            while (getPixel(x - 1, y, &currPixel) && currPixel.value == targetColor.value) {
                setPixel(x - 1, y, color);
                x--;
            }
            if (x < x1) {
                next[index] = x;
                next[index + 1] = x1 - 1;
                next[index + 2] = y - dy;
                next[index + 3] = -dy;
                index += 4;
            }
        }

        while (x1 <= x2) {
            while (getPixel(x1, y, &currPixel) && currPixel.value == targetColor.value) {
                setPixel(x1, y, color);
                x1++;
            }
            if (x1 > x) {
                next[index] = x;
                next[index + 1] = x1 - 1;
                next[index + 2] = y + dy;
                next[index + 3] = dy;
                index += 4;
            }
            if (x1 - 1 > x2) {
                next[index] = x2 + 1;
                next[index + 1] = x1 - 1;
                next[index + 2] = y - dy;
                next[index + 3] = -dy;
                index += 4;
            }
            x1++;
            while (x1 < x2 && getPixel(x1, y, &currPixel) && currPixel.value != targetColor.value) x1++;
            x = x1;
        }
        if (index == 0) break;
        index -= 4;
        x1 = next[index];
        x2 = next[index + 1];
        y  = next[index + 2];
        dy = next[index + 3];
    }

    delete[] next;
}
