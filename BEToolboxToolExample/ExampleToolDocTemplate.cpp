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

#include "stdafx.h"
#include "resource.h"
#include "ExampleToolDocTemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CExampleToolDocTemplate,CEAFDocTemplate)

CExampleToolDocTemplate::CExampleToolDocTemplate(UINT nIDResource,
                                       IEAFCommandCallback* pCallback,
                                       CRuntimeClass* pDocClass,
                                       CRuntimeClass* pFrameClass,
                                       CRuntimeClass* pViewClass,
                                       HMENU hSharedMenu,
                                       int maxViewCount)
: CEAFDocTemplate(nIDResource,pCallback,pDocClass,pFrameClass,pViewClass,hSharedMenu,maxViewCount)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CString strDocName;
   GetDocString(strDocName,CDocTemplate::docName);

   HICON hIcon = AfxGetApp()->LoadIcon(IDR_EXAMPLE);
   m_TemplateGroup.AddItem( new CEAFTemplateItem(this,strDocName,nullptr,hIcon) );
   m_TemplateGroup.SetIcon(hIcon);
}

CString CExampleToolDocTemplate::GetTemplateGroupItemDescription(const CEAFTemplateItem* pItem) const
{
   CString strDescription(_T("Create a new Example Tool project. "));
   return strDescription;
}
