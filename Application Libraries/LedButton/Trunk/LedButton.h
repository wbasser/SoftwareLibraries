// ==========================================================================
//!
//! \file LedButton.h
//!
//! \brief CLedButton and CLedStateCondition Classes.
//!
//! \author 
//!    Bill Basser
// ==========================================================================
#ifndef _LEDBUTTON__H_ // _LEDBUTTON__H_
#define _LEDBUTTON__H_ // _LEDBUTTON__H_

// //////////////////////////////////////////////////////////////////////////
// Include Files
// //////////////////////////////////////////////////////////////////////////
#include <afxtempl.h>

// //////////////////////////////////////////////////////////////////////////
// Type Definitions
// //////////////////////////////////////////////////////////////////////////
typedef unsigned int LedState;

// //////////////////////////////////////////////////////////////////////////
// Name Spaces
// //////////////////////////////////////////////////////////////////////////
namespace
{
    /** Predefined initial Led State as "Off" state. */
    const LedState LED_BUTTON_STATE_OFF = 0; 

    /** Predefined "On" Led State. */
    const LedState LED_BUTTON_STATE_ON = 1;  

    /** Predefined activity duration in msec. */
    const int LED_BUTTON_DEFAULT_ACTIVITY_DURATION = 150;  

    /** Predefined Led States Number. */
    const int LED_BUTTON_PREDEFINED_STATES_NUMBER = 2;
                                                          
};

//
// To avoid a warning for 'C4100: unreferenced formal parameter' on Level4 Warnings,
// on the CLedStateCondition::ChangeState method. I'm adding the following
// preprocessor warning disable command. The warning is generated due
// that 'oldLedState' and 'isForcedChange' are parameters that are not used in
// the inline implementation of this method. Derived classes from CLedStateCondition
// may use this parameters for its proper behavior.
//
#pragma warning( push )
#pragma warning( disable : 4100 )

// LED state condition class
class CLedStateCondition
{
public:
    // construction/destruction
    CLedStateCondition(void) {};
    virtual ~CLedStateCondition(void) {};

	// implamentation
	virtual LedState ChangeState(LedState newLedState, 
                                 LedState oldLedState, 
                                 bool isForcedChange=false) { return(newLedState); };
};
#pragma warning( pop )

// Led button class
class CLedButton : public CButton
{
	DECLARE_DYNAMIC(CLedButton)
// construction/destruction
public:
	CLedButton();
	virtual ~CLedButton();

// attributes
public:
    void SetLedStatesNumber(int maxLedStatesNumber, bool isInvalidate=true);
    void SetLedState(LedState ledState, bool isForcedChange=false);
    LedState GetLedState(void) { return (m_ledState); };
    void SetLedActivityTimer(UINT timerId, 
                             int duration=LED_BUTTON_DEFAULT_ACTIVITY_DURATION, 
                             LedState offState=LED_BUTTON_STATE_OFF);
    void SetLedStateCondition(CLedStateCondition *pCondition); 

// operations
public:
    bool SetIcon(LedState ledState, UINT iconId, int width=0, int height=0);
    bool SetIcon(LedState ledState, HICON hIcon);
    bool SetIcons(UINT offIconId, UINT onIconId);
    void RemoveIcon(LedState ledState);
    void RestoreDefaultColors(bool isInvalidate=true);
	void SetTextForeground(LedState ledState, COLORREF colorRef, bool isInvalidate=true);
    void SetTextBackground(LedState ledState, COLORREF colorRef, bool isInvalidate=true);
    void SetTextColors(LedState ledState, COLORREF fgColorRef, COLORREF bgColorRef, bool isInvalidate=true);
	COLORREF GetTextForegroundColor(LedState ledState);
    COLORREF GetTextBackgroundColor(LedState ledState);
    void SetTooltipText(UINT id, bool isActivate=true);
	void SetTooltipText(LPCTSTR text, bool isActivate=true);
	void ActivateTooltip(bool isActivate=true);

// implementation
// overrides
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
    virtual void PreSubclassWindow();
   	virtual void OnDrawBackground(CDC* pDC, CRect* pRect);

// message handler
	DECLARE_MESSAGE_MAP()
    afx_msg void OnDestroy();
    afx_msg void OnSysColorChange();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

private:
    typedef struct LedData
	{
        COLORREF    textForegroundColor;   /*!< The text foreground to use */
        COLORREF    textBackgroundColor;   /*!< The text background to use */
		HICON		hIcon;                 /*!< The Icon handler. */
		DWORD		width;                 /*!< The Icon width */
		DWORD		height;                /*!< The Icon height */
    } LedData;
   	void ToolTipInit(void);
    LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
    CArray<LedData , LedData&> m_ledDataList;  ///< List of Led States this control.
	CToolTipCtrl m_toolTip;                    ///< Tooltip control associated with the control.
    LedState m_ledState;                       ///< Current LedState the control is in it.
    UINT     m_listCount;                      ///< Number of Led States in this control.
    UINT     m_align;                          ///< Text/Icon align attribute.
    bool     m_isDisabled;                     ///< is this control disabled?
    LONG     m_buttonStyle;                    ///< the button style.
    UINT_PTR  m_activityTimer;                 ///< The internal activation timer if relevant.
    int       m_activityDuration;              ///< The activation timer duration if relevant.
    UINT      m_activityId;                    ///< The activation timer identifier if relevant.
    LedState  m_activityState;                 ///< The activation "off" state to return.
    CLedStateCondition *m_pCondition;          ///< the condition led to use if relevant.
};



#endif //_LEDBUTTON__H_

//
// --- No-code-allowed-beyond-this-line--------------------------------------
//

