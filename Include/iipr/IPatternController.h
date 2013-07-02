#ifndef iipr_IPatternController_included
#define iipr_IPatternController_included


#include "istd/IChangeable.h"


namespace iipr
{


class IPatternController: virtual public istd::IChangeable
{
public:
	static const int CF_PATTERN_LEARNED = (1 << 20);

	/**
		Do teaching of the pattern.
	*/
	virtual bool TeachPattern(const istd::IChangeable* sourceObjectPtr = NULL) = 0;

	/**
		Clear all pattern features.
	*/
	virtual void ResetPattern() = 0;

	/**
		Get \c true of the pattern is valid
	*/
	virtual bool IsPatternValid() const = 0;

	/**
		Get pattern object.
	*/
	virtual const iser::ISerializable* GetPatternObject() const = 0;
};


} // namespace iipr


#endif // !iipr_IPatternController_included
