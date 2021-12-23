#include <stdio.h>
#include <stdlib.h>

int main() {
    int x1, x2, y1, y2;
    FILE *fp = fopen("input/day-17.input", "r");
    fscanf(fp, "target area: x=%d..%d, y=%d..%d", &x1, &x2, &y1, &y2);
    fclose(fp);
    
    int part1 = -1;
    unsigned int part2 = 0;
    for (int yv = 1000; yv >= -1000; yv--) {
        for (int xv = -1000; xv <= 1000; xv++) {
            int xp = 0;
            int yp = 0;
            int xvv = xv;
            int yvv = yv;
            int maxy = yp;
            while (1) {
                if ((xvv > 0 && xp > x2) || (xvv < 0 && xp < x1) || yp < y1)
                    break;
                if (yp > maxy)
                    maxy = yp;
                if (xp >= x1 && xp <= x2 && yp >= y1 && yp <= y2) {
                    if (part1 == -1)
                        part1 = maxy;
                    part2++;
                    break;
                }
                xp += xvv;
                yp += yvv;
                xvv += (xvv > 0) * -1 + (xvv < 0) * 1;
                yvv -= 1;
            }
        }
    }

    printf("PART 1: %d\n", part1);
    printf("PART 2: %u\n", part2);

    return 0;
}