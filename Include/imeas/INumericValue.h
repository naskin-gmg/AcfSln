#ifndef imeas_INumericParams_included
#define imeas_INumericParams_included


#include "iser/ISerializable.h"

#include "imath/CVarVector.h"


namespace imeas
{


class INumericConstraints;


/**
	General parameter set containing list of numeric values.
*/
class INumericValue: virtual public iser::ISerializable
{
public:
	/**
		Define standard numeric value types.
	*/
	enum ValueTypeId
	{
		/**
			Automatic value, implementation specific.
		*/
		VTI_AUTO = 0,

		/**
			Weight of some value
			This value should be normalized to range [0, 1].
		*/
		VTI_WEIGHT,

		/**
			Value of position (can be N-dimensional).
		*/
		VTI_POSITION,

		/**
			Value of radius.
		*/
		VTI_RADIUS,

		/**
			Scaling factor(s).
		*/
		VTI_SCALE,

		/**
			Angle in radian.
		*/
		VTI_ANGLE,

		/**
			Alpha value (proportion).
		*/
		VT_ALPHA,

		/**
			Length or distance.
		*/
		VTI_LENGTH,

		/**
			2D-transformation, given in the form (m11, m21, m12, m22, dx, dy)
		*/
		VTI_2D_TRANSFORM,

		/**
			Line in 2D-space given in the form (x1, y1, x2, y2)
		*/
		VTI_2D_LINE
	};

	/**
		Check if the given value type (or value component) is supported.
		If the value component is supported you can access this via GetComponentValue method.
		\sa GetComponentValue
	*/
	virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const = 0;

	/**
		Get parameter constraints describing possible parameter values.
	*/
	virtual const INumericConstraints* GetNumericConstraints() const = 0;

	/**
		Get access to component value of the given type.
		\sa ValueTypeId
		\return	if data vector cannot be calculated it returns empty vector.
	*/
	virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const = 0;

	/**
		Get list of numeric values.
	*/
	virtual imath::CVarVector GetValues() const = 0;

	/**
		Set list of numeric values.
	*/
	virtual bool SetValues(const imath::CVarVector& values) = 0;
};


} // namespace imeas


#endif // !imeas_INumericParams_included


