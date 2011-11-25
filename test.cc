
#include "ncutils.h"

#include <stdio.h>
#include <iostream>
#include <bitset>
#include <vector>
#include <algorithm>
#include <string>

void test_vectors();
void block_level_example();


int main() {

	//test_vectors();
	block_level_example();


	return 0;
}

void block_level_example(){

	/**
	 *
	 * This example shows how to use ncutils to build simulate
	 * random network coding network. Uncoded packets are first
	 * created then they are transformed to the corresponding coded
	 * packets. These are linearly combined (as a network would) in
	 * new coded packets. The resulting packets are finally decoded
	 * with the packet decoder.
	 *
	 * @author lokeller
	 */


	FiniteField* ff = FiniteField::getDefaultFiniteField();

	int blockNumber = 10;
	int payloadLen = 10;
	int payloadLenCoeffs = 20;


	/* create the uncoded packets */
	//UncodedPacket[] inputPackets = new UncodedPacket[blockNumber];
	std::vector<UncodedPacket> inputPackets;

	for ( int i = 0 ; i < blockNumber ; i++) {
		unsigned char* payload = new unsigned char[payloadLen];
		std::fill(payload, payload+payloadLen, 0xA0+i);
		//Arrays.fill(payload, (byte) (0XA0 +  i));
		inputPackets.push_back(UncodedPacket(i, payload, payloadLen));
		std::cout<< "Uncodedpacket: " << i << ": "<< inputPackets[i].toString();
	}

//
//	        System.out.println(" Input blocks: ");
//	        printUncodedPackets(Arrays.asList(inputPackets), payloadLen);
//
//	        /* prepare the input packets to be sent on the network */
//	        CodedPacket[] codewords = new CodedPacket[blockNumber];
//
//	        for ( int i = 0 ; i < blockNumber ; i++) {
//	            codewords[i] = new CodedPacket( inputPackets[i], blockNumber, ff);
//	        }
//
//	        System.out.println(" Codewords: ");
//	        printCodedPackets(Arrays.asList(codewords), payloadLenCoeffs);
//
//	        /* create a set of linear combinations that simulate
//	         * the output of the network
//	         */
//
//	        CodedPacket[] networkOutput = new CodedPacket[blockNumber];
//
//	        Random r = new Random(2131231);
//
//	        for ( int i = 0 ; i < blockNumber ; i++) {
//
//	            networkOutput[i] = new CodedPacket(blockNumber, payloadLen, ff);
//
//	            for ( int j = 0 ; j < blockNumber ; j++) {
//	                int x = r.nextInt(ff.getCardinality());
//	                CodedPacket copy = codewords[j].scalarMultiply(x);
//	                networkOutput[i] = networkOutput[i].add(copy);
//
//	            }
//	        }
//
//	        System.out.println(" Network output: ");
//	        printCodedPackets(Arrays.asList(networkOutput), payloadLenCoeffs);
//
//	        /* decode the received packets */
//	        PacketDecoder decoder = new PacketDecoder(ff, blockNumber, payloadLen);
//
//	        System.out.println(" Decoded packets: ");
//	        for ( int i = 0; i < blockNumber ; i++) {
//	            Vector<UncodedPacket> packets = decoder.addPacket(networkOutput[i]);
//	            printUncodedPackets(packets, payloadLen);
//	        }
//
//	    }
//
//	    private static void printUncodedPackets(Iterable<UncodedPacket> packets, int payloadLen) {
//	        for (UncodedPacket p : packets) {
//	            System.out.println(p);
//	        }
//	    }
//
//	    private static void printCodedPackets(Iterable<CodedPacket> packets, int payloadLen) {
//	        for (CodedPacket p : packets) {
//	            System.out.println(p);
//	        }
//	    }
//
//	}


}


void test_vectors(){

    unsigned char payload[10] = {0xa0, 0xb1, 0xc2,0x53,0x4,0x5,0x6, 0x7, 0x8, 0xac};


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
    printf(" vector to string :\n" );

	std::cout << vector->toString()<< std::endl;

    printf("\n\n");


    printf("getting back the payload from the vector \n");

    unsigned char* back = field->vectorToBytes(vector) ;


    printf("\n\n Hex representation \n <  ");
    for ( int i=0 ; i<10; i++){
    	if ( i == 9 ){
        	printf(" %x >", back[i]);
    	} else {
        	printf(" %x, ", back[i]);
    	}

    }
    printf("\n\n");


    printf(" Bit representation \n <  ");
    for ( int i=0 ; i<10; i++){
    	if ( i == 9 ){
    		std::cout << std::bitset<8>(back[i])<< " >" << std::endl;
    	} else {
    		std::cout << std::bitset<8>(back[i])<< ", ";
    	}

    }
}


