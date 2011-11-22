
#include "ncutils.h"

#include <stdio.h>
#include <iostream>
#include <bitset>

int main() {

    unsigned char payload[10] = {0xa0, 0xb1, 0xc2,0x53,0x4,0x5,0x6, 0x7, 0x8, 0xff};


    printf("\n\n Hex representation \n <  ");
    for ( int i=0 ; i<10; i++){
    	if ( i == 9 ){
        	printf(" %x >", payload[i]);
    	} else {
        	printf(" %x, ", payload[i]);
    	}

    }
    printf("\n\n");


    printf(" Bit representation \n <  ");
    for ( int i=0 ; i<10; i++){
    	if ( i == 9 ){
    		std::cout << std::bitset<8>(payload[i])<< " >" << std::endl;
    	} else {
    		std::cout << std::bitset<8>(payload[i])<< ", ";
    	}

    }
    printf("\n\n");

    printf("payload 10 bytes - ff Q=16, m=4 -> vector with 20 elements \n\n");

    FiniteField* field = new FiniteField(2,4);

    FiniteFieldVector* vector = field->byteToVector(payload, 10);

    printf("creating a finitefield vector with %u elements \n", vector->getLength());


    printf(" vector elements: \n <  ");
    for ( int i=0 ; i<vector->getLength(); i++){
    	if ( i == vector->getLength()-1 ){
    		std::cout << std::bitset<4>(vector->getCoordinate(i))<< " >" << std::endl;
    	} else {
    		std::cout << std::bitset<4>(vector->getCoordinate(i))<< ", ";
    	}

    }
    printf("\n\n");

	return 0;
}
