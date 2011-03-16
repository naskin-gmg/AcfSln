#ifndef iqwt_CProgressHistoryGuiComp_included
#define iqwt_CProgressHistoryGuiComp_included


// Qwt includes
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_scale_draw.h"
#include "qwt_plot_marker.h"

// ACF includes
#include "iproc/IProgressManager.h"
#include "iqtgui/TDesignerGuiCompBase.h"

#include "Generated/ui_CProgressHistoryGuiComp.h"


namespace iqwt
{



/**
	Shows progress history for nonlinear and 'non-progressive' processes.
*/
class CProgressHistoryGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CProgressHistoryGuiComp>,
			public iproc::IProgressManager
{
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CProgressHistoryGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CProgressHistoryGuiComp);
		I_REGISTER_INTERFACE(iproc::IProgressManager);
		I_ASSIGN(m_historyStepsCountAttrPtr, "HistoryStepsCount", "Number of steps shown in this history", true, 100);
		I_ASSIGN(m_showTaskDescriptionAttrPtr, "ShowTaskDescription", "If true, description of current task will be shown", true, true);
		I_ASSIGN(m_showCancelAttrPtr, "ShowCancel", "If true, cancel button will be shown", true, true);
		I_ASSIGN_MULTI_0(m_progressIdsAttrPtr, "ProgressIds", "List of known progress ID's", true);
		I_ASSIGN_MULTI_0(m_progressColorsAttrPtr, "ProgressColors", "Colors for known progress ID's,\nFormat: #RRGGBB or SVG color keyword names", true);
	I_END_COMPONENT;

	CProgressHistoryGuiComp();

	// reimplemented (iproc::IProgressManager)
	virtual int BeginProgressSession(
				const std::string& progressId,
				const istd::CString& description,
				bool isCancelable = false);
	virtual void EndProgressSession(int sessionId);
	virtual void OnProgress(int sessionId, double currentProgress);
	virtual bool IsCanceled(int sessionId) const;

protected:
	void UpdateState();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(int, m_historyStepsCountAttrPtr);
	I_ATTR(bool, m_showTaskDescriptionAttrPtr);
	I_ATTR(bool, m_showCancelAttrPtr);
	I_MULTIATTR(istd::CString, m_progressIdsAttrPtr);
	I_MULTIATTR(istd::CString, m_progressColorsAttrPtr);

	istd::TDelPtr<QwtPlot> m_plotPtr;
	istd::TDelPtr<QwtPlotMarker> m_nowMarker;

	typedef std::vector<double> SingleData;

	struct Session
	{
		SingleData axisY;
		QwtPlotCurve curve;
		bool isCancelable;
		istd::CString description;
	};

	typedef istd::TDelPtr<Session> SessionPtr;

	typedef std::map<int, SessionPtr> IdToSessionMap;
	IdToSessionMap m_idToSessionMap;

	SingleData m_axisXData;

	int m_currentId;
	int m_cancelsCount;
};


} // namespace iqwt


#endif // !iqwt_CProgressHistoryGuiComp_included


