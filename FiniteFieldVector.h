

#include "FiniteField.h"
#include <vector>



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


	private:
		int *coordinates;
		int length;
		FiniteField ff ;

};

