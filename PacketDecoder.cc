#include "ncutils.h"
#include <assert.h>

/**
 * Constructs a new PacketDecoder.
 *
 * @param field the finite field over which the decoder will operate
 * @param maxPackets the maximum number of coded packets, i.e. the length of
 * the coding vectors
 * @param payloadBytesLength the length in bytes of the payload of the packets
 */
PacketDecoder::PacketDecoder(FiniteField* field, int maxPackets, int payloadBytesLength) {

	this->ff = field;
	codingVectorDecoder = new CodingVectorDecoder(maxPackets,ff);
	this->payloadCoordinatesCount = ff->coordinatesCount(payloadBytesLength);
}

PacketDecoder::~PacketDecoder(){
	delete codingVectorDecoder;
}

/**
 *
 * Add a coded packet to the decoding buffer and returns all the new uncoded
 * packets that can be decoded thanks to it.
 *
 * @param p a CodedPacket with payload length and coding coefficient length
 * compatible with the decoder
 * @return a vector of uncoded packets that have been decoded thanks to this
 * coded packet (and what was previously added)
 */
std::vector<UncodedPacket*> PacketDecoder::addPacket(CodedPacket* p) {

	assert(p->getFiniteField()->getCardinality() == ff->getCardinality());
	assert(p->getCodingVector()->getLength() == codingVectorDecoder->getMaxPackets());
	assert(p->getPayloadVector()->getLength() == payloadCoordinatesCount);

	std::vector<UncodedPacket*> output;

	try {

		std::map<int, FiniteFieldVector*> decoded = codingVectorDecoder->addVector(p->getCodingVector());

		/* add the current packet only if it was linearly independant, this
		 will be used to decode future packets*/
		this->packets.push_back(p);
		//packets.add(p);

		/* decode the new packets that can be decoded */
		// messo fuori dal try
		//std::vector<UncodedPacket*> output;// = new std::vector<UncodedPacket*>();

		std::map<int, FiniteFieldVector*>::iterator iter;
		for (iter = decoded.begin(); iter != decoded.end(); iter++) {

			FiniteFieldVector* decodedPayload = decodePayload(iter->second);

			output.push_back(new UncodedPacket(iter->first, decodedPayload));

			// we create a copy
			delete decodedPayload;
		}

		// free memory not used anymore
		for (iter = decoded.begin(); iter != decoded.end(); iter++) {
			delete iter->second;
		}

//		for ( Map.Entry<Integer, FiniteFieldVector> entry : decoded.entrySet() ) {
//
//			FiniteFieldVector decodedPayload = decodePayload(entry.getValue());
//
//			output.add(new UncodedPacket((int) entry.getKey(), decodedPayload));
//
//		}

		return output;


	} catch (LinearDependantException ex) {
		return output;//std::vector<UncodedPacket*>();
	}

}



FiniteFieldVector* PacketDecoder::decodePayload(FiniteFieldVector* encoding) {

	/*
	 * this vector will store the linear combination of coded payloads that
	 * correspond to the decoded payload
	 */
	FiniteFieldVector* decodedPayload = new FiniteFieldVector(payloadCoordinatesCount, ff);

	//int coordinates3 = decodedPayload.coordinates;

	/* linearly combine the payloads */

	//int[] coordinates = encoding.coordinates;

	for (int codedPacketId = 0; codedPacketId < encoding->getLength(); codedPacketId++) {

		int coeff = encoding->getCoordinate(codedPacketId); // coordinates[codedPacketId];

		/* skip the packet if the coordinate is zero */
		if (coeff == 0) {
			continue;
		}

		FiniteFieldVector* codedPayload = packets[codedPacketId]->getPayloadVector();

		//int[] coordinates2 = codedPayload.coordinates;

		/* linearly combine the payload of packet "codedPacketId" */
		for (int c = 0; c < codedPayload->getLength(); c++) {
			int v2 = codedPayload->getCoordinate(c); //coordinates2[c];
			int v1 = decodedPayload->getCoordinate(c); //coordinates3[c];
			int val = ff->sum(v1, ff->mul(coeff, v2));
			decodedPayload->setCoordinate(c, val); //coordinates3[c] = val;
		}
	}

	return decodedPayload;
}

//
//
//    public List<CodedPacket> getCodedPackets() {
//    	return packets;
//    }
//

/**
 * Returns the maximum number of packets that can be combined
 *
 * @return the number of packets that can be combined at most
 */
int PacketDecoder::getMaxPackets() {
	return codingVectorDecoder->getMaxPackets();
}


/**
 *
 * Returns the number of linearly independent packets received up to now
 *
 * @return a number between 0 and getMaxPackets()
 */
int PacketDecoder::getSubspaceSize() {
	return codingVectorDecoder->getSubspaceSize();
}

