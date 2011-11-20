
#include <cmath>
//#include "FiniteField.h"
#include "ncutils.h"

#define null 0


//
// Support functions for mathematical operations
//
int ipow(int base, int exp)
	{
		int result = 1;
		while (exp)
		{
			if (exp & 1)
				result *= base;
			exp >>= 1;
			base *= base;
		}

		return result;
}

/**
 * Constructs a new extension field
 *
 * @param m the power of the polynomial
 */
FiniteField::FiniteField(int q, int m) {

	// TODO: check m and q

	//if (q < 1 || m < 0) throw new RuntimeException("Invalid field size");
	//if (q != 2 || m > 16) throw new UnsupportedOperationException("Finite field not supported");

	Q = ipow(q,m);


	inverse = new int[Q];

	sum = new int*[Q];
	mul = new int*[Q];
	div = new int*[Q];
	sub = new int*[Q];

	for (int i = 0; i < Q; i++) {
		sum[i] = new int[Q];
		mul[i] = new int[Q];
		div[i] = new int[Q];
		sub[i] = new int[Q];
	}


	int primitive_polynomial [] = { 3, 7, 11, 19, 37, 67, 137,
									285, 529,1033,2053,4179,
									8219,17475, 32771, 69643 };

	int c = primitive_polynomial[m - 1] - ( 1 << m );


	for (int i = 0 ; i < Q ; i++) {
		for (int j = 0 ; j < Q ; j++) {

			/*
			 *  ^ = xor
			 *  In a finite field with characteristic 2,
			 *  addition modulo 2, subtraction modulo 2,
			 *  and XOR are identical.
			 *
			 */
			sum[i][j] = i ^ j;
			sub[i][j] = i ^ j;

			int a = i;
			int b = j;
			int p = 0;

			/* paesant's algorithm*/
			for ( int k = 0 ; k < m ; k++) {
				if ( (b & 0x1) == 1) {
					p = p ^ a;
				}

				bool r = (a & (0x1 << (m-1))) > 0;
				a = (a << 1) % Q;
				if (r) {
					a = a ^ c;
				}
				b = b >> 1;
			}

			mul[i][j] = p;

		}
	}

	for (int i = 0 ; i < Q ; i++) {
		for (int j = 0 ; j < Q ; j++) {

			div[mul[i][j]][i] = j;
			div[mul[i][j]][j] = i;

		}
	}

	for (int i = 1 ; i < Q ; i++) {
		inverse[i] = div[1][i];
	}

}

/**
 * Constructs a new finite field
 *
 * @param q the prime used to define the field
 */

FiniteField::FiniteField(int q) {

	Q = q;

	// TODO: check size
	//if (q < 1) throw new RuntimeException("Invalid field size");

	inverse = new int[Q];

	sum = new int*[Q];
	mul = new int*[Q];
	div = new int*[Q];
	sub = new int*[Q];

	for (int i = 0; i < Q; i++) {
		sum[i] = new int[Q];
		mul[i] = new int[Q];
		div[i] = new int[Q];
		sub[i] = new int[Q];
	}

	/* build inverse table */
	for (int b = 1 ; b < Q ; b++) {
			for (int i = 1 ; i < Q ; i++) {
					if ((i *  b) % Q == 1) {
							inverse[b] = i;
							break;
					}
			}
	}

	/* build tables */
	for (int b = 0 ; b < Q ; b++) {
			for (int i = 0 ; i < Q ; i++) {
				sum[b][i] = (b+i) % Q;
				sub[b][i] = (b-i+Q) % Q;
				mul[b][i] = (b * i) % Q;
				div[b][i] = (b * inverse[i]) % Q;
			}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////
///**
// * Convert a byte array to its finite field vector representation, this method
// * creates a vector with as many coordinates as they can be read from the byte
// * array
// *
// * @param bytes an array of bytes
// * @return the representation of the array as a vector
// */
//public FiniteFieldVector byteToVector(char *bytes) {
//	return byteToVector(bytes, coordinatesCount(bytes.length));
//}
//
//
///**
// * Convert a byte array to its finite field vector representation
// *
// * @param bytes an array of bytes
// * @param coordinates how many coordinates should be read
// * @return the representation of the array as a vector
// */
//public FiniteFieldVector byteToVector(byte [] bytes, int coordinates) {
//	return byteToVector(bytes, 0, bytes.length, coordinates);
//}
//
///**
// * Convert a byte array to its finite field vector representation
// *
// * @param bytes an array of bytes
// * @param offset the offset of the first byte to be converted
// * @param length the number of bytes that must be converted
// * @return the representation of the array as a vector
// */
//FiniteFieldVector byteToVector(byte bytes, int offset, int length, int coordinates) {
//
//	int [] data = new int[coordinates];
//
//	int bitsPerField = bitsPerCoordinate();
//
//	for ( int i = 0 ; i < coordinates; i++) {
//		data[i] = readBits(bytes, offset, i, bitsPerField);
//	}
//
//    return new FiniteFieldVector(data, this);
//
//}
//
//FiniteFieldVector byteToVector(byte  bytes, int offset, int length) {
//	return byteToVector(bytes, offset, length, coordinatesCount(bytes.length));
//}

/////////////////////////////////

//FiniteFieldVector byteToVector(char *byte, int byte_length) {
//
//	//return byteToVector(bytes, coordinatesCount(bytes.length));
//	//return byteToVector(bytes, 0, bytes.length, coordinates);
//
//	coordinates = coordinatesCount(bytes.length);
//
//	// data sarà il mio vettore all'interno di finitefieldvector
//	int data = new int[coordinates];
//
//	// bit interni a un elemento di coordinates
//	int bitsPerField = bitsPerCoordinate();
//
//	// per ogni elemento del vettore
//	for ( int i = 0 ; i < coordinates; i++) {
//		//inserisci in ogni campo m bit
//		data[i] = readBits(bytes, offset, i, bitsPerField);
//	}
//
//	//restutuisci il vettore creato
//    return new FiniteFieldVector(data, datalenght, this);
//
//}
//
//int readBits(byte* data, int offset, int field, int fieldSize) {
//
//	int start_bit, end_bit, i;
//	int ret;
//
//	start_bit = field * fieldSize;
//	end_bit = ( field + 1) * fieldSize;
//
//	ret = 0;
//
//	for (i = start_bit; i < end_bit; i++) {
//			ret = (ret << 1 ) | ( (data[i / 8+offset] >> ( i%8)) & 0x1);
//	}
//
//	return ret % Q;
//
//}

//////////////////////////////////////////////////////////////////////////////////////
//
//    /**
//     * Convert a vector to its byte array representation
//     *
//     * @param vector a vector over the specified finite field
//     * @return the byte array representation
//     */
//    public byte[] vectorToBytes(FiniteFieldVector vector) {
//    	byte[] output = new byte[bytesLength(vector.getLength())];
//    	vectorToBytes(vector, output, 0);
//    	return output;
//    }
//
//
//    private void writeBits(byte[] data, int offset, int field, int value, int fieldSize) {
//        int start_bit, end_bit, i;
//
//        value = value % Q;
//
//        start_bit = field * fieldSize;
//        end_bit = ( field + 1) * fieldSize;
//
//        for ( i = start_bit; i < end_bit; i++) {
//                byte mask, bit;
//
//                mask = (byte) ( ~( 1 << (i % 8)));
//                bit = (byte) (( ( value >> (fieldSize - 1) ) & 0x1) << ( i % 8));
//
//                data[i / 8+offset] = (byte) (( data[i / 8 +offset] & mask) | bit);
//                value = value << 1;
//        }
//
//    }
//

//
//    void vectorToBytes (FiniteFieldVector vector, byte [] output, int start) {
//
//        int[] coordinates = vector.coordinates;
//
//        int bitsPerField = bitsPerCoordinate();
//
//        for ( int i = 0 ; i < coordinates.length; i++) {
//        	writeBits(output, start, i, coordinates[i], bitsPerField);
//        }
//
//    }
//
//





/**
 * Returns the number of bytes that can be represented with a given number
 * of coordinates
 *
 * @param coordinatesCount  the number of
 * @return the number of bytes
 */
int FiniteField::bytesLength(int coordinates_count) {

	int bits_per_coordinate = bitsPerCoordinate();

	//return (int) Math.ceil(((double) (coordinates_count * bits_per_coordinate)) / 8.0);

	return int(ceil((coordinates_count * bits_per_coordinate)/ 8.0)  );
}



/**
 * Returns the number of bits necessary to represent an element of the field
 *
 * @return the number of bits necessary to store a field element
 */

int FiniteField::bitsPerCoordinate() {

	//int bitsPerCoordinate = (int) Math.ceil(Math.log(Q) / Math.log(2));

	int bits_per_coordinate = int(log(double(Q))/log(2.0));

	return bits_per_coordinate;
}



/**
 * Returns the number of coordinates that can be represented with a given
 * number of bytes
 *
 * @param bytesLength the number of bytes
 * @return the number of bytes
 */
int FiniteField::coordinatesCount(int bytes_length) {

	int bits_per_coordinate = bitsPerCoordinate();

	return bytes_length * 8 / bits_per_coordinate;

}



/**
 * Return the cardinality of the field
 * @return the number of elements in the field
 */
int FiniteField::getCardinality() {
	return Q;
}


//
//    @Override
//    public boolean equals(Object obj) {
//        if ( obj instanceof FiniteField) {
//            return ((FiniteField) obj).getCardinality() == Q;
//        } else {
//            return false;
//        }
//    }
//
//    @Override
//    public int hashCode() {
//        int hash = 5;
//        hash = 89 * hash + this.Q;
//        return hash;
//    }
//
//
//
//}
