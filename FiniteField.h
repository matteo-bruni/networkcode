/*
 * number of elements of the field p^m
 */
#ifndef P_FIELD_CHARACTERISTIC
#define P_FIELD_CHARACTERISTIC 2
#endif


int ipow(int base, int exp);


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

		/**
		 * Returns the default finite field
		 *
		 * @return a finite field
		 */
		//static FiniteField* getDefaultFiniteField();

		int getCardinality();

	private:
		/**
		 * Total number of elements in the field
		 */
		int Q;

		static FiniteField *defaultFiniteField;


};
