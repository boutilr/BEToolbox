///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2016  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

#pragma once

#include <set>
#include "afxcmn.h"
#include "PGStableStrands.h"
#include "PGStableFormView.h"

// CPGStableLiftingView form view

class CPGStableLiftingView : public CPGStableFormView
{
	DECLARE_DYNCREATE(CPGStableLiftingView)

protected:
	CPGStableLiftingView();           // protected constructor used by dynamic creation
	virtual ~CPGStableLiftingView();

	CEdit	   m_ctrlEc;
   CEdit    m_ctrlFc;
   CEdit    m_ctrlK1;
   CEdit    m_ctrlK2;
   CString m_strUserEc;
   void UpdateEc();

   void GetMaxFpe(Float64* pFpeStraight,Float64* pFpeHarped,Float64* pFpeTemp);

   CPGStableStrands m_Strands;
   CPGStableLiftingCriteria m_LiftingCriteria;


public:
	enum { IDD = IDD_PGSTABLELIFTINGVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

   virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,CCreateContext* pContext);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
   virtual void OnActivateView();
   virtual void OnDeactivateView();

   boost::shared_ptr<CReportSpecification> m_pRptSpec;
   boost::shared_ptr<CReportBrowser> m_pBrowser; // this is the actual browser window that displays the report
   void RefreshReport();

   void UpdateFpeControls();
   void UpdateCriteriaControls();

public:
   afx_msg void OnUserEc();
   afx_msg void OnChangeFc();
   afx_msg void OnChange();
   afx_msg void OnEditFpe();
   afx_msg void OnPrint();
   afx_msg void OnPrintDirect();
   afx_msg void OnClickedLiftingTensionMax();
   afx_msg LRESULT OnCommandHelp(WPARAM, LPARAM lParam);
   afx_msg void OnCmenuSelected(UINT id);
   virtual void OnInitialUpdate();
protected:
   virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnWindTypeChanged();
   afx_msg void OnCbnSelchangeStresses();
};

