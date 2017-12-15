/*
 * Author: Francisco Gallardo
 * Language: C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int byteValue;
unsigned char imageHeaderDataArray[54];

void printImageWidthAndHeight(FILE *imageFile) ;

int main(int argc, char *arcv[]) {
    char thresholdCommandString[4] = "-t";
    char optionsCommandString[4] = "-o";
    char helpCommandString[4] = "-h";
    char negativeFilterCommandString[4] = "-n";
    char grayScaleFilterCommandString[4] = "-g";
    static const int pixelColourWhite = 255;
    static const int pixelColourBlack = 000;
    int pixelRedIndex = 0;
    int pixelBlueIndex = 1;
    int pixelGreenIndex =2;
    char singleCharacter;
    long imageFileSize;

    // If the second argument is equal to "-h" print the help message.
    if (strncmp(arcv[1], helpCommandString, 20) == 0) {
        FILE *helpFile = fopen("-h.txt", "r");

        int x;
        while ((x = fgetc(helpFile)) != EOF ) {
            printf("%c", x);
        }
        fclose(helpFile);
    } else if (argc == 2) {

        FILE *imageSourceFile = fopen(arcv[1], "r");

        if (imageSourceFile != NULL) {
            printImageWidthAndHeight(imageSourceFile);
            // Close the source image file.
            fclose(imageSourceFile);
        }

    } else if (strncmp(arcv[1], thresholdCommandString, 4) == 0) {
        if (argc == 5) {
            FILE *imageSourceFile = fopen(arcv[4], "r");
            float thresholdValueRequired = (float) atof(arcv[2]);

            if (imageSourceFile != NULL) {

                printImageWidthAndHeight(imageSourceFile);

                // Open the file at the fourth position in "writable" mode.
                FILE *imageOutputFile = fopen(arcv[3], "w+");
                // Move to the beginning of the file.
                fseek(imageSourceFile, 0, SEEK_SET);

                while (!feof(imageSourceFile)) {
                    singleCharacter = (char) fgetc(imageSourceFile);
                    fputc(singleCharacter, imageOutputFile);
                }

                fclose(imageSourceFile);
                // Move to the last byte of the output file.
                fseek(imageOutputFile,0,SEEK_END);
                // Assign the actual file size of the output file to a variable.
                imageFileSize = ftell(imageOutputFile);
                // Move to the byte 54 (End of the header).
                fseek(imageOutputFile, 54, SEEK_SET);
                // Create a new variable of the type 'unsigned char' of the size of the image data.
                unsigned char imagedata2[imageFileSize - 53];
                if(imageOutputFile != NULL) {
                    // Copy all the bytes of the variable imagedata to the target file.
                    for (int i = 0; i < imageFileSize; i++) {
                        imagedata2[i]= (unsigned char) fgetc(imageOutputFile);
                    }
                    // Move to the beginning of the actual image data of the output file (Byte 54).
                    fseek(imageOutputFile, 54, SEEK_SET);
                    // Average the values of every byte, divide by 765 to get a decimal number between 0 and 1.
                    do {
                        // Assign this new value to a floating point variable named averagePixel.
                        float averagePixel = (((float) imagedata2[pixelRedIndex] + (float) imagedata2[pixelBlueIndex] + (float) imagedata2[pixelGreenIndex]) / (255*3));
                        if (averagePixel > thresholdValueRequired) {
                            imagedata2[pixelRedIndex] = (unsigned char) pixelColourWhite;
                            imagedata2[pixelBlueIndex] = (unsigned char) pixelColourWhite;
                            imagedata2[pixelGreenIndex] = (unsigned char) pixelColourWhite;
                            // Put the 3 bytes (RGB Colours) inside the target file on each iteration.
                            fputc(imagedata2[pixelRedIndex], imageOutputFile);
                            fputc(imagedata2[pixelBlueIndex], imageOutputFile);
                            fputc(imagedata2[pixelGreenIndex], imageOutputFile);
                        } else {
                            imagedata2[pixelRedIndex] = (unsigned char) pixelColourBlack;
                            imagedata2[pixelBlueIndex] = (unsigned char) pixelColourBlack;
                            imagedata2[pixelGreenIndex] = (unsigned char) pixelColourBlack;
                            // Put the 3 bytes (RGB Colours) inside the image output file on each iteration.
                            fputc(imagedata2[pixelRedIndex], imageOutputFile);
                            fputc(imagedata2[pixelBlueIndex], imageOutputFile);
                            fputc(imagedata2[pixelGreenIndex], imageOutputFile);
                        }
                        pixelRedIndex = pixelRedIndex + 3;
                        pixelBlueIndex = pixelBlueIndex + 3;
                        pixelGreenIndex = pixelGreenIndex + 3;

                    } while (pixelGreenIndex != imageFileSize-53);
                }
            }
        } else if (argc == 4) {
            FILE *imageSourceFile = fopen(arcv[3], "r");
            float thresholdValueRequired = (float) atof(arcv[2]);
            if (imageSourceFile != NULL) {
                printImageWidthAndHeight(imageSourceFile);

                // Create a file named "out.bmp" in writeable mode.
                FILE *imageOutputFile = fopen("out.bmp", "w+");
                // Move to the beginning of the file.
                fseek(imageSourceFile,0,SEEK_SET);

                while ( !feof(imageSourceFile) ) {
                    singleCharacter = (char) fgetc(imageSourceFile);
                    fputc(singleCharacter, imageOutputFile);
                }
                fclose(imageSourceFile);
                // Move to the last byte of the output file.
                fseek(imageOutputFile, 0, SEEK_END);
                // Assign the actual file size of the output file to a variable.
                imageFileSize = ftell(imageOutputFile);
                // Move to the byte 54 (End of the header).
                fseek(imageOutputFile, 54, SEEK_SET);
                // Create a new variable of the type 'unsigned char' of the size of the image data.
                unsigned char imagedata2[imageFileSize - 53];
                if(imageOutputFile != NULL) {
                    // Copy all the bytes of the variable imagedata to the target file.
                    for (int i = 0; i < imageFileSize; i++){
                        imagedata2[i]= (unsigned char) fgetc(imageOutputFile);
                    }
                    fseek(imageOutputFile, 54, SEEK_SET);
                    do{
                        // Average the values of every byte: divide by 765 (255 * 3, Blue, Green and Blue) to get a decimal number between 0 and 1.
                        float averagePixel = (((float)imagedata2[pixelRedIndex]+(float)
                                imagedata2[pixelBlueIndex]+(float)imagedata2[pixelGreenIndex])/(765));
                        if (averagePixel > thresholdValueRequired){
                            // Put 3 bytes (RGB Colours) inside the target file on each iteration.
                            imagedata2[pixelRedIndex] = pixelColourWhite;
                            imagedata2[pixelBlueIndex] = pixelColourWhite;
                            imagedata2[pixelGreenIndex] = pixelColourWhite;
                            fputc(imagedata2[pixelRedIndex], imageOutputFile);
                            fputc(imagedata2[pixelBlueIndex], imageOutputFile);
                            fputc(imagedata2[pixelGreenIndex], imageOutputFile);
                        }
                        else{
                            imagedata2[pixelRedIndex] = pixelColourBlack;
                            imagedata2[pixelBlueIndex] = pixelColourBlack;
                            imagedata2[pixelGreenIndex] = pixelColourBlack;
                            fputc(imagedata2[pixelRedIndex], imageOutputFile);
                            fputc(imagedata2[pixelBlueIndex], imageOutputFile);
                            fputc(imagedata2[pixelGreenIndex], imageOutputFile);
                        }
                        pixelRedIndex = pixelRedIndex + 3;
                        pixelBlueIndex = pixelBlueIndex + 3;
                        pixelGreenIndex = pixelGreenIndex + 3;
                    } while (pixelGreenIndex != imageFileSize-53);
                }
            }
        }
    } else if (strncmp(arcv[1], negativeFilterCommandString, 4) == 0) {
        FILE *imageSourceFile = fopen(arcv[2], "r");
        if(imageSourceFile != NULL){
            printImageWidthAndHeight(imageSourceFile);
            FILE *target_file = fopen("out.bmp", "w+");
            fseek(imageSourceFile,0,SEEK_SET);
            while ( !feof(imageSourceFile) ) {
                singleCharacter = (char) fgetc(imageSourceFile);
                fputc(singleCharacter, target_file);
            }
            fclose(imageSourceFile);
            fseek(target_file,0,SEEK_END);
            imageFileSize = ftell(target_file);
            fseek(target_file, 54, SEEK_SET);
            unsigned char imagedata2[imageFileSize];
            if(target_file != NULL) {
                for (int i=0; i<imageFileSize; i++) {
                    imagedata2[i] = (unsigned char) fgetc(target_file);
                }
                fseek(target_file, 54, SEEK_SET);
                do {
                    imagedata2[pixelRedIndex] = (unsigned char) (255 - imagedata2[pixelRedIndex]);
                    imagedata2[pixelBlueIndex] = (unsigned char) (255 - imagedata2[pixelBlueIndex]);
                    imagedata2[pixelGreenIndex] = (unsigned char) (255 - imagedata2[pixelGreenIndex]);
                    fputc(imagedata2[pixelRedIndex], target_file);
                    fputc(imagedata2[pixelBlueIndex], target_file);
                    fputc(imagedata2[pixelGreenIndex], target_file);
                    pixelRedIndex=pixelRedIndex+3;
                    pixelBlueIndex=pixelBlueIndex+3;
                    pixelGreenIndex=pixelGreenIndex+3;
                } while (pixelGreenIndex != imageFileSize-53);
            }
        }
    } else if (strncmp(arcv[1],grayScaleFilterCommandString,4)==0) {
        FILE *imageSourceFile = fopen(arcv[2], "r");
        if(imageSourceFile != NULL) {
            printImageWidthAndHeight(imageSourceFile);
            FILE *target_file = fopen("out.bmp", "w+");
            fseek(imageSourceFile,0,SEEK_SET);
            while ( !feof(imageSourceFile) ) {
                singleCharacter = (char) fgetc(imageSourceFile);
                fputc(singleCharacter, target_file);
            }
            fclose(imageSourceFile);
            fseek(target_file,0,SEEK_END);
            imageFileSize = ftell(target_file);
            fseek(target_file, 54, SEEK_SET);
            unsigned char imagedata2[imageFileSize];
            if(target_file != NULL) {
                for (int i=0; i<imageFileSize; i++) {
                    imagedata2[i] = (unsigned char) fgetc(target_file);
                }
                fseek(target_file, 54, SEEK_SET);
                do{ float averagePixel = ((float)imagedata2[pixelRedIndex]+(float)
                            imagedata2[pixelBlueIndex]+(float)imagedata2[pixelGreenIndex])/3;
                    imagedata2[pixelRedIndex] = (unsigned char) averagePixel;
                    imagedata2[pixelBlueIndex] = (unsigned char) averagePixel;
                    imagedata2[pixelGreenIndex] = (unsigned char) averagePixel;
                    fputc(imagedata2[pixelRedIndex], target_file);
                    fputc(imagedata2[pixelBlueIndex], target_file);
                    fputc(imagedata2[pixelGreenIndex], target_file);
                    pixelRedIndex=pixelRedIndex+3;
                    pixelBlueIndex=pixelBlueIndex+3;
                    pixelGreenIndex=pixelGreenIndex+3;
                } while (pixelGreenIndex != imageFileSize-53);
            }
        }
    }
}

// This function prints the width and height of any bmp image file.
void printImageWidthAndHeight(FILE *imageFile) {

    int fileHeaderIndex = 0;
    int imageWidth;
    int imageHeight;

    do {
        byteValue = fgetc(imageFile);
        imageHeaderDataArray[fileHeaderIndex] = (unsigned char) byteValue;
        fileHeaderIndex++;
        if (fileHeaderIndex == 54)
            break;
    } while (!feof(imageFile)) ;

    imageWidth = imageHeaderDataArray[18] + imageHeaderDataArray[19] * 256;
    imageHeight = imageHeaderDataArray[22] + imageHeaderDataArray[23] * 256;
    printf("Image width: %d \n", imageWidth);
    printf("Image height: %d \n", imageHeight);

}
