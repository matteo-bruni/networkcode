
#include <assert.h>
#include <memory.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>


//#include "FiniteFieldVector.h"
#include "ncutils.h"

/**
 * Constructs a vector
 *
 * @param length the number of coordinates of the vector
 * @param ff the finite field used to define the vector
 */
FiniteFieldVector::FiniteFieldVector(unsigned int length, FiniteField* field) {
	ff = field;
	coordinates = new int[length];
	this->length = length;
	setToZero();
}

FiniteFieldVector::FiniteFieldVector(int coords[], unsigned int coords_len, FiniteField* field) {
	ff = field;
	coordinates = coords;
	this->length = coords_len;
}

FiniteFieldVector::~FiniteFieldVector(){
	delete [] coordinates;
}

/**
 * Returns the number of coordinates of the vector
 *
 * @return the length of the vector
 */
unsigned int FiniteFieldVector::getLength() {
	return length;
}


/**
 * Returns the finite field over which the vector is defined
 *
 * @return a finite field
 */
FiniteField* FiniteFieldVector::getFiniteField() {
	return ff;
}

/**
 * Set a coordinate of the vector
 *
 * @param index the index of the coordinate (starts at 0)
 * @param value the value of the coordinate, must be an element of
 * the finite field where the vector has been defined
 */
void FiniteFieldVector::setCoordinate(unsigned int index, int value) {

	assert(index >= 0);
	assert(value < ff->getCardinality() && value >= 0);
	assert(index <= length);
	coordinates[index] = value;
}


/**
 *
 * Returns a coordinate of the vector
 *
 * @param index the index of the coordinate (starts at 0)
 * @return an element of the finite field used to define this vector
 */
int FiniteFieldVector::getCoordinate(unsigned int index) {

	assert(index >= 0);
	assert(index <= length);

	return coordinates[index];

}

/**
 * Sets all the coordinates of the vector to zero
 */
void FiniteFieldVector::setToZero() {

	//coordinates[length-1] = {0};
	memset( coordinates, 0, length*sizeof(int) );
	//Arrays.fill(coordinates, 0);
}


//
/**
 * Creates a copy of the vector
 *
 * @return a copy of the vector
 */
FiniteFieldVector* FiniteFieldVector::copy() {

	FiniteFieldVector *vector = new FiniteFieldVector(length, ff);

	memcpy(vector->coordinates, coordinates, length*sizeof(int));

	return vector;
}

/**
 * Returns the sum of this vector and another vector
 *
 * @param vector the other summand
 * @return the sum of this and vector
 */
FiniteFieldVector* FiniteFieldVector::add(FiniteFieldVector* vector) {

	assert(vector->getFiniteField()->getCardinality() == ff->getCardinality());
	assert(vector->length == this->length);

	FiniteFieldVector *out = new FiniteFieldVector(getLength(), ff);


	for ( unsigned int i = 0 ; i < this->length ; i++ ) {
		out->setCoordinate(i, ff->sum(this->coordinates[i], vector->getCoordinate(i)) );
	}

	return out;
}


void FiniteFieldVector::addInPlace(FiniteFieldVector* vector) {

	assert(vector->getFiniteField()->getCardinality() == ff->getCardinality());
	assert(vector->length == this->length);

	for (unsigned int i = 0 ; i < this->length ; i++ ) {
		coordinates[i] = ff->sum(coordinates[i], vector->getCoordinate(i));
	}

}


/**
 * Returns the scalar multiplication of this vector by a coefficient
 *
 * @param c an element form the field used to define the vector
 * @return the scalar multiple of this vector
 */
FiniteFieldVector* FiniteFieldVector::scalarMultiply(int c) {

	assert(c < ff->getCardinality() && c >= 0);

	FiniteFieldVector *out = new FiniteFieldVector(getLength(), ff);

	for ( unsigned int i = 0 ; i < this->length ; i++ ) {
		out->setCoordinate(i, ff->mul(coordinates[i], c) );
	}
	return out;
}


void FiniteFieldVector::scalarMultiplyInPlace(int c) {
	for ( unsigned int i = 0 ; i < this->length ; i++ ) {
		coordinates[i] = ff->mul(coordinates[i],c);
	}
}


FiniteFieldVector* FiniteFieldVector::multiplyAndAdd(int c, FiniteFieldVector* other) {

	assert(other->getFiniteField()->getCardinality() == ff->getCardinality());
	assert(other->length == this->length);
	assert(c < ff->getCardinality() && c >= 0);

	FiniteFieldVector* out = new FiniteFieldVector(getLength(), ff);

	for (unsigned int i = 0 ; i < this->length ; i++ ) {
		out->setCoordinate(i, ff->sum(ff->mul(other->getCoordinate(i),c), coordinates[i]) );
	}

	return out;

}



void FiniteFieldVector::multiplyAndAddInPlace(int c, FiniteFieldVector* other) {

	assert(other->getFiniteField()->getCardinality() == ff->getCardinality());
	assert(other->length == this->length);

	for (unsigned int i = 0 ; i < this->length ; i++ ) {
		coordinates[i] = ff->sum( ff->mul(other->getCoordinate(i), c), coordinates[i]);
	}

}

// TODO: implement osstring method
std::string FiniteFieldVector::toString() {
		std::string ret;
		for (unsigned int i = 0 ; i < this->length ; i++ ) {
				std::ostringstream oss;
				oss << std::setfill ('0') << std::setw (2);
				oss << coordinates[i];
				ret += " "+ oss.str();
				//ret += (ret.length() != 0 ? " " : "") + String.format("%02d ", c); ;
		}

		return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////

//
//
//    @Override
//        public int hashCode() {
//                final int prime = 31;
//                int result = 1;
//                result = prime * result + Arrays.hashCode(coordinates);
//                result = prime * result + ((ff == null) ? 0 : ff.hashCode());
//                return result;
//        }
//
//        @Override
//        public boolean equals(Object obj) {
//                if (this == obj)
//                        return true;
//                if (obj == null)
//                        return false;
//                if (!(obj instanceof FiniteFieldVector))
//                        return false;
//                FiniteFieldVector other = (FiniteFieldVector) obj;
//                if (!Arrays.equals(coordinates, other.coordinates))
//                        return false;
//                if (ff == null) {
//                        if (other.ff != null)
//                                return false;
//                } else if (!ff.equals(other.ff))
//                        return false;
//                return true;
//        }
//

//
//
//}
