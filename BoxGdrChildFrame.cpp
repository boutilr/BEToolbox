///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2013  Washington State Department of Transportation
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

#include "StdAfx.h"
#include "resource.h"
#include "BoxGdrChildFrame.h"
#include "BoxGdrDoc.h"

IMPLEMENT_DYNCREATE(CBoxGdrChildFrame, CEAFChildFrame)

CBoxGdrChildFrame::CBoxGdrChildFrame(void)
{
}

CBoxGdrChildFrame::~CBoxGdrChildFrame(void)
{
}

BEGIN_MESSAGE_MAP(CBoxGdrChildFrame, CEAFChildFrame)
   ON_WM_CREATE()
   ON_BN_CLICKED(IDC_UPDATE,OnUpdate)
   ON_BN_CLICKED(IDC_US,OnUSUnits)
   ON_BN_CLICKED(IDC_SI,OnSIUnits)
   ON_BN_CLICKED(IDC_ADD,OnAdd)
   ON_BN_CLICKED(IDC_REMOVE,OnRemove)
   ON_UPDATE_COMMAND_UI(IDC_REMOVE,OnUpdateRemove)
END_MESSAGE_MAP()

BOOL CBoxGdrChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // force this window to be maximized (not sure why WS_VISIBLE is required)
   cs.style |= WS_MAXIMIZE | WS_VISIBLE;

   return CEAFChildFrame::PreCreateWindow(cs);
}

int CBoxGdrChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEAFChildFrame::OnCreate(lpCreateStruct) == -1)
      return -1;

   {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if (!m_DlgBar.Create(this,IDD_BOXGDR,CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, AFX_IDW_CONTROLBAR_FIRST))
   {
      TRACE0("Failed to create dialog bar\n");
      return -1;
   }

   EnableToolTips();
   m_DlgBar.EnableToolTips();

   SetIcon(AfxGetApp()->LoadIcon(IDR_BOXGDR),TRUE);
   }

   return 0;
}

void CBoxGdrChildFrame::OnUpdate()
{
   m_DlgBar.UpdateData(TRUE);

   CBoxGdrDoc* pDoc = (CBoxGdrDoc*)GetActiveDocument();
   if ( pDoc )
   {
      pDoc->SetModifiedFlag();
      pDoc->UpdateAllViews(NULL);
   }
}

void CBoxGdrChildFrame::OnUSUnits()
{
   ASSERT(m_DlgBar.IsDlgButtonChecked(IDC_US) == 1 );
   SetUnitsMode(eafTypes::umUS);
}

void CBoxGdrChildFrame::OnSIUnits()
{
   ASSERT(m_DlgBar.IsDlgButtonChecked(IDC_SI) == 1 );
   SetUnitsMode(eafTypes::umSI);
}

void CBoxGdrChildFrame::OnAdd()
{
   m_DlgBar.AddProblem();
}

void CBoxGdrChildFrame::OnRemove()
{
   m_DlgBar.RemoveSelectedProblems();
}

void CBoxGdrChildFrame::OnUpdateRemove(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_DlgBar.AreProblemsSelected() ? TRUE : FALSE);
}

void CBoxGdrChildFrame::SetUnitsMode(eafTypes::UnitMode um)
{
   CEAFApp* pApp = EAFGetApp();
   if ( pApp->GetUnitsMode() != um )
   {
      UpdateData(TRUE); // update the data
      // before changing the units mode
      pApp->SetUnitsMode(um);
      UpdateUnitsDisplayMode();

      CBoxGdrDoc* pDoc = (CBoxGdrDoc*)GetActiveDocument();
      if ( pDoc )
      {
         pDoc->SetModifiedFlag();
      }
   }
}

void CBoxGdrChildFrame::UpdateUnitsDisplayMode()
{
   m_DlgBar.OnUnitsModeChanged();
}

#if defined _DEBUG
void CBoxGdrChildFrame::AssertValid() const
{
   CEAFChildFrame::AssertValid();
}
#endif

BOOL CBoxGdrChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
   // CEAFChildFrame::LoadFrame is going to load resources based on nIDResource
   // We have to set the module context to this module so the load happens
   // correctly
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if ( !CEAFChildFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext) )
      return FALSE;

   return TRUE;
}

void CBoxGdrChildFrame::UpdateData(BOOL bUpdate)
{
   m_DlgBar.UpdateData(bUpdate);
}