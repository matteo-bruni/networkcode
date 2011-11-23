#include "ncutils.h"
#include <memory.h>



// TODO: non sembra mai usato
///**
// * Constructs a new UncodedPacket with the specified id and payload
// * The uncoded packet contains a reference to the payload therefore
// * any change to the array payload will be reflected in the contents
// * of the uncoded packet.
// *
// * @param id the id of the packet
// * @param payload the payload
// */
//public static UncodedPacket wrap(int id, byte[] payload) {
//	UncodedPacket pkt = new UncodedPacket(id);
//	pkt.payload = payload;
//	return pkt;
//}



UncodedPacket::UncodedPacket(int id) {
	this->id = id;
}

/**
 * Constructs a new UncodedPacket with the specified id and payload
 *
 * @param id the id of the packet
 * @param payload the payload
 */
UncodedPacket::UncodedPacket(int id, unsigned char* payload, int length) {
	this->id = id;
	this->payload = new unsigned char[length];
	memcpy(this->payload, payload, length);
	this->payload_length = length;
}


// TODO: serve vectortobytes
/**
 * Creates an uncoded packet with the specified Id and as payload the
 * byte array representation ( see FiniteField.vectorToBytes ) of a
 *  given vector
 *
 * @param id the id of the packet
 * @param payload a vector that will be the payload
 */
UncodedPacket::UncodedPacket(int id, FiniteFieldVector* vector) {

	this->id = id;
	this->payload = vector->getFiniteField()->vectorToBytes(vector);

}

/**
 * Returns the id of the packet
 * @return the id of the packet
 */
int UncodedPacket::getId() {
	return this->id;
}



/**
 * Returns the payload of the packet
 *
 * @return the byte array that contains the payload of the packet
 */
unsigned char* UncodedPacket::getPayload() {
	return payload;
}
int UncodedPacket::getPayloadLength(){
	return payload_length;
}

UncodedPacket* UncodedPacket::copy() {

	UncodedPacket* copy = new UncodedPacket(id, new unsigned char[payload_length], payload_length);
	memcpy(copy->payload, payload, payload_length);

	return copy;
}



//
//    @Override
//    public String toString() {
//        String ret = "Id: " + id + " Payload: ";
//        for (int k = 0; k < payload.length; k++) {
//            ret += String.format("%02X ", payload[k]);
//        }
//
//        return ret;
//    }
//

//
//	@Override
//	public int hashCode() {
//		final int prime = 31;
//		int result = 1;
//		result = prime * result + id;
//		result = prime * result + Arrays.hashCode(payload);
//		return result;
//	}
//
//	@Override
//	public boolean equals(Object obj) {
//		if (this == obj)
//			return true;
//		if (obj == null)
//			return false;
//		if (!(obj instanceof UncodedPacket))
//			return false;
//		UncodedPacket other = (UncodedPacket) obj;
//		if (id != other.id)
//			return false;
//		if (!Arrays.equals(payload, other.payload))
//			return false;
//		return true;
//	}
//
//    public int compareTo(UncodedPacket o) {
//
//        if (o.getId() == this.getId()) return 0;
//        else if ( o.id > this.id) return -1;
//        else return 1;
//
//    }
//
//
//}
