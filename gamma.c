#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
//asumimos que estamos trabajando con binarios de 32 bits


int main(int argc, char* argv[]) {
    if (argc < 2) {printf("Debe proporcionar la secuencia de bits como argumento.\n");return 1;}

    #define MAX_SIZE argc - 1
    //uint32_t list[argc-1];
    /*list[0] = 0b00000110001000000001010001010001;
    list[1] = 0b11000000000111011111000110000000;
    list[2] = 0b00010001101000001010100000111010;
    list[3] = 0b00000101001000001101110000001100;
    list[4] = 0b01001100001010000000110110000000;
    list[5] = 0b10000000000000010010010100101000;
    list[6] = 0b00100100000001010100000011100000;
    list[7] = 0b00000101110010000000011111001000;
    list[8] = 0b00011011000000010011000000010011;
    list[9] = 0b10000001001111000010110000001000;
    list[10] = 0b10001010000000100111010000000000;*/

    uint32_t* list = (uint32_t*)malloc((argc - 1) * sizeof(uint32_t));
    if (list == NULL) {
        printf("Error: No se pudo asignar memoria para la lista.\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        const char* binarySequence = argv[i];
        uint32_t value = 0;

        for (int j = 0; binarySequence[j] != '\0'; j++) {
            value <<= 1;  // Left-shift the value by 1 to make room for the next bit

            if (binarySequence[j] == '1') {
                value |= 1;  // Set the least significant bit to 1
            }
        }

        list[i - 1] = value;
    }

    int i = 0;
    int leading = 0; //cantidad de 0's antes del primer 1
    uint32_t temp = 0b00000000000000000000000000000000;
    uint32_t temp2 = 0b00000000000000000000000000000000;
    int d = 0; //cuantos shifts a la derecha se deben aplicar a temp
    int decoded_int = 0;
    int compensacion = 0;
    int leading2 = 0;

    while (1){
        //caso donde la compensacion indica un salto del int entero, 
        //osea que debemos leer el siguiente numero en la lista
        //sin considerar lo anterior
        if (compensacion == 32)
        {
            //printf("reset\n");
            i = i + 1;
            compensacion = 0;
            if (i >= MAX_SIZE)
            {
                //printf("stop 3\n");
                return 1;
            }
            continue;
        }

        temp = list[i];
        temp = temp << compensacion;
        if (temp == 0b00000000000000000000000000000000)
        {
            leading = 32 - compensacion;
        }
        else
        {
            leading = __builtin_clz(temp) ;//- 16;
        }

        //caso donde el leading esta repartido en dos elementos de la lista
        if (leading + compensacion >= 32)
        {
            //printf("leading: %d\n", leading);
            //printf("C\n");
            if (i+1 >= MAX_SIZE)
            {
                //printf("stop 4\n");
                return 1;
            }
            temp2 = list[i+1];
            leading2 = __builtin_clz(temp2); // - 16;
            temp2 = temp2 << (leading2 + 1);
            temp2 = temp2 >> (32 - (leading2 + leading));
            decoded_int = pow(2, (leading2 + leading)) + (int)temp2;
            //printf("i: %d\n", i);
            //printf("leading: %u\n", leading);
            //printf("leading2: %u\n", leading2);
            //printf("temp: %u\n", temp);
            //printf("temp2: %u\n", temp2);
            //printf("compensacion: %u\n", compensacion);
            printf("Numero decodificado: %u\n", decoded_int);
            i = i + 1;
            compensacion = leading2 + 1 + leading2 + leading ;//- compensacion + 1;
            continue;
        }

        //caso donde el leading esta en un solo elemento de la lista pero el resto esta repartido
        if (compensacion + 1 + 2*leading > 32) //necesitamos los primeros 2*leading + 1 - (16 - compensacion) digitos del siguiente valor
        {
            //printf("B\n");
            temp2 = list[i+1]; 
            temp2 = temp2 >> (32 - (2*leading + 1 - (32 - compensacion)));
            temp = temp << (leading + 1);
            temp = temp >> (leading + 1 + compensacion);
            temp = temp << (leading - (32 - (leading + 1 + compensacion)));
            //printf("i: %d\n", i);
            //printf("leading: %u\n", leading);
            //printf("temp: %u\n", temp);
            //printf("temp2: %u\n", temp2);
            //printf("compensacion: %u\n", compensacion);
            decoded_int = pow(2, leading) + (int)(temp + temp2);
            printf("Numero decodificado: %u\n", decoded_int);
            i = i + 1;
            if (i >= MAX_SIZE)
            {
                //printf("stop 1\n");
                return 1;
            }
            compensacion = (2*leading + 1 - (32 - compensacion));
            continue;
        }

        //printf("A\n");
        //Caso standard donde todo el numero esta en el mismo elemento de la lista
        //printf("i: %d\n", i);
        //printf("leading: %u\n", leading);
        //printf("temp: %u\n", temp);
        //printf("compensacion: %u\n", compensacion);
        temp = temp << (leading + 1);
        temp = temp >> (leading + 32 - (2 * leading));
        decoded_int = pow(2, leading) + (int)temp;
        printf("Numero decodificado: %u\n", decoded_int);
        compensacion = compensacion + 2*(leading) + 1;
    }

    free(list);
    return 0;
}
