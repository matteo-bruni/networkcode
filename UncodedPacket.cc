#include "ncutils.h"
#include <memory.h>
#include <sstream>


#include <stdio.h>


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
	this->payload_length = 0;
}

/**
 * Constructs a new UncodedPacket with the specified id and payload
 *
 * @param id the id of the packet
 * @param payload the payload
 */
UncodedPacket::UncodedPacket(int id, unsigned char* payload_vector, int length) {
	this->id = id;
	this->payload = new unsigned char[length];
	memcpy(this->payload, payload_vector, length);
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

UncodedPacket::~UncodedPacket(){

	std::cout << "Distruttore UncodedPacket " <<std::endl;

	delete [] payload;


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




std::string UncodedPacket::toString() {

//	std::ostringstream os;
//	os << "Hello test " << someint << ' ' << somestring;
//	std::string buffer(os.str());


	//std::stringstream sstm;

	//TODO FIXME
	//std::string ret = "Id: " + id + " Payload: ";

	//sstm << "Id: " << id << " Payload: ";
	//std::string ret = sstm.str();


    char tmp[10];
    char buffer[500];
    buffer[0] = 0;
    int len = 500;

    snprintf(buffer, len, "ID: %d - " , this->id);

    for (int i = 0 ; i < this->payload_length ; i++) {
        snprintf(tmp, 10, "%02hhx ", this->payload[i]);

        if ( strlen(buffer) + strlen(tmp) < len) {
            strcat(buffer, tmp);
        }
    }


    std::string ret(buffer);




	//std::stringstream sstm;

	//TODO FIXME
	//std::string ret = "Id: " + id + " Payload: ";

	//sstm << "Id: " << id << " Payload: ";
	//std::string ret = sstm.str();




//	for (int k = 0; k < payload_length; k++) {
//		//std::ostringstream oss;
//		//oss << payload[k];
//		//ret += " "+ oss.str();
//		printf(" %x ", payload[k]);
//		//ret += std::string(reinterpret_cast<char>(payload[k]));
//		//ret += String.format("%02X ", payload[k]);
//	}
	//printf(" \n");

	return ret;
}


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
