#pragma once


// ACF includes
#include <icalib/CAffineCalibration2d.h>
#include <icalib/CPerspectiveCalibration2d.h>
#include <icalib/CLUTCalibration2d.h>
#include <icalib/CSimpleLensCorrection.h>


namespace icalib
{


// a simple header-only calibration factory for known classes
// to avoid cloning of whole compositums because of memory leaks and dangling pointers upon destruction

inline istd::IChangeableUniquePtr FactorizeFrom(const istd::IChangeable* calibPtr)
{
	if (!calibPtr)
		return nullptr;

	istd::IChangeable* resultPtr = nullptr;

	if (auto affinePtr = dynamic_cast<const icalib::CAffineCalibration2d*>(calibPtr)) {
		resultPtr = new icalib::CAffineCalibration2d;
	}
	else if (auto perspPtr = dynamic_cast<const icalib::CPerspectiveCalibration2d*>(calibPtr)) {
		resultPtr = new icalib::CPerspectiveCalibration2d;
	}
	else if (auto lutPtr = dynamic_cast<const icalib::CLUTCalibration2d*>(calibPtr)) {
		resultPtr = new icalib::CLUTCalibration2d;
	}
	else if (auto lensPtr = dynamic_cast<const icalib::CSimpleLensCorrection*>(calibPtr)) {
		resultPtr = new icalib::CSimpleLensCorrection;
	}

	if (resultPtr) {
		resultPtr->CopyFrom(*calibPtr);
		return resultPtr;
	}

	// default clone
	return calibPtr->CloneMe();
}


}
