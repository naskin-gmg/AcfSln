#ifndef ialgo_TIHoughSpace_included
#define ialgo_TIHoughSpace_included


// Qt includes
#include <QtCore/QMultiMap>

// ACF includes
#include <istd/TIndex.h>
#include <imath/TVector.h>


namespace ialgo
{


/**
	Template interface for Hough space.
*/
template <int Dimensions, class Element = double>
class TIHoughSpace: virtual public iser::ISerializable
{
public:
	class ResultsConsumer: virtual public istd::IPolymorphic
	{
	public:
		/**
			Get list of number of neighbours supprted by this consumer.
			For example if it works correctly with 4 and 8 neighbourshood it should return {4, 8}.
		*/
		virtual QList<int> GetSupportedNeghboursCount() const = 0;

		/**
			Called when processing is started.
		*/
		virtual void OnProcessingBegin(
					const TIHoughSpace<Dimensions, Element>& space,
					const Element& minValue) = 0;
		/**
			Called when processing is finished.
		*/
		virtual void OnProcessingEnd(const TIHoughSpace<Dimensions, Element>& space) = 0;
		/**
			Will be called when some local maximum is reached.
			\param	space	Hough space.
			\param	position		element position where maximum was found.
			\param	value			value of element at given position.
			\param	neghboursPtr	list of nighbours.
			\param	neghboursCount	number of neighbours in the list. Must be one of returned by \c GetSupportedNeghboursCount().
			\param	minValue		current minimal value, it can be correctd by this function to improve search performance.
			\return	true, if object found and no more itarations are needed.
		*/
		virtual bool OnMaximumFound(
					const TIHoughSpace<Dimensions, Element>& space,
					const istd::TIndex<Dimensions>& position,
					const Element& value,
					const Element* neghboursPtr,
					int neghboursCount,
					Element& minValue) = 0;
	};

	/**
		Create Hough space with specified size.
	*/
	virtual bool CreateHoughSpace(const istd::TIndex<Dimensions>& size, const Element& initValue = 0) = 0;

	/**
		Check if this space is wrapped horizontaly, it means the the left pixel is neighbour of the right one.
	*/
	virtual bool IsDimensionWrapped(int dimensionIndex) const = 0;
	/**
		Set if this space to be wrapped horizontaly or not.
		Space is horizonally wrapped if the left pixel is neighbour of the right one.
	*/
	virtual void SetDimensionWrapped(int dimensionIndex, bool state) = 0;

	/**
		Increase the value at specified position.
	*/
	virtual void IncreaseValueAt(const imath::TVector<Dimensions>& position, Element value) = 0;

	/**
		Smooth this space with specified stronness.
	*/
	virtual void SmoothHoughSpace(const istd::TIndex<Dimensions>& iterations) = 0;
	/**
		Analyse this Hough space to find set of local maximums.
		\param	minValue		minimal value of hough at point.
		\param	resultProcessor	will be called for each maximum.
	*/
	virtual bool AnalyseHoughSpace(
				const Element& minValue,
				ResultsConsumer& resultProcessor) = 0;

	/**
		Extract this Hough space to some gray scale bitmap.
	*/
	virtual bool ExtractToBitmap(iimg::IBitmap& bitmap) const = 0;

	/**
		Get position in space for some input.
		It consides the wrapping of space.
		\return	true, if this position is representable in the space.
	*/
	virtual bool GetSpacePosition(const imath::TVector<Dimensions>& position, imath::TVector<Dimensions>& result) const = 0;

	/**
		Get distance between two hough space positions considering the space wrapping.
	*/
	virtual double GetDistance(const imath::TVector<Dimensions>& position1, const imath::TVector<Dimensions>& position2) const = 0;

	/**
		Get square of distance between two hough space positions considering the space wrapping.
	*/
	virtual double GetDistance2(const imath::TVector<Dimensions>& position1, const imath::TVector<Dimensions>& position2) const = 0;
};


} // namespace ialgo


#endif // !ialgo_TIHoughSpace_included


