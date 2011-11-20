
#include <string>


int ipow(int base, int exp);

class FiniteFieldVector;

class FiniteField {

	public:
		/**
		 * Associates each field element to its inverse
		 */
		int *inverse;

		/**
		 * Associate two field elements to their sum
		 */
		int **sum;

		/**
		 * Associate two field elements to their division
		 */
		int **div;

		/**
		 * Associate two field elements to their substraction
		 */
		int **sub;

		/**
		 * Associate two field elements to their multiplication
		 */
		int **mul;


		FiniteField(int q=2, int m=4);
		FiniteField(int q);

		FiniteFieldVector byteToVector(char *bytes);

		int bytesLength(int coordinates_count);
		int bitsPerCoordinate();
		int coordinatesCount(int bytes_length);
		int getCardinality();


	private:
		/**
		 * Total number of elements in the field
		 */
		int Q;

};


class FiniteFieldVector {

	public:

		FiniteFieldVector(int len, const FiniteField& ff);
		FiniteFieldVector(int coords[], int len, const FiniteField& field);

		int getLength();
		FiniteField getFiniteField();

		void setCoordinate(int index, int value);
		int getCoordinate(int index);
		void setToZero();

		FiniteFieldVector* copy();
		FiniteFieldVector* add(FiniteFieldVector& vector);
		void addInPlace(FiniteFieldVector& vector);

		FiniteFieldVector* scalarMultiply(int c);
		void scalarMultiplyInPlace(int c);

		FiniteFieldVector* multiplyAndAdd(int c, FiniteFieldVector& other);
		void multiplyAndAddInPlace(int c, FiniteFieldVector& other);

		std::string toString();



	private:
		int *coordinates;
		int length;
		FiniteField ff ;

};
