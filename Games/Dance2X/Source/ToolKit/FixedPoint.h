//! A class library to for fixed point variables with 8 bit pre

#ifndef FIXPOINT_H
#define FIXPOINT_H

#define PRECISION_IN_BITS 	(8)
#define PRECISION_IN_DEC 	(256)

class FixedPoint
{
private:
	//! Holds the fixed point value
	int _Value;

public:
	//! Standard constructor
	FixedPoint(void) : _Value(0)
	{
		// Intentionally left blank
	}

	//! Constructors to take all variable types
	FixedPoint(const float Value)
	{
		_Value = static_cast<int>(Value * PRECISION_IN_DEC);
	}

	FixedPoint(const double Value)
	{
		_Value = static_cast<int>(Value * PRECISION_IN_DEC);
	}

	FixedPoint(const unsigned int Value)
	{
		_Value = static_cast<int>(Value << PRECISION_IN_BITS);
	}

	FixedPoint(const int Value)
	{
		_Value = static_cast<int>(Value << PRECISION_IN_BITS);
	}

	//! Standard destructor
	~FixedPoint(void)
	{
		// Intentionally left blank
	}

	//! Assignment operator for all common types
	const FixedPoint & operator= (const float &Rhs)
	{
		_Value = static_cast<int>(Rhs * PRECISION_IN_DEC);
		return *this;
	}

	const FixedPoint & operator= (const double &Rhs)
	{
		_Value = static_cast<int>(Rhs * PRECISION_IN_DEC);
		return *this;
	}

	const FixedPoint & operator= (const unsigned int &Rhs)
	{
		_Value = static_cast<int>(Rhs << PRECISION_IN_BITS);
		return *this;
	}

	const FixedPoint & operator= (const int &Rhs)
	{
		_Value = static_cast<int>(Rhs << PRECISION_IN_BITS);
		return *this;
	}

	//! Addition operator for Fixed Point objects
	const FixedPoint operator+ (const FixedPoint &Rhs) const
	{
		FixedPoint Temp(*this);
		Temp._Value += Rhs._Value;
		return Temp;
	}

	//! Addition assignment operator for Fixed Point objects
	const FixedPoint & operator+= (const FixedPoint &Rhs)
	{
		_Value += Rhs._Value;
		return *this;
	}

	//! Subtraction operator for Fixed Point objects
	const FixedPoint operator- (const FixedPoint &Rhs) const
	{
		FixedPoint Temp(*this);
		Temp._Value -= Rhs._Value;
		return Temp;
	}

	//! Subtraction assignment operator for Fixed Point objects
	const FixedPoint & operator-= (const FixedPoint &Rhs)
	{
		_Value -= Rhs._Value;
		return *this;
	}

	//! Multiplication operator for Fixed Point objects
	const FixedPoint operator* (const FixedPoint &Rhs) const
	{
		FixedPoint Temp;
		Temp._Value = (_Value * Rhs._Value) >> PRECISION_IN_BITS;
		return Temp;
	}

	//! Multiplication assignment operator for Fixed Point objects
	const FixedPoint & operator*= (const FixedPoint &Rhs)
	{
		_Value = (_Value * Rhs._Value) >> PRECISION_IN_BITS;
		return *this;
	}

	//! Division operator for Fixed Point objects
	const FixedPoint operator/ (const FixedPoint &Rhs) const
	{
		FixedPoint Temp;
		Temp._Value = (_Value << PRECISION_IN_BITS) / Rhs._Value;
		return Temp;
	}

	//! Division assignment operator for Fixed Point objects
	const FixedPoint & operator/= (const FixedPoint &Rhs)
	{
		_Value = (_Value << PRECISION_IN_BITS) / Rhs._Value;
		return *this;
	}

	//! Comparison operator > for fixed points
	bool operator> (const FixedPoint &Rhs)
	{
		return (this->_Value > Rhs._Value);
	}

	//! Comparison operator >= for fixed points
	bool operator>= (const FixedPoint &Rhs)
	{
		return (this->_Value >= Rhs._Value);
	}

	//! Comparison operator < for fixed points
	bool operator< (const FixedPoint &Rhs)
	{
		return (this->_Value < Rhs._Value);
	}

	//! Comparison operator <= for fixed points
	bool operator<= (const FixedPoint &Rhs)
	{
		return (this->_Value <= Rhs._Value);
	}

	//! Comparison operator == for fixed points
	bool operator== (const FixedPoint &Rhs)
	{
		return (this->_Value == Rhs._Value);
	}

	//! Comparison operator != for fixed points
	bool operator!= (const FixedPoint &Rhs)
	{
		return (this->_Value != Rhs._Value);
	}

	//! Return the value as a float
	const float ToFloat(void) const
	{
		return static_cast<float>(_Value) / PRECISION_IN_DEC;
	}

	//! Return the value as a double
	const double ToDouble(void) const
	{
		return static_cast<double>(_Value) / PRECISION_IN_DEC;
	}

	//! Return the value as an integer
	const int ToInt(void) const
	{
		return _Value >> PRECISION_IN_BITS;
	}
};

#endif // FIXPOINT_H

/*
History
=======
2006-08-01: Fixed errors with the single operations
2006-06-29: Created file
*/
