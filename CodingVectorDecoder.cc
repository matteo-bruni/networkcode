#include <map>
#include "ncutils.h"
#include <iostream>
#include <memory.h>

///**
// *
// * This class is used to find how to linearly combine coding vectors in order
// * to obtain elementary vectors. Using this knowledge it is possible to
// * decoded coded packets.
// *
// * The algorithm used in this class is a modification of the standard Gaussian-
// * Jordan elimination. Each coding vector inserted is simplified with elementary
// * rows operations trying to obtain an elementary vector. The operations
// * performed are kept track in an auxiliary vector (one per added vector).
// *
// * Internally the class uses O(N²) memory where N is the maximum number of
// * packets that can get combined.
// *
// * @author lokeller
// */

/**
 * Construct a new decoder
 *
 * @param maxPackets the length of the vectors have to be decoded
 * @param ff the finite field used in the decoder
 */
CodingVectorDecoder::CodingVectorDecoder(int maxPackets, FiniteField* ff) {

	this->packetCount = 0;
	this->decodeMatrixLenght = maxPackets;

	decodeMatrix = new int*[maxPackets];

	for (int i = 0; i < maxPackets; i++) {
		decodeMatrix[i] = new int[maxPackets * 2];
		memset( decodeMatrix[i], 0, maxPackets*2*sizeof(int) );
	}

	pivotPos = new int[maxPackets];
	memset(pivotPos, 0, maxPackets*sizeof(int) );

	decoded = new bool[maxPackets];
	isPivot = new bool[maxPackets];

	this->ff = ff;

}


CodingVectorDecoder::~CodingVectorDecoder(){
	//std::cout << "Distruttore CodingVectorDecoder " <<std::endl;


	for(int i = 0; i < decodeMatrixLenght; ++i){
		delete [] decodeMatrix[i];
	}

	delete [] decodeMatrix;
	delete [] pivotPos;
	delete [] isPivot;
	delete [] decoded;

	//std::cout << "finito " <<std::endl;
}

/**
 * Returns the maximum number of packets that can be combined (i.e. the
 * length of the coding vectors being decoded)
 *
 * @return the number of packets supported
 */
int CodingVectorDecoder::getMaxPackets() {
	//return decodeMatrix.length;
	return decodeMatrixLenght;
}

/**
 *
 * Adds the coding vector to the internal decoding buffer and returns
 * for new each elementary vector that can be constructed by linearly
 * combining the vectors in the coding buffer a vector with the
 * coefficients necessary to create it.
 *
 * @param v a coding vector of length compatible with the decoder
 * @return a map that associates an id of an uncoded packet with a vector
 * containing the coefficients that must be used to recover its payload
 *
 * @throws LinearDependantException the vector being added is linearly dependant
 * from the previously decoded vectors.
 */
//Map<Integer,FiniteFieldVector>
std::map<int, FiniteFieldVector*> CodingVectorDecoder::addVector(FiniteFieldVector* v) throw(LinearDependantException) {


	/* if the matrix is already full rank don't add this vector */
	if ( packetCount == decodeMatrixLenght) {
		throw LinearDependantException();
	}

	// TODO: uso direttamente ff ?
//	final int [][] mul = ff.mul;
//	final int [][] sub = ff.sub;
//	final int [][] div = ff.div;



	int size = decodeMatrixLenght;
	//int totalSize = decodeMatrix[0].length;
	int totalSize = decodeMatrixLenght*2;


	/* add the received packet at the bottom of the matrix */
	for ( int i = 0 ; i < v->getLength() ; i++) {
		//std::cout << "packetcount: "<< packetCount <<std::endl;
		//std::cout << "coordinata "<< v->getCoordinate(i) <<std::endl;
		decodeMatrix[packetCount][i] = v->getCoordinate(i);
		decodeMatrix[packetCount][i+size] = 0 ;
	}


	/* put zeros on the inverse matrix but on position packet count*/
	for ( int i = size ; i < totalSize ; i++) {
		decodeMatrix[packetCount][i] = 0 ;
	}


	decodeMatrix[packetCount][size + packetCount] = 1;


	/* simplify the new packet */

	/* zeros before */
	for (int i = 0 ; i < packetCount ; i++)  {

		int m = decodeMatrix[packetCount][pivotPos[i]];
		if (m == 0) continue;

		for (int j = 0 ; j < totalSize ; j++) {
			int val = decodeMatrix[packetCount][j];
			int val2 = decodeMatrix[i][j];
			decodeMatrix[packetCount][j] = ff->sub[val][ff->mul[val2][m]];
		}

	}

	/* find pivot on the line */
	int pivot = -1;
	for (int i = 0 ; i < size ; i++) {
		if (isPivot[i]) continue;
		if (decodeMatrix[packetCount][i] != 0) {
			pivotPos[packetCount] = i;
			isPivot[i] = true;
			pivot = i;
			break;
		}
	}

	/* if the packet is not li stop here */

	if (pivot == -1 ) {
		throw LinearDependantException();
	}


	/* divide the line */

	if ( decodeMatrix[packetCount][pivot] != 1 ) {
		int pval = decodeMatrix[packetCount][pivot];

		for (int j = 0 ; j < totalSize ; j++) {
				int val = decodeMatrix[packetCount][j];
				decodeMatrix[packetCount][j] = ff->div[val][pval];
		}

	}

	/* zero the column above the pivot */
	for ( int i = 0 ; i < packetCount ; i++ ) {

		int m = decodeMatrix[i][pivot];

		if (m == 0) continue;

		for (int j = 0 ; j < totalSize ; j++) {

			int val2 = decodeMatrix[packetCount][j];
			int val = decodeMatrix[i][j];

			decodeMatrix[i][j] = ff->sub[val][ff->mul[val2][m]];

		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////

	packetCount++;

	/* look for decodable blocks */

	//HashMap<Integer,FiniteFieldVector> willDecode =
	//				new HashMap<Integer, FiniteFieldVector>();

	std::map<int, FiniteFieldVector*> willDecode;

	for ( int i = 0; i < packetCount ; i++) {
		int pos = -1;

		/* skip if the line is marked decoded */
		if (decoded[i]) continue;

		for ( int j = 0 ; j < size ; j++) {

			if (decodeMatrix[i][j] != 0 && pos != -1) {
				pos = -1;
				break;
			} else if (decodeMatrix[i][j] != 0) pos = j;
		}

		if ( pos >= 0) {

			decoded[i] = true;

			/* build the vector that explains how to obtain the block */
			FiniteFieldVector* vector = new FiniteFieldVector(decodeMatrixLenght, ff);

			//int[] coordinates = vector.coordinates;

			for ( int j = size ; j < size + size ; j++) {

				vector->setCoordinate(j-size, decodeMatrix[i][j]);
				//coordinates[j-size] = decodeMatrix[i][j];
			}

			// TODO:
			//willDecode.put(pos, vector);
			willDecode[pos] = vector;

		}
	}

	return willDecode;

}
/////////////////////////////////////////////////////////


/**
 * Returns the number of linearly independent coding vectors received
 * up to now.
 *
 * @return a number between 0 and getMaxPackets()
 */
int CodingVectorDecoder::getSubspaceSize() {
	return packetCount;
}



