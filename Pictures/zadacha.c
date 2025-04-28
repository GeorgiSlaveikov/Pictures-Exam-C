#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Picture
{
    int id;
    char artistName[30];
    char pictureName[30];
    float price;
} Picture;

int inputNumber(int lower, int upper, char expression[])
{
    int n;
    printf(expression);
    scanf("%d", &n);

    while (n < lower || n > upper)
    {
        printf("Incorrect input!\n");
        printf(expression);
        scanf("%d", &n);
    }
    return n;
}

Picture* callocArr(int n)
{
    Picture* pictures = (Picture*)calloc(n, sizeof(Picture));
    if (pictures == NULL)
    {
        printf("Couldn't alocate memory!\n");
        exit(1);
    }
    return pictures;
}

Picture createPicture(int number)
{
    printf("=====%d=====\n", number);

    int id;
    char artistName[30], pictureName[30];
    float price;

    id = inputNumber(1000, 10000, "Id: (4 digit integer) ");
    fflush(stdin);
    printf("Artist Name: ");
    fgets(artistName, sizeof(artistName), stdin);
    artistName[strcspn(artistName, "\n")] = '\0';
    fflush(stdin);
    printf("Picture Name: ");
    fgets(pictureName, sizeof(pictureName), stdin);
    pictureName[strcspn(pictureName, "\n")] = '\0';
    printf("Price: ");
    scanf("%f", &price);

    Picture picture;
    picture.id = id;
    picture.price = price;
    strcpy(picture.artistName, artistName);
    strcpy(picture.pictureName, pictureName);
    return picture;
}

Picture* fillArr(Picture* pictures, int n)
{
    for (int i = 0; i < n; i++)
    {
        pictures[i] = createPicture(i+1);
    }
    return pictures;
}

float avgPrice(Picture* pictures, int n, float price)
{
    float priceSum = 0;
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (pictures[i].price > price)
        {
            priceSum += pictures[i].price;
            count += 1;
        }
    }
    if (count > 0)
    {
        float result = priceSum / count;
        return result;
    }
    return 0;
}

int saveToTextFile(Picture* pictures, int n, char letter)
{
    char path[] = "d:\\C++CProgramming\\Exam2\\info.txt";
    FILE* filePtr = fopen(path, "w");

    if (filePtr == NULL)
    {
        printf("Wrong path!");
        exit(1);
    }

    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (pictures[i].artistName[0] == letter)
        {
            fprintf(filePtr, "%d; %s; %0.2fleva\n", pictures[i].id, pictures[i].pictureName, pictures[i].price);
            count += 1;
        }
    }
    fclose(filePtr);
    return count;
}

void printPicture(Picture picture)
{
    printf("Picture name: %s\n", picture.pictureName);
    printf("Price: %0.2f\n", picture.price);
    printf("=================\n");
}

int numberOfLines(char path[])
{
    FILE* filePtr = fopen(path, "rb");

    if (filePtr == NULL)
    {
        printf("Wrong path!");
        exit(1);
    }

    int count = 0;
    while (!feof(filePtr))
    {
        count += 1;
        fgetc(filePtr);
    }
    fclose(filePtr);
    return count;
}

void writeBinaryFile(Picture* pictures, int n)
{
    char path[] = "d:\\C++CProgramming\\Exam2\\picture.bin";
    FILE* filePtr = fopen(path, "wb");

    if (filePtr == NULL)
    {
        printf("Wrong path!");
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        int artistNameLen = strlen(pictures[i].artistName);
        int pictureNameLen = strlen(pictures[i].pictureName);
        
        fwrite(&pictures[i].id, sizeof(int), 1, filePtr);
        fwrite(&artistNameLen, sizeof(int), 1, filePtr);
        fwrite(pictures[i].artistName, sizeof(char), artistNameLen, filePtr);
        fwrite(&pictureNameLen, sizeof(int), 1, filePtr);
        fwrite(pictures[i].pictureName, sizeof(char), pictureNameLen, filePtr);
        fwrite(&pictures[i].price, sizeof(float), 1, filePtr);
        fputc('\n', filePtr);
    }
    fclose(filePtr);
}

void readBinaryFile(char artistNameToSearch[])
{
    char path[] = "d:\\C++CProgramming\\Exam2\\picture.bin";
    FILE* filePtr = fopen(path, "rb");

    if (filePtr == NULL)
    {
        printf("Wrong path!");
        exit(1);
    }

    int i = 0;
    int flag = 0;
    while (!feof(filePtr))
    {
        int id;
        int artistNameLen;
        int pictureNameLen;
        char* artistName;
        char* pictureName;
        float price;

        fread(&id, sizeof(int), 1, filePtr);
      
        fread(&artistNameLen, sizeof(int), 1, filePtr);
        
        artistName = (char*)malloc(artistNameLen + 1);
        fread(artistName, sizeof(char), artistNameLen, filePtr);
        artistName[artistNameLen] = '\0';

        fread(&pictureNameLen, sizeof(int), 1, filePtr);
       
        pictureName = (char*)malloc(pictureNameLen + 1);
        fread(pictureName, sizeof(char), pictureNameLen, filePtr);
        pictureName[pictureNameLen] = '\0';

        fread(&price, sizeof(float), 1, filePtr);

        if (strcmp(artistName, artistNameToSearch) == 0)
        {
            Picture pic;
            pic.id = id;
            pic.price = price;
            strcpy(pic.artistName, artistName);
            strcpy(pic.pictureName, pictureName);

            printPicture(pic);
            flag = 1;
        }

        fgetc(filePtr);
        free(artistName);
        free(pictureName);
        i+=1;
    }

    if (flag == 0)
    {
        printf("Person not found!\n");
    }
    fclose(filePtr);
}

void printArr(Picture* pictures, int n)
{
    printf("\n=====All Pictures=====\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d; %s; %s; %.2f;\n", pictures[i].id, pictures[i].artistName, pictures[i].pictureName, pictures[i].price);
    }
    printf("\n=======================\n");
}



int main()
{
    int n = inputNumber(3, 30, "Enter number of pictures (integer between 3 and 30): ");
    Picture* pictures = callocArr(n);
    pictures = fillArr(pictures, n);

    printArr(pictures, n);
    fflush(stdin);

    float price;
    printf("Enter a price to compare: ");
    scanf("%f", &price);
    printf("Avg: %0.2f\n", avgPrice(pictures, n, price));

    fflush(stdin);
    char firstLetter;
    printf("Enter first letter of a name: ");
    scanf("%c", &firstLetter);
    printf("Saved info in file: %d people;\n", saveToTextFile(pictures, n, firstLetter));

    writeBinaryFile(pictures, n);

    printf("Enter a name of artist to search: ");
    char name[30];
    fflush(stdin);
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    readBinaryFile(name);
    return 0;
}