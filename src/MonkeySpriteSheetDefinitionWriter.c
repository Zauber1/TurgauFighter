#include <stdio.h>
#include "MonkeySpriteSheetDefinitionWriter.h"

void runWriter(int max_y, int min_y, int max_x, int min_x, int x_distance_between_pictures) {
    printf("[DEBUG] runWriter called with parameters:\n");
    printf("        min_x: %d, max_x: %d, step: %d\n", min_x, max_x, x_distance_between_pictures);
    printf("        min_y: %d, max_y: %d\n", min_y, max_y);

    FILE *csvFile = fopen("../src/MonkeySpriteSheetDefinition.csv", "w");

    if (csvFile == NULL) {
        perror("[DEBUG ERROR] Failed to open MonkeySpriteSheetDefinition.csv");
        return;
    }

    printf("[DEBUG] File opened successfully. Writing headers...\n");

    // Write header/file path reference and animation label
    fprintf(csvFile, "assets/Textures/SpriteSheets/MonkeySpriteSheet.png\n\n");
    fprintf(csvFile, "./MonkeySpriteSheetDefinition.csv\n");

    int frame_count = 0;

    // Loop through each frame position along the horizontal row
    for (int current_x = min_x; current_x < max_x; current_x += x_distance_between_pictures) {
        int x1 = current_x;
        int y1 = min_y;

        int x2 = current_x + x_distance_between_pictures;
        int y2 = max_y;

        fprintf(csvFile, "%d,%d,%d,%d\n", x1, y1, x2, y2);

        frame_count++;
        printf("[DEBUG] Wrote Frame %d: x1=%d, y1=%d, x2=%d, y2=%d\n", frame_count, x1, y1, x2, y2);
    }

    if (frame_count == 0) {
        printf("[DEBUG WARNING] Loop did not execute! Check if min_x (%d) is greater than max_x (%d).\n", min_x, max_x);
    }

    fprintf(csvFile, ";\n");
    fclose(csvFile);

    printf("[DEBUG] Finished writing %d frame(s). File closed.\n", frame_count);
}