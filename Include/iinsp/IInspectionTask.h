#ifndef iinsp_IInspectionTask_included
#define iinsp_IInspectionTask_included


// ACF includes
#include <iser/ISerializable.h>
#include <iprm/IParamsSet.h>
#include <iinsp/ISupplier.h>


namespace iinsp
{


/**
	Representation of complete inspection task.
	Inspection contains of set of subtask suppliers (steps).
*/
class IInspectionTask: virtual public iser::ISerializable
{
public:
	/**
		Get number of inspection steps (subtasks).
	*/
	virtual int GetSubtasksCount() const = 0;

	/**
		Get single inspection steps (subtasks).
	*/
	virtual iinsp::ISupplier* GetSubtask(int subtaskIndex) const = 0;

	/**
		Get general parameters additionaly stored next to parameters stored in suppliers.
	*/
	virtual iprm::IParamsSet* GetGeneralParameters() const = 0;
};


} // namespace iinsp


#endif // !iinsp_IInspectionTask_included


