
#include "ncutils.h"
#include <assert.h>



 /*
 * Create a new coded packet containing only one uncoded packet. The
 * coding vector of this packet is an elementary vector, i.e. all entries
 * but one, corresponding to the uncoded packet id, are zero.
 *
 * @param packet the uncoded packet that will be put in the coded packet
 * @param maxPackets the maximal number of uncoded packets that can be combined
 * in this coded packet. This correspond to the length of teh coding vector.
 * @param ff The finite fielconstructor ‘CodedPacket::CodedPacket(UncodedPacket, int, FiniteField*)’:
 * d that over which the vectors in the packet are
 * defined
 */
CodedPacket::CodedPacket( UncodedPacket* packet, int max_packets, FiniteField* ff) {

	this->coding_vector = new FiniteFieldVector(max_packets, ff) ;
	this->payload_vector = ff->byteToVector(packet->getPayload(), packet->getPayloadLength() , 0);
	this->coding_vector->setCoordinate(packet->getId(), 1);

}


/**
 * Creates an empty coded packet, i.e. the coding vector is set to the zero
 * vector (and consequentely the payload vector is set to zero).
 *
 * @param maxPackets the maximal number of uncoded packets that can be combined
 * in this coded packet. This correspond to the length of teh coding vector.
 * @param payloadByteLen the length in bytes of the uncoded packets that can be combined
 * in this packet. The length of the payload vector of this packet will be choosen
 * based on this number accordingly to the finite field used.
 * @param ff The finite field that over which the vectors in the packet are
 * defined
 */
CodedPacket* CodedPacket::createEmptyCodedPacket(int max_packets, int payload_byte_lenght, FiniteField* ff) {

	return new CodedPacket(new FiniteFieldVector(max_packets, ff), new FiniteFieldVector(ff->coordinatesCount(payload_byte_lenght), ff));

}


/**
 * Creates a coded packet from its binary representation.
 *
 * @param maxPackets the maximal number of uncoded packets that can be combined
 * in this coded packet. This correspond to the length of the coding vector.
 *
 * @param data an array containing the binary representation of the coded packet
 * @param offset the first byte of the binary representation in the array data
 * @param length the length of the binary representation
 * @param ff The finite field that over which the vectors in the packet are
 * defined
 */

CodedPacket::CodedPacket(int maxPackets, unsigned char* data, int offset, int length, FiniteField* ff) {

	int header_byte_lenght = ff->bytesLength(maxPackets);

	this->coding_vector = ff->byteToVector(data, header_byte_lenght, offset);
	this->payload_vector = ff->byteToVector(data, length - header_byte_lenght, header_byte_lenght+offset);

}

/*
 * Private Constructor
 *
 */
CodedPacket::CodedPacket(FiniteFieldVector* codingVector, FiniteFieldVector* payloadVector) {
	this->coding_vector = codingVector;
	this->payload_vector = payloadVector;
}



/**
 * Returns the coding vector of this packet. The coding vector describes
 * which, and how uncoded packets have been combined to form this coded
 * packet. WARNING: Changing the coding vector of a packet without updating accordingly
 * the payload vector introduces decoding errors.
 *
 * @return the coding vector of this packet
 */
FiniteFieldVector* CodedPacket::getCodingVector() {
   return coding_vector;
}



/**
 * Returns the payload vector of this packet. The payload vector is a
 * a linear combination of uncoded packets (seen as finite field vectors).
 * If the coding vector is ( a1, a2, ...., an) then the payload vector is
 * a1 * p1 + a2 * p2 + ... + an * pn where p1, ...pn are the finite field
 * vector  representations of the payload of the uncoded packets.
 *
 * @return the payload vector of this packet
 */
FiniteFieldVector* CodedPacket::getPayloadVector() {
	return payload_vector;
}


/**
 * Returns the finite field that is used to define the vectors
 * of this packet
 *
 * @return the finite field of the coding and payload vectors
 */
FiniteField* CodedPacket::getFiniteField() {
	return coding_vector->getFiniteField();
}


/**
 *
 * Set the index-th coordinate of the vector representation of the packet. If
 * index is smaller than the length of the coding vector the corresponding
 * coding vector coordinate will be set, otherwise the cofficient index -
 * (lenght of the coding vector) of the payload will be set
 *
 *
 * @param index the index of the coordinate that must be set
 * @param value an element of the field over which the packet is defined
 */
void CodedPacket::setCoordinate(int index, int value) {

	assert( index >= 0);
	assert(value < getFiniteField()->getCardinality() && value >= 0);

	if ( index < coding_vector->getLength()) {
		coding_vector->setCoordinate(index, value);
	} else {
		payload_vector->setCoordinate(index - coding_vector->getLength(), value);
	}
}


/**
 *
 * Get the index-th coordinate of the vector representation of the packet. If
 * index is smaller than the length of the coding vector the corresponding
 * coding vector coordinate will be returned, otherwise the coefficient index -
 * (length of the coding vector) of the payload will be returned
 *
 * @param index the index of the coordinate that must be retrieved
 * @return the value of the coordinate, an element of the field over which the packet is defined
 */
int CodedPacket::getCoordinate(int index) {

	assert(index >= 0);

	if ( index < coding_vector->getLength()) {
		return coding_vector->getCoordinate(index);
	} else {
		return payload_vector->getCoordinate(index - coding_vector->getLength());
	}
}


/**
 * Creates a copy of the packet
 *
 * @return a copy of the packet
 */
CodedPacket* CodedPacket::copy() {

	return new CodedPacket(coding_vector->copy(), payload_vector->copy());

}

/**
 * Set the packet contents to be a linear combination of no uncoded packets.
 * This sets coding and payload vector of the packet to zero.
 */
void CodedPacket::setToZero() {
	coding_vector->setToZero();
	payload_vector->setToZero();

}

/**
 *
 * Returns a CodedPacket which is the sum of the current CodedPacket and
 * another packet. The created packet will have a coding and payload vector
 * which will be consistent, i.e. the content of the payload of the newly
 * created packet corresponds to the linear combination specified in its
 * coding vector
 *
 * @param vector the CodedPacket that will be summed
 * @return the sum of this and vector
 */
CodedPacket* CodedPacket::add(CodedPacket* vector) {

	assert(vector->getFiniteField() == getFiniteField());

	return new CodedPacket(codingVector.add(vector.codingVector), payloadVector.add(vector.payloadVector));

}
//
//    /**
//    *
//    * Adds the specified CodedPacket to the current CodedPacket. This method
//    * modifies the CodedPacket.
//    *
//    * @param vector the CodedPacket that will be summed
//    *
//    */
//    public void  addInPlace(CodedPacket vector) {
//    	codingVector.addInPlace(vector.codingVector);
//    	payloadVector.addInPlace(vector.payloadVector);
//    }
//
//    /**
//     *
//     * Returns a CodedPacket which is a scalar multiple of the current
//     * CodedPacket. The created packet will have a coding and payload vector
//     * which will be consistent, i.e. the content of the payload of the newly
//     * created packet corresponds to the linear combination specified in its
//     * coding vector
//     *
//     * @param c an element of the finite field used to define this packet that
//     * will be used to multiply the packet
//     * @return the scalar multiple of the current packet, i.e. each coordinate
//     * of the current packet will be multiplied by c.
//     */
//    public CodedPacket scalarMultiply(int c) {
//        assert(c < getFiniteField().getCardinality() && c >= 0);
//
//        return new CodedPacket(codingVector.scalarMultiply(c), payloadVector.scalarMultiply(c));
//
//    }
//
//    /**
//    *
//    * Multiplies the CodedPacket by a scalar. This method modifies the CodedPacket.
//    *
//    * @param c an element of the finite field used to define this packet that
//    * will be used to multiply the packet
//    *
//    */
//
//    public void scalarMultiplyInPlace(int c) {
//    	codingVector.scalarMultiplyInPlace(c);
//    	payloadVector.scalarMultiply(c);
//    }
//
//
//    /**
//    *
//    * Returns a CodedPacket which is the sum of the current packet and a scalar multiple
//    * of the another CodedPacket. The created packet will have a coding and
//    * payload vector which will be consistent, i.e. the content of the payload of the
//    * newly created packet corresponds to the linear combination specified in its
//    * coding vector
//    *
//    * @param c an element of the finite field used to define this packet that
//    * will be used to multiply the packet that will be added
//    * @param packet a packet that will be multiplied by c and then added to obtain the resulting
//    * packet
//    * @return the sum of the current packet and the scalar multiple of packet. The i-th coordinate
//    * of this vector is equal to the sum of the i-th coordinate of the current vector and the i-th
//    * coordinate of packet multiplied by c
//    */
//    public CodedPacket multiplyAndAdd(int c, CodedPacket packet) {
//        assert(packet.getFiniteField() == getFiniteField());
//
//        return new CodedPacket(codingVector.multiplyAndAdd(c, packet.codingVector), payloadVector.multiplyAndAdd(c, packet.payloadVector));
//
//    }
//
//    /**
//     * Adds to the current packet the CodedPacket other multiplied by c. This
//     * method modifies the current CodedPacket
//     *
//     * @param c an element of the finite field used to define this
//     * @param other another packet with the parameters as the current packet
//     */
//    public void  multiplyAndAddInPlace(int c, CodedPacket other) {
//    	codingVector.multiplyAndAddInPlace(c, other.codingVector);
//    	payloadVector.multiplyAndAddInPlace(c, other.payloadVector);
//    }
//
//    /**
//     * Returns the binary representation of the packet
//     *
//     * @return a byte array containing coding vector and payload
//     */
//    public byte[] toByteArray() {
//
//    	FiniteField ff = getFiniteField();
//
//    	int headerLength = ff.bytesLength(codingVector.getLength());
//
//		byte[] ret = new byte[headerLength + ff.bytesLength(payloadVector.getLength())];
//
//    	ff.vectorToBytes(codingVector, ret, 0);
//    	ff.vectorToBytes(payloadVector, ret, headerLength);
//
//    	return ret;
//
//    }
//
//
//    @Override
//    public String toString() {
//
//        return codingVector.toString() + " | " + payloadVector.toString();
//
//    }
//
//
//
//
//}
