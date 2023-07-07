#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void writeToFile(const char* content) {
    // printf("%s\n", content);
    FILE* file = fopen("out.txt", "a");

    if (file != NULL) {
        fputs(content, file);
        fputs("\n", file);

        fclose(file);
    } else {
        printf("Unable to open the file.\n");
    }
}

char** processList(uint32_t* list, int size, int NumeroElementos) {
    #define STRING_LENGTH 20

    char** out = (char**)malloc(NumeroElementos * sizeof(char*));
    for (int i = 0; i < NumeroElementos; i++) {
        out[i] = (char*)malloc((STRING_LENGTH + 1) * sizeof(char));
    }

    int i = 0;
    int l = 0;
    int leading = 0; // cantidad de 0's antes del primer 1
    uint32_t temp = 0b00000000000000000000000000000000;
    uint32_t temp2 = 0b00000000000000000000000000000000;
    int d = 0; // cuantos shifts a la derecha se deben aplicar a temp
    int decoded_int = 0;
    int compensacion = 0;
    int leading2 = 0;
    char content[20];

    while (1) {
        // caso donde la compensacion indica un salto del int entero,
        // osea que debemos leer el siguiente numero en la lista
        // sin considerar lo anterior
        if (compensacion == 32) {
            i = i + 1;
            compensacion = 0;
            if (i >= size) {
                break;
            }
            continue;
        }

        temp = list[i];
        temp = temp << compensacion;
        if (temp == 0b00000000000000000000000000000000) {
            leading = 32 - compensacion;
        } else {
            leading = __builtin_clz(temp);
        }

        // caso donde el leading esta repartido en dos elementos de la lista
        if (leading + compensacion >= 32) {
            if (i + 1 >= size) {
                break;
            }
            temp2 = list[i + 1];
            leading2 = __builtin_clz(temp2);
            temp2 = temp2 << (leading2 + 1);
            temp2 = temp2 >> (32 - (leading2 + leading));
            decoded_int = pow(2, (leading2 + leading)) + (int)temp2;
            i = i + 1;
            compensacion = leading2 + 1 + leading2 + leading;
        }

        // caso donde el leading esta en un solo elemento de la lista pero el resto esta repartido
        else if (compensacion + 1 + 2 * leading > 32) {
            i = i + 1;
            if (i >= size) {
                break;
            }
            temp2 = list[i];
            temp2 = temp2 >> (32 - (2 * leading + 1 - (32 - compensacion)));
            temp = temp << (leading + 1);
            temp = temp >> (leading + 1 + compensacion);
            temp = temp << (leading - (32 - (leading + 1 + compensacion)));
            decoded_int = pow(2, leading) + (int)(temp + temp2);
            compensacion = (2 * leading + 1 - (32 - compensacion));
        } else {
            // Caso standard donde todo el numero esta en el mismo elemento de la lista
            if (leading == 0) {
                decoded_int = 1;
            } else {
                temp = temp << (leading + 1);
                temp = temp >> (32 - leading);
                decoded_int = pow(2, leading) + (int)temp;
            }
            compensacion = compensacion + 2 * (leading) + 1;
        }

        sprintf(content, "%d", decoded_int);
        strcpy(out[l], content);
        l += 1;
    }

    return out;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Debe proporcionar la secuencia de bits como argumento.\n");
        return 1;
    }
    clock_t start_time, end_time;
    double execution_time;
    start_time = clock();

    FILE* file = fopen("out.txt", "w");
    fclose(file);

    uint32_t* list = (uint32_t*)malloc((argc - 2) * sizeof(uint32_t));
    const int numeroDeElementos = atoi(argv[1]);

    if (list == NULL) {
        printf("Error: No se pudo asignar memoria para la lista.\n");
        return 1;
    }

    for (int i = 1; i + 1 < argc; i++) {
        const char* binarySequence = argv[i + 1];
        uint32_t value = 0;

        for (int j = 0; binarySequence[j] != '\0'; j++) {
            value <<= 1;

            if (binarySequence[j] == '1') {
                value |= 1;
            }
        }

        list[i - 1] = value;
    }

    char** out = processList(list, argc - 2, numeroDeElementos);
    
    end_time = clock();
    for (int k = 0; k < numeroDeElementos; k++) {
        writeToFile(out[k]);
        free(out[k]);
    }
    free(out);

    
    execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution Time: %.4f seconds\n", execution_time);

    free(list);
    return 0;
}
