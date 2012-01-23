/*
 * C++ Conversion of NCUTILS https://code.google.com/p/ncutils/
 * Copyright (c) 2011-2012 Matteo Bruni <matteo.bruni at gmail.com>
 *
 * Original License header:
 *
 * Original Java implementation Copyright (c) 2010, EPFL - ARNI
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the EPFL nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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

FiniteField* FiniteField::getDefaultFiniteField(){
	return new FiniteField(2,4);
}

/**
 * Constructs a new extension field
 *
 * @param m the power of the polynomial
 */
FiniteField::FiniteField(int q, int m) {

	// TODO: check m and q

    //if (q < 1 || m < 0) throw cRuntimeError("Array of size %d indexed by %d", msg_arraysize, k);
    //if (q != 2 || m > 16) throw cRuntimeError("Array of size %d indexed by %d", msg_arraysize, k);

	//if (q < 1 || m < 0) throw new RuntimeException("Invalid field size");
	//if (q != 2 || m > 16) throw new UnsupportedOperationException("Finite field not supported");

	Q = ipow(q,m);


	inverse = new int[Q];

	_sum = new int*[Q];
	_mul = new int*[Q];
	_div = new int*[Q];
	_sub = new int*[Q];

	for (int i = 0; i < Q; i++) {
		_sum[i] = new int[Q];
		_mul[i] = new int[Q];
		_div[i] = new int[Q];
		_sub[i] = new int[Q];
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
			 *  addition modulo 2, _subtraction modulo 2,
			 *  and XOR are identical.
			 *
			 */
			_sum[i][j] = i ^ j;
			_sub[i][j] = i ^ j;

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

			_mul[i][j] = p;

		}
	}

	for (int i = 0 ; i < Q ; i++) {
		for (int j = 0 ; j < Q ; j++) {

			_div[_mul[i][j]][i] = j;
			_div[_mul[i][j]][j] = i;

		}
	}

	for (int i = 1 ; i < Q ; i++) {
		inverse[i] = _div[1][i];
	}

}

/**
 * Constructs a new finite field
 *
 * @param q the prime used to define the field
 */

FiniteField::FiniteField(int total_size) {

	Q = total_size;

	// TODO: check size
	//if (q < 1) throw new RuntimeException("Invalid field size");

	inverse = new int[Q];

	_sum = new int*[Q];
	_mul = new int*[Q];
	_div = new int*[Q];
	_sub = new int*[Q];

	for (int i = 0; i < Q; i++) {
		_sum[i] = new int[Q];
		_mul[i] = new int[Q];
		_div[i] = new int[Q];
		_sub[i] = new int[Q];
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
				_sum[b][i] = (b+i) % Q;
				_sub[b][i] = (b-i+Q) % Q;
				_mul[b][i] = (b * i) % Q;
				_div[b][i] = (b * inverse[i]) % Q;
			}
	}

}

FiniteField::~FiniteField(){

	for(int i = 0; i < Q; ++i){
		delete [] _sum[i];
		delete [] _mul[i];
		delete [] _div[i];
		delete [] _sub[i];
	}

	delete [] inverse;
	delete [] _sum;
	delete [] _mul;
	delete [] _div;
	delete [] _sub;
}


int FiniteField::sum(int a, int b){
	return _sum[a][b];
}
int FiniteField::sub(int a, int b){
	return _sub[a][b];
}
int FiniteField::div(int a, int b){
	return _div[a][b];
}
int FiniteField::mul(int a, int b){
	return _mul[a][b];
}



/**
 * Convert a byte array to its finite field vector representation, this method
 * creates a vector with as many coordinates as they can be read from the byte
 * array
 *
 * @param bytes an array of bytes
 * @param bytes_lenght the number of bytes that must be converted
 * @param offset the offset of the first byte to be converted
 * @return the representation of the array as a vector
 */
FiniteFieldVector* FiniteField::byteToVector(unsigned char *bytes, int bytes_lenght, int offset) {

	unsigned int coordinates = coordinatesCount(bytes_lenght);
	//return byteToVector(bytes, bytes_lenght, coordinatesCount(bytes_lenght));

	int *data = new int[coordinates];

	int bits_per_field = bitsPerCoordinate();

	for (unsigned int i = 0 ; i < coordinates; i++) {
		data[i] = readBits(bytes, bytes_lenght, offset, i, bits_per_field);
	}

    return new FiniteFieldVector(data, coordinates, this);

}



int FiniteField::readBits(unsigned char* data, int data_lenght, int offset, int field, int fieldSize) {

	int start_bit, end_bit, i;
	int ret;

	start_bit = field * fieldSize;
	end_bit = ( field + 1) * fieldSize;

	ret = 0;

	for (i = start_bit; i < end_bit; i++) {
			ret = (ret << 1 ) | ( (data[i / 8+offset] >> ( i%8)) & 0x1);
	}

	return ret % Q;

}



/**
 * Convert a vector to its byte array representation
 *
 * @param vector a vector over the specified finite field
 * @return the byte array representation
 */
unsigned char* FiniteField::vectorToBytes(FiniteFieldVector* vector) {
	unsigned char *output = new unsigned char[bytesLength(vector->getLength())];
	vectorToBytes(vector, output, 0);
	return output;
}





void  FiniteField::vectorToBytes (FiniteFieldVector* vector, unsigned char* output, int start) {

	//int[] coordinates = vector.coordinates;

	unsigned int bits_per_field = bitsPerCoordinate();

	for (unsigned int i = 0 ; i < vector->getLength(); i++) {
		writeBits(output, start, i, vector->getCoordinate(i), bits_per_field);
	}

}




void FiniteField::writeBits(unsigned char* data, int offset, int field, int value, int fieldSize) {

	int start_bit, end_bit, i;

	value = value % Q;

	start_bit = field * fieldSize;
	end_bit = ( field + 1) * fieldSize;

	for ( i = start_bit; i < end_bit; i++) {

			unsigned char mask;
			unsigned char bit;

			mask = ( ~( 1 << (i % 8)));

			// prima veniva fatto un cas a byte
			bit = (( ( value >> (fieldSize - 1) ) & 0x1) << ( i % 8));
			// prima veniva fatto un cas a byte a (( data
			data[i / 8+offset] = (( data[i / 8 +offset] & mask) | bit);
			value = value << 1;
	}

}






/**
 * Returns the number of bytes that can be represented with a given number
 * of coordinates
 *
 * @param coordinatesCount  the number of
 * @return the number of bytes
 */
unsigned int FiniteField::bytesLength(int coordinates_count) {

	int bits_per_coordinate = bitsPerCoordinate();

	//return (int) Math.ceil(((double) (coordinates_count * bits_per_coordinate)) / 8.0);

	return (unsigned int)(ceil((coordinates_count * bits_per_coordinate)/ 8.0)  );
}



/**
 * Returns the number of bits necessary to represent an element of the field
 *
 * @return the number of bits necessary to store a field element
 */

unsigned int FiniteField::bitsPerCoordinate() {

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
unsigned int FiniteField::coordinatesCount(int bytes_length) {

	unsigned int bits_per_coordinate = bitsPerCoordinate();

	return bytes_length * 8 / bits_per_coordinate;

}



/**
 * Return the cardinality of the field
 * @return the number of elements in the field
 */
int FiniteField::getCardinality() {
	return Q;
}



////// TODO: sstream da fare, l'hash va fatto?
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
