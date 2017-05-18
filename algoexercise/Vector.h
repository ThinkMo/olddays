#ifndef _VECTOR_H
#define _VECTOR_H

#include <algorithm>
#include <iostream>
#include <stdexecept>

template<typename Object>
class Vector
{
	private:
		int theSize;
		int theCapacity;
		Object *objects;

	public:
		explicit Vector(int initSize = 0)
			: theSize{ initSize },theCapacity{initSize + SPARE_CAPACITY }
		{ objects = new Object[ theCapacity ]; }

		Vector(const Vector &rhs)
			: theSize{ rhs.theSize },theCapacity{ rhs.theCapacity }, objects{ nullptr }
		{
			objects = new Object[ theCapacity ];
			for(int k = 0; k < theSize; ++k)
			  object[ k ] = rhs.objects[ k ];
	   	}

		Vector & operator= (const Vector &rhs)
		{
			Vector copy = rhs;
			std::swap(*this, copy );
			return *this;
		}

		~Vector()
		{	delete [] objects; }

		bool empty() const
		{ return size() == 0; }

		int size() const
		{ return theSize; }

		int capacity() const
		{ return theCapacity; }

		Object & operator[](int index)
		{
			if(index < 0 || index >= size() )
			  throw ArrayIndexOutOfBoundsException{ };

			return objects[index];
		}


		const Object & operator[](int index) const
		{
			if(index < 0 || index >= size() )
			  throw ArrayIndexOutOfBoundsException{ };

			return objects[index];
		}

		void resize(int newSize)
		{
			if(newSize > theCapacity )  
			  reserve( newSize * 2);
			theSize = newSize;
		}

		void reserve(int newCapacity)
		{
			if(newCapacity < theSize)
			  return;

			Object *newArray = new Object[ newCapacity ];
			for(int k = 0; k < theSize; k++)
			  newArray[k] = std::move( objects[k] );

			theCapacity = newCapacity;
			std::swap(Objects, newArray);
			delete [] newArray;
		}

		void push_back(const Object & x)
		{
			if( theSize == theCapacity)
			  reserve(2 * theCapacity +1);
			objects[ theSize++ ] = x;
		}

		void pop_back()
		{
			if( empty() )
			  throw UnderflowException{};
			--theSize;
		}

		typedef Object * iterator;
		typedef const Object * const_iterator;

		iterator begin()
		{return &objects[0];}

		const_iterator begin() const
		{return &objects[0];}

		iterator end()
		{ return &objects[size()];}

		const_iterator end() const
		{ return &objects[size()];}

		static const int SPARE_CAPACITY = 2;

};

#endif
