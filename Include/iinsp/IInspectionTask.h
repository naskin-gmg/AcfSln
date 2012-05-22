#ifndef iinsp_IInspectionTask_included
#define iinsp_IInspectionTask_included


// ACF includes
#include "iser/ISerializable.h"
#include "iprm/IParamsSet.h"
#include "iproc/ISupplier.h"


namespace iinsp
{


/**
	Representation of complete inspection task.
	Inspection contains of set of subtask suppliers (steps).
*/
class IInspectionTask: virtual public iproc::ISupplier
{
public:
	/**
		Get number of inspection steps (subtasks).
	*/
	virtual int GetSubtasksCount() const = 0;
	/**
		Get single inspection steps (subtasks).
	*/
	virtual iproc::ISupplier* GetSubtask(int subtaskIndex) const = 0;
};


} // namespace iinsp


#endif // !iinsp_IInspectionTask_included


