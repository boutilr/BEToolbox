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

// PrismaticGirder.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PGStableDoc.h"
#include "PGStablePrismaticGirder.h"
#include "PGStableDDX.h"
#include <MFCTools\MFCTools.h>

#include <IFace\BeamFactory.h>

void DDX_Girder(CDataExchange* pDX,stbGirder& girder)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   ATLASSERT(girder.GetSectionCount() == 1); // prismatic girders have only one section
   IndexType sectIdx = 0;
   Float64 Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
   girder.GetSectionProperties(sectIdx,stbTypes::Start,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);

   Float64 L = girder.GetSectionLength(sectIdx);

   Yt *= -1; //this is in girder section coordinates, convert to Y positive down coordinates

   DDX_UnitValueAndTag(pDX,IDC_HG,IDC_HG_UNIT, Hg, pDispUnits->ComponentDim);
   DDV_UnitValueGreaterThanZero(pDX,IDC_HG,Hg,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_WTF,IDC_WTF_UNIT, Wtf, pDispUnits->ComponentDim);
   DDV_UnitValueGreaterThanZero(pDX,IDC_WTF, Wtf, pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_WBF,IDC_WBF_UNIT, Wbf, pDispUnits->ComponentDim);
   DDV_UnitValueGreaterThanZero(pDX,IDC_WBF, Wbf, pDispUnits->ComponentDim);
   
   DDX_UnitValueAndTag(pDX,IDC_AREA,IDC_AREA_UNIT,Ag,pDispUnits->Area);
   DDV_UnitValueGreaterThanZero(pDX,IDC_AREA,Ag,pDispUnits->Area);

   DDX_UnitValueAndTag(pDX,IDC_IX,IDC_IX_UNIT,Ix,pDispUnits->MomentOfInertia);
   DDV_UnitValueGreaterThanZero(pDX,IDC_IX,Ix,pDispUnits->MomentOfInertia);

   DDX_UnitValueAndTag(pDX,IDC_IY,IDC_IY_UNIT,Iy,pDispUnits->MomentOfInertia);
   DDV_UnitValueGreaterThanZero(pDX,IDC_IY,Iy,pDispUnits->MomentOfInertia);

   DDX_UnitValueAndTag(pDX,IDC_YT,IDC_YT_UNIT,Yt,pDispUnits->ComponentDim);
   DDV_UnitValueGreaterThanZero(pDX,IDC_YT,Yt,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_L,IDC_L_UNIT,L,pDispUnits->SpanLength);
   DDV_UnitValueGreaterThanZero(pDX,IDC_L,L,pDispUnits->SpanLength);


   if ( pDX->m_bSaveAndValidate )
   {
      Yt *= -1;
      girder.SetSectionProperties(sectIdx,L,Ag,Ix,Iy,Yt,Hg,Wtf,Wbf);
   }
}

void DDX_PrismaticGirderStrands(CDataExchange* pDX,CPGStableStrands& strands)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   DDX_CBEnum(pDX,IDC_PS_METHOD,strands.strandMethod);

   DDX_UnitValueAndTag(pDX,IDC_XFER_LENGTH,IDC_XFER_LENGTH_UNIT,strands.XferLength,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_XFER_LENGTH,strands.XferLength,pDispUnits->ComponentDim);

   strands.YsMeasure = BOTTOM;
   strands.YtMeasure = TOP;

   strands.Xh1 = 0;
   strands.Xh1Measure = FRACTION;
   strands.Yh1Measure = TOP;

   strands.Xh2Measure = FRACTION;
   strands.Yh2Measure = BOTTOM;

   DDX_UnitValueAndTag(pDX,IDC_YS,IDC_YS_UNIT,strands.Ys,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YS,strands.Ys,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_YTEMP,IDC_YTEMP_UNIT,strands.Yt,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YTEMP,strands.Yt,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_YH1,IDC_YH1_UNIT,strands.Yh1,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YH1,strands.Yh1,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_YH2,IDC_YH2_UNIT,strands.Yh2,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YH2,strands.Yh2,pDispUnits->ComponentDim);

   DDX_Text(pDX,IDC_HP,strands.Xh2);
   DDV_MinMaxDouble(pDX,strands.Xh2,0.0,0.5);

   if ( pDX->m_bSaveAndValidate )
   {
      strands.Xh3 = 1.0 - strands.Xh2;
      strands.Xh3Measure = FRACTION;
      strands.Yh3 = strands.Yh2;
      strands.Yh3Measure = strands.Yh2Measure;

      strands.Xh4 = 1.0;
      strands.Xh4Measure = FRACTION;
      strands.Yh4 = strands.Yh1;
      strands.Yh4Measure = strands.Yh1Measure;
   }
}

// CPGStablePrismaticGirder dialog

IMPLEMENT_DYNAMIC(CPGStablePrismaticGirder, CDialog)

CPGStablePrismaticGirder::CPGStablePrismaticGirder(CWnd* pParent /*=NULL*/)
	: CDialog(CPGStablePrismaticGirder::IDD, pParent)
{
   m_pPointLoadGrid = NULL;
}

CPGStablePrismaticGirder::~CPGStablePrismaticGirder()
{
   delete m_pPointLoadGrid;
}

void CPGStablePrismaticGirder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      DDX_MetaFileStatic(pDX, IDC_STRAND_LAYOUT, m_StrandLayout, AfxGetResourceHandle(), _T("PrismaticStrandLayout"), _T("Metafile") );
   }
   DDX_Control(pDX,IDC_GIRDER,m_ctrlGirder);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   CString strGirder = pDoc->GetGirder();
   DDX_CBString(pDX,IDC_GIRDER_LIST,strGirder);

   stbGirder girder = pDoc->GetGirder(PRISMATIC);
   CPGStableStrands strands = pDoc->GetStrands(PRISMATIC,LIFTING);


   Float64 Cd = girder.GetDragCoefficient();
   DDX_Text(pDX,IDC_DRAG_COEFFICIENT,Cd);
   DDV_GreaterThanZero(pDX,IDC_DRAG_COEFFICIENT,Cd);

   DDX_Girder(pDX,girder);
   DDX_PrismaticGirderStrands(pDX,strands);

   Float64 densityWithRebar = pDoc->GetDensityWithRebar();
   Float64 density = pDoc->GetDensity();

   DDX_UnitValueAndTag(pDX,IDC_DENSITY,IDC_DENSITY_UNIT,density,pDispUnits->Density);
   DDV_UnitValueGreaterThanZero(pDX,IDC_DENSITY,density,pDispUnits->Density);
   DDX_UnitValueAndTag(pDX,IDC_DENSITY_WITH_REBAR,IDC_DENSITY_WITH_REBAR_UNIT,densityWithRebar,pDispUnits->Density);
   DDV_UnitValueGreaterThanZero(pDX,IDC_DENSITY_WITH_REBAR,densityWithRebar,pDispUnits->Density);

   std::vector<std::pair<Float64,Float64>> vLoads = girder.GetAdditionalLoads();
   DDX_PointLoadGrid(pDX,m_pPointLoadGrid,vLoads);


   if ( pDX->m_bSaveAndValidate )
   {
      girder.SetAdditionalLoads(vLoads);

      girder.SetDragCoefficient(Cd);

      pDoc->SetGirder(PRISMATIC,girder);
      pDoc->SetStrands(PRISMATIC,LIFTING,strands);

      CPGStableStrands haulingStrands = pDoc->GetStrands(PRISMATIC,HAULING);
      haulingStrands.strandMethod = strands.strandMethod;
      if ( strands.strandMethod == CPGStableStrands::Simplified )
      {
         // we were operating on the lifting prestress information
         // since we are using the simplified method the geometry
         // of the hauling prestress is the same as the lifting prestress
         // update the hauling prestress information here
         haulingStrands.XferLength = strands.XferLength;
         haulingStrands.Xh1 = strands.Xh1;
         haulingStrands.Xh1Measure = strands.Xh1Measure;
         haulingStrands.Xh2 = strands.Xh2;
         haulingStrands.Xh2Measure = strands.Xh2Measure;
         haulingStrands.Xh3 = strands.Xh3;
         haulingStrands.Xh3Measure = strands.Xh3Measure;
         haulingStrands.Xh4 = strands.Xh4;
         haulingStrands.Xh4Measure = strands.Xh4Measure;
         haulingStrands.Ys = strands.Ys;
         haulingStrands.YsMeasure = strands.YsMeasure;
         haulingStrands.Yt = strands.Yt;
         haulingStrands.YtMeasure = strands.YtMeasure;
      }
      pDoc->SetStrands(PRISMATIC,HAULING,haulingStrands);

      pDoc->SetDensity(density);
      pDoc->SetDensityWithRebar(densityWithRebar);

      pDoc->SetGirder(strGirder);
   }

   if ( !pDX->m_bSaveAndValidate )
   {
      m_ctrlGirder.Invalidate();
      m_ctrlGirder.UpdateWindow();
   }
}


BEGIN_MESSAGE_MAP(CPGStablePrismaticGirder, CDialog)
   ON_BN_CLICKED(IDC_ADD, &CPGStablePrismaticGirder::OnAddPointLoad)
   ON_BN_CLICKED(IDC_REMOVE, &CPGStablePrismaticGirder::OnRemovePointLoad)
   ON_UPDATE_COMMAND_UI(IDC_REMOVE,&CPGStablePrismaticGirder::OnUpdateRemove)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
   ON_CBN_SELCHANGE(IDC_PS_METHOD, &CPGStablePrismaticGirder::OnPSMethodChanged)
   ON_CBN_SELCHANGE(IDC_GIRDER_LIST, &CPGStablePrismaticGirder::OnGirderChanged)
END_MESSAGE_MAP()


LRESULT CPGStablePrismaticGirder::OnCommandHelp(WPARAM, LPARAM lParam)
{
   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_PGSTABLE_PRISMATIC_GIRDER_VIEW );
   return TRUE;
}

BOOL CPGStablePrismaticGirder::OnInitDialog()
{
   {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   m_pPointLoadGrid = new CPGStablePointLoadGrid;
	m_pPointLoadGrid->SubclassDlgItem(IDC_POINT_LOAD_GRID, this);
   m_pPointLoadGrid->CustomInit();

   m_ctrlGirder.SubclassDlgItem(IDC_GIRDER,this);
   m_ctrlGirder.CustomInit();
   }

   CComboBox* pcbMethod = (CComboBox*)GetDlgItem(IDC_PS_METHOD);
   pcbMethod->SetItemData(pcbMethod->AddString(_T("Simplified")),(DWORD_PTR)CPGStableStrands::Simplified);
   pcbMethod->SetItemData(pcbMethod->AddString(_T("Detailed")),(DWORD_PTR)CPGStableStrands::Detailed);

   CPGStableDoc* pDoc = (CPGStableDoc*)EAFGetDocument();
   CComboBox* pcbGirders = (CComboBox*)GetDlgItem(IDC_GIRDER_LIST);
   pcbGirders->AddString(gs_strGirder);
   const GirderLibrary* pLib = pDoc->GetGirderLibrary();
   libKeyListType keyList;
   pLib->KeyList(keyList);
   BOOST_FOREACH(const std::_tstring& key,keyList)
   {
      // fill combo box with only the names of prismatic girders
      const GirderLibraryEntry* pGirderEntry = (const GirderLibraryEntry*)(pLib->GetEntry(key.c_str()));
      const GirderLibraryEntry::Dimensions& dimensions = pGirderEntry->GetDimensions();
      CComPtr<IBeamFactory> factory;
      pGirderEntry->GetBeamFactory(&factory);

      CComQIPtr<ISplicedBeamFactory> splicedFactory(factory); // using only PGSuper prismatic beams... want splicedFactory to be NULL
      if ( splicedFactory == NULL && factory->IsPrismatic(dimensions) )
      {
         pcbGirders->AddString(key.c_str());
      }
   }

   CDialog::OnInitDialog();

   OnPSMethodChanged();
   OnGirderChanged();

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

// CPGStablePrismaticGirder message handlers

void CPGStablePrismaticGirder::OnAddPointLoad()
{
   // TODO: Add your control notification handler code here
   m_pPointLoadGrid->AddLoad();
}

void CPGStablePrismaticGirder::OnRemovePointLoad()
{
   m_pPointLoadGrid->RemoveSelectedLoads();
}

void CPGStablePrismaticGirder::OnUpdateRemove(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(AreLoadsSelected() ? TRUE : FALSE);
}

BOOL CPGStablePrismaticGirder::AreLoadsSelected()
{
   if ( m_pPointLoadGrid )
   {
      return m_pPointLoadGrid->AreLoadsSelected();
   }
   else
   {
      return FALSE;
   }
}

void CPGStablePrismaticGirder::OnPSMethodChanged()
{
   // TODO: Add your control notification handler code here
   CComboBox* pcbMethod = (CComboBox*)GetDlgItem(IDC_PS_METHOD);
   int curSel = pcbMethod->GetCurSel();
   CPGStableStrands::StrandMethod method = (CPGStableStrands::StrandMethod)(pcbMethod->GetItemData(curSel));

   CString strFpeNote;
   if ( method == CPGStableStrands::Simplified )
   {
      strFpeNote = _T("NOTE: Effective prestress force (Fpe) is defined on the Lifting and Hauling tabs");
   }
   else
   {
      strFpeNote = _T("NOTE: Magnitude and location of the effective prestress force (Fpe) is defined on the Lifting and Hauling tabs");
   }
   GetDlgItem(IDC_FPE_NOTE)->SetWindowText(strFpeNote);

   int show = (method == CPGStableStrands::Simplified ? SW_SHOW : SW_HIDE);

   GetDlgItem(IDC_STRAND_LAYOUT)->ShowWindow(show);
   GetDlgItem(IDC_GIRDER)->ShowWindow(method == CPGStableStrands::Simplified ? SW_HIDE : SW_SHOW);

   GetDlgItem(IDC_XFER_LENGTH_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_XFER_LENGTH)->ShowWindow(show);
   GetDlgItem(IDC_XFER_LENGTH_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_STRAND_LAYOUT)->ShowWindow(show);

   GetDlgItem(IDC_YS_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YS)->ShowWindow(show);
   GetDlgItem(IDC_YS_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_YH1_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YH1)->ShowWindow(show);
   GetDlgItem(IDC_YH1_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_YH2_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YH2)->ShowWindow(show);
   GetDlgItem(IDC_YH2_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_HP_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_HP)->ShowWindow(show);
   GetDlgItem(IDC_HP_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_YTEMP_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YTEMP)->ShowWindow(show);
   GetDlgItem(IDC_YTEMP_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_SS_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_HS_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_TS_LABEL)->ShowWindow(show);
}

std::vector<std::pair<Float64,Float64>> CPGStablePrismaticGirder::GetGirderProfile()
{
   std::vector<std::pair<Float64,Float64>> vProfile;

   CDataExchange dx(this,TRUE);

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   stbGirder girder = pDoc->GetGirder(PRISMATIC);
   DDX_Girder(&dx,girder);

   IndexType sectIdx = 0;
   Float64 Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
   girder.GetSectionProperties(sectIdx,stbTypes::Start,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);

   Float64 Lg = girder.GetGirderLength();
   vProfile.push_back(std::make_pair(0,0));
   vProfile.push_back(std::make_pair(Lg,0));
   vProfile.push_back(std::make_pair(Lg,-Hg));
   vProfile.push_back(std::make_pair(0,-Hg));
   return vProfile;
}

void CPGStablePrismaticGirder::GetStrandProfiles(std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp)
{
   std::vector<std::pair<Float64,Float64>> vProfile;

   CDataExchange dx(this,TRUE);

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   stbGirder girder = pDoc->GetGirder(PRISMATIC);
   DDX_Girder(&dx,girder);

   CPGStableStrands strands = pDoc->GetStrands(NONPRISMATIC,LIFTING);
   DDX_PrismaticGirderStrands(&dx,strands);

   pDoc->GetStrandProfiles(strands,girder,pvStraight,pvHarped,pvTemp);
}

void CPGStablePrismaticGirder::OnGirderChanged()
{
   CString strGirder;
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_GIRDER_LIST);
   int curSel = pCB->GetCurSel();
   pCB->GetLBText(curSel,strGirder);

   BOOL bEnable = strGirder == gs_strGirder ? TRUE : FALSE;
   GetDlgItem(IDC_HG)->EnableWindow(bEnable);
   GetDlgItem(IDC_WTF)->EnableWindow(bEnable);
   GetDlgItem(IDC_WBF)->EnableWindow(bEnable);
   GetDlgItem(IDC_AREA)->EnableWindow(bEnable);
   GetDlgItem(IDC_IX)->EnableWindow(bEnable);
   GetDlgItem(IDC_IY)->EnableWindow(bEnable);
   GetDlgItem(IDC_YT)->EnableWindow(bEnable);
   GetDlgItem(IDC_DRAG_COEFFICIENT)->EnableWindow(bEnable);

   if ( !bEnable )
   {
      CPGStableDoc* pDoc = (CPGStableDoc*)EAFGetDocument();
      const GirderLibrary* pLib = pDoc->GetGirderLibrary();
      const GirderLibraryEntry* pEntry = (const GirderLibraryEntry*)(pLib->GetEntry(strGirder));

      Float64 Cd = pEntry->GetDragCoefficient();

      CComPtr<IBeamFactory> factory;
      pEntry->GetBeamFactory(&factory);

      IBeamFactory::Dimensions dimensions = pEntry->GetDimensions();
      CComPtr<IGirderSection> section;
      factory->CreateGirderSection(NULL,INVALID_ID,dimensions,-1,-1,&section);

      CComPtr<IShape> pShape;
      section.QueryInterface(&pShape);
      ATLASSERT(pShape != NULL);

      CComPtr<IShapeProperties> shapeProperties;
      pShape->get_ShapeProperties(&shapeProperties);

      Float64 Wtf, Wbf, Hg;
      section->get_TopWidth(&Wtf);
      section->get_BottomWidth(&Wbf);
      section->get_GirderHeight(&Hg);

      Float64 Yt, Ag, Ix, Iy;
      shapeProperties->get_Area(&Ag);
      shapeProperties->get_Ixx(&Ix);
      shapeProperties->get_Iyy(&Iy);
      shapeProperties->get_Ytop(&Yt);

      CEAFApp* pApp = EAFGetApp();
      const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
      CDataExchange dx(this,false);
      DDX_UnitValueAndTag(&dx,IDC_HG,IDC_HG_UNIT, Hg, pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_WTF,IDC_WTF_UNIT, Wtf, pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_WBF,IDC_WBF_UNIT, Wbf, pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_AREA,IDC_AREA_UNIT,Ag,pDispUnits->Area);
      DDX_UnitValueAndTag(&dx,IDC_IX,IDC_IX_UNIT,Ix,pDispUnits->MomentOfInertia);
      DDX_UnitValueAndTag(&dx,IDC_IY,IDC_IY_UNIT,Iy,pDispUnits->MomentOfInertia);
      DDX_UnitValueAndTag(&dx,IDC_YT,IDC_YT_UNIT,Yt,pDispUnits->ComponentDim);
      DDX_Text(&dx,IDC_DRAG_COEFFICIENT,Cd);
   }
}