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

// BEToolboxPlugin.cpp : Implementation of CBEToolboxPlugin

#include "stdafx.h"
#include "BEToolboxPlugin.h"
#include "BoxGdrDocTemplate.h"
#include "BoxGdrDoc.h"
#include "BoxGdrRptView.h"
#include "BoxGdrChildFrame.h"

#include "GenCompDocTemplate.h"
#include "GenCompDoc.h"
#include "GenCompRptView.h"
#include "GenCompChildFrame.h"

#include "GirCompDocTemplate.h"
#include "GirCompDoc.h"
#include "GirCompRptView.h"
#include "GirCompChildFrame.h"

#include "UltColDocTemplate.h"
#include "UltColDoc.h"
#include "UltColView.h"
#include "UltColChildFrame.h"

#include "CurvelDocTemplate.h"
#include "CurvelDoc.h"
#include "CurvelRptView.h"
#include "CurvelChildFrame.h"

#include <MFCTools\VersionInfo.h>


// CBEToolboxPlugin
BOOL CBEToolboxPlugin::Init(CEAFApp* pParent)
{
   return TRUE;
}

void CBEToolboxPlugin::Terminate()
{
}

void CBEToolboxPlugin::IntegrateWithUI(BOOL bIntegrate)
{
}

std::vector<CEAFDocTemplate*> CBEToolboxPlugin::CreateDocTemplates()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   std::vector<CEAFDocTemplate*> vDocTemplates;

   CBoxGdrDocTemplate* pBoxGdrDocTemplate;
	pBoxGdrDocTemplate = new CBoxGdrDocTemplate(
      IDR_BOXGDR,
      NULL,
		RUNTIME_CLASS(CBoxGdrDoc),
		RUNTIME_CLASS(CBoxGdrChildFrame),
		RUNTIME_CLASS(CBoxGdrRptView),
      NULL,1);

   vDocTemplates.push_back(pBoxGdrDocTemplate);


   CGenCompDocTemplate* pGenCompDocTemplate;
	pGenCompDocTemplate = new CGenCompDocTemplate(
      IDR_GENCOMP,
      NULL,
		RUNTIME_CLASS(CGenCompDoc),
		RUNTIME_CLASS(CGenCompChildFrame),
		RUNTIME_CLASS(CGenCompRptView),
      NULL,1);

   vDocTemplates.push_back(pGenCompDocTemplate);


   CGirCompDocTemplate* pGirCompDocTemplate;
	pGirCompDocTemplate = new CGirCompDocTemplate(
      IDR_GIRCOMP,
      NULL,
		RUNTIME_CLASS(CGirCompDoc),
		RUNTIME_CLASS(CGirCompChildFrame),
		RUNTIME_CLASS(CGirCompRptView),
      NULL,1);

   vDocTemplates.push_back(pGirCompDocTemplate);


   CUltColDocTemplate* pUltColDocTemplate;
	pUltColDocTemplate = new CUltColDocTemplate(
      IDR_ULTCOL,
      NULL,
		RUNTIME_CLASS(CUltColDoc),
		RUNTIME_CLASS(CUltColChildFrame),
		RUNTIME_CLASS(CUltColView),
      NULL,1);

   vDocTemplates.push_back(pUltColDocTemplate);


   CCurvelDocTemplate* pCurvelDocTemplate;
	pCurvelDocTemplate = new CCurvelDocTemplate(
      IDR_CURVEL,
      NULL,
		RUNTIME_CLASS(CCurvelDoc),
		RUNTIME_CLASS(CCurvelChildFrame),
		RUNTIME_CLASS(CCurvelRptView),
      NULL,1);

   vDocTemplates.push_back(pCurvelDocTemplate);

   return vDocTemplates;
}

HMENU CBEToolboxPlugin::GetSharedMenuHandle()
{
   return NULL;
}

CString CBEToolboxPlugin::GetName()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   CVersionInfo verInfo;
   CString strAppName = AfxGetAppName();
   strAppName += _T(".dll");
   verInfo.Load(strAppName);

   CString strVersion = verInfo.GetProductVersionAsString();

   CString str;
   str.Format(_T("BEToolbox Version %s"),strVersion);
   return str;
}