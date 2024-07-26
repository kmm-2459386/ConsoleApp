#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// �֐��v���g�^�C�v
int MinOfArray(int array[], int arraySize);
int randRange(int min, int max);

int main()
{
    int number;
    srand((unsigned int)time(nullptr)); // srand() �ɂ� unsigned int ���K�v�ł�

    do {
        printf("�l��:");
        scanf_s("%d", &number);
    } while (number <= 0);

    int* height = (int*)calloc(number, sizeof(int));
    if (height == nullptr) {
        printf("calloc()���s");
        exit(1);
    }

    printf("%d�l�̐g������͂��܂�\n", number);
    for (int i = 0; i < number; i++) {
        height[i] = randRange(140, 190);
        printf("%d\n", height[i]);
    }

    int min = MinOfArray(height, number);
    printf("�ŏ��l�� %d �ł�\n", min);

    free(height);
    return 0;
}

int MinOfArray(int array[], int arraySize)
{
    if (arraySize <= 0) {
        printf("�z��̃T�C�Y��0�ȉ��ł�\n");
        return INT_MAX;
    }

    int min = array[0]; 

    for (int i = 1; i < arraySize; ++i) {
        if (array[i] < min) {
            min = array[i]; 
        }
    }

    return min; 
}

int randRange(int min, int max)
{
    int wid = max - min + 1;
    return min + rand() % wid;
}