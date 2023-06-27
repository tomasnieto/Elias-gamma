#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
//asumimos que estamos trabajando con binarios de 16 bits
#define MAX_SIZE 6

int main() {
    uint16_t list[MAX_SIZE];
    list[0] = 0b0000100010000010;
    list[1] = 0b0010000100100100;
    list[2] = 0b0001000000000000;
    list[3] = 0b1000000001000001;
    list[4] = 0b0000000010010000;
    list[5] = 0b0011000000000000;

    int i = 0;
    int leading = 0; //cantidad de 0's antes del primer 1
    uint16_t temp = 0b0000000000000000;
    uint16_t temp2 = 0b0000000000000000;
    int d = 0; //cuantos shifts a la derecha se deben aplicar a temp
    int decoded_int = 0;
    int compensacion = 0;
    int leading2 = 0;

    while (1){
        //caso donde la compensacion indica un salto del int entero, 
        //osea que debemos leer el siguiente numero en la lista
        //sin considerar lo anterios
        if (compensacion == 16)
        {
            
            i = i + 1;
            compensacion = 0;
            if (i >= MAX_SIZE)
            {
                printf("stop 3\n");
                return 1;
            }
            continue;
        }
        
        temp = list[i];
        temp = temp << compensacion;
        leading = __builtin_clz(temp) - 16;


        //caso donde el leading esta repartido en dos elementos de la lista
        //TODO: >= o >?
        if (leading + compensacion > 16)
        {
            //printf("leading: %d\n", leading);
            //printf("C\n");
            if (i+1 >= MAX_SIZE)
            {
                printf("stop 4\n");
                return 1;
            }
            temp2 = list[i+1];
            leading2 = __builtin_clz(temp2) - 16;
            temp2 = temp2 << (leading2 + 1);
            temp2 = temp2 >> (16 - (leading2 + leading - compensacion + 1));
            decoded_int = pow(2, (leading2 + leading - compensacion + 1)) + (int)temp2;
            printf("Numero decodificado: %u\n", decoded_int);
            i = i + 1;
            compensacion = leading2 + 1 + leading2 + leading - compensacion + 1;
            continue;
        }
        

        //caso donde el leading esta en un solo elemento de la lista pero el resto esta repartido
        if (compensacion + 1 + 2*leading > 16) //necesitamos los primeros 2*leading + 1 - (16 - compensacion) digitos del siguiente valor
        {
            //printf("B\n");
            temp2 = list[i+1]; 
            temp2 = temp2 >> (16 - (2*leading + 1 - (16 - compensacion)));
            temp = temp << (leading + 1);
            temp = temp >> (leading + 1 + compensacion);
            temp = temp << leading;
            //printf("leading: %u\n", leading);
            //printf("temp: %u\n", temp);
            //printf("temp2: %u\n", temp2);
            decoded_int = pow(2, leading) + (int)(temp + temp2);
            printf("Numero decodificado: %u\n", decoded_int);
            i = i + 1;
            if (i >= MAX_SIZE)
            {
                printf("stop 1\n");
                return 1;
            }
            
            compensacion = (2*leading + 1 - (16 - compensacion));
            continue;

        }
        
        //printf("A\n");
        //Caso standard donde todo el numero esta en el mismo elemento de la lista
        
        d = 15 - (2 * leading);
        temp = temp << (leading + 1);
        temp = temp >> (leading + 1 + d);
        decoded_int = pow(2, leading) + (int)temp;
        printf("Numero decodificado: %u\n", decoded_int);
        compensacion = compensacion + 2*(leading) + 1;
    }
    return 0;
}
