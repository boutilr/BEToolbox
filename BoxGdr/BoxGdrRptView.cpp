///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2019  Washington State Department of Transportation
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

// BoxGdrRptView.cpp : implementation file
//

#include "stdafx.h"
#include "BoxGdrDoc.h"
#include "BoxGdrRptView.h"
#include "BoxGdrChildFrame.h"

#include <EAF\EAFHints.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CBoxGdrRptView

IMPLEMENT_DYNCREATE(CBoxGdrRptView, CEAFReportView)

CBoxGdrRptView::CBoxGdrRptView()
{

}

CBoxGdrRptView::~CBoxGdrRptView()
{
}

BEGIN_MESSAGE_MAP(CBoxGdrRptView, CEAFReportView)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CBoxGdrRptView::OnFilePrint)
END_MESSAGE_MAP()

std::shared_ptr<CReportSpecification> CBoxGdrRptView::CreateReportSpecification()
{
   CBoxGdrDoc* pDoc = (CBoxGdrDoc*)GetDocument();

   std::shared_ptr<CReportSpecification> pRptSpec;
   std::vector<std::_tstring> rptNames = pDoc->m_RptMgr.GetReportNames();

   std::shared_ptr<CReportBuilder> pRptBuilder = pDoc->m_RptMgr.GetReportBuilder(rptNames.front());
   CReportDescription rptDesc = pRptBuilder->GetReportDescription();

   std::shared_ptr<CReportSpecificationBuilder> pRptSpecBuilder = pRptBuilder->GetReportSpecificationBuilder();
   pRptSpec = pRptSpecBuilder->CreateDefaultReportSpec(rptDesc);

   return pRptSpec;
}

std::shared_ptr<CReportBrowser> CBoxGdrRptView::CreateReportBrowser()
{
   if (m_pReportSpec == nullptr)
      return nullptr;

   CBoxGdrDoc* pDoc = (CBoxGdrDoc*)GetDocument();
   return pDoc->m_RptMgr.CreateReportBrowser(GetSafeHwnd(),m_pReportSpec,std::shared_ptr<CReportSpecificationBuilder>());
}

void CBoxGdrRptView::RefreshReport()
{
   if ( m_pReportSpec == nullptr )
      return;

   CBoxGdrDoc* pDoc = (CBoxGdrDoc*)GetDocument();


   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = &pDoc->m_RptMgr;
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   // refresh the report
   std::shared_ptr<CReportBuilder> pBuilder = pDoc->m_RptMgr.GetReportBuilder( m_pReportSpec->GetReportName() );
   std::shared_ptr<rptReport> pReport = pBuilder->CreateReport( m_pReportSpec );
   m_pReportBrowser->UpdateReport( pReport, true );
}

// CBoxGdrRptView diagnostics

#ifdef _DEBUG
void CBoxGdrRptView::AssertValid() const
{
	CEAFReportView::AssertValid();
}

#ifndef _WIN32_WCE
void CBoxGdrRptView::Dump(CDumpContext& dc) const
{
	CEAFReportView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBoxGdrRptView message handlers

void CBoxGdrRptView::OnInitialUpdate()
{
   CBoxGdrDoc* pDoc = (CBoxGdrDoc*)GetDocument();

   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = &pDoc->m_RptMgr;
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   m_pFrame = (CBoxGdrChildFrame*)GetParentFrame();
   m_pFrame->UpdateUnitsDisplayMode();
   m_pFrame->UpdateData(FALSE);

   CEAFReportView::OnInitialUpdate();
}

void CBoxGdrRptView::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   RefreshReport();
}
