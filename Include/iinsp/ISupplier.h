#ifndef iinsp_ISupplier_included
#define iinsp_ISupplier_included


// ACF includes
#include <istd/IChangeable.h>
#include <imod/IModel.h>
#include <ilog/IMessageContainer.h>
#include <iprm/IParamsSet.h>


namespace iinsp
{


/**
	Base interface for suppliers providing calculating of objects on demand <em>("pull data" model)</em>.
	Additional interfaces (providers) should provide access to intern stored calculated output objects.
	For example image provider should provide method \c{const iimg::IImage* GetImage() const}, where the image will be calculated on demand.
	\image html SupplierStates.svg
*/
class ISupplier: virtual public istd::IChangeable
{
public:
	enum WorkStatus
	{
		/**
			Supplier is invalidated, no result is produced.
		*/
		WS_INVALID,
		/**
			New work is initialized and will produce new result on demand.
		*/
		WS_INIT,
		/**
			Supplier is locked becouse it is doing processing step.
		*/
		WS_LOCKED,
		/**
			Work was done correctly and no error occure.
		*/
		WS_OK,
		/**
			Work was canceled.
		*/
		WS_CANCELED,
		/**
			Work couldn't be done.
		*/
		WS_FAILED
	};

	/**
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		/**
			Supplier result changed.
		*/
		CF_SUPPLIER_RESULTS = 0x6326415
	};

	enum MessageContainerType
	{
		MCT_RESULTS,
		MCT_TEMP,
		MTC_LAST = MCT_TEMP
	};

	/**
		Get status of last work.
		Please note, that this status is not a part of supplier model and can be changed without any notification. If you want to observe the status model of the supplier, please use the GetWorkStatusModel method.
		\return	work status defined in iinsp::ISupplier::WorkStatus.
		\sa GetWorkStatusModel
	*/
	virtual int GetWorkStatus() const = 0;

	/**
		Get access to the status model of the supplier.
	*/
	virtual imod::IModel* GetWorkStatusModel() const = 0;

	/**
		Called to signalize that this supplier is invalid.
		This signal will be transfered to all supplier which are registered as output.
		\return	true, if initialization could be done.
	*/
	virtual void InvalidateSupplier() = 0;

	/**
		Force the supplier to initialize its work.
		First if work is initialized, new calculation can be done.
	*/
	virtual void EnsureWorkInitialized() = 0;

	/**
		Ensure that all objects are produced.
	*/
	virtual void EnsureWorkFinished() = 0;

	/**
		Remove all stored work results.
		This set also work state to \c WS_INIT.
	*/
	virtual void ClearWorkResults() = 0;

	/**
		Get messages outputted by this work.
		The pointed object will not be relocated - no reconnect is needed if it is observed.
		If the supplier doesn't support list of messages, it returns NULL.
		\param	containerType	type of message container, \sa MessageContainerType.
	*/
	virtual const ilog::IMessageContainer* GetWorkMessages(int containerType) const = 0;

	/**
		Get parameter set using by this supplier.
		The pointed object will not be relocated - no reconnect is needed if it is observed.
		If the supplier doesn't support parameters, it returns NULL.
	*/
	virtual iprm::IParamsSet* GetModelParametersSet() const = 0;
};


} // namespace iinsp


#endif // !iinsp_ISupplier_included


