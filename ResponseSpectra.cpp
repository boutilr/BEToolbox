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

#include "stdafx.h"
#include "ResponseSpectra.h"
#include <MathEx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CResponseSpectra::CResponseSpectra()
{
}

CResponseSpectra::CResponseSpectra(const CResponseSpectra& other)
{
   *this = other;
}

CResponseSpectra& CResponseSpectra::operator=(const CResponseSpectra& other)
{
   Init(other.m_PGA,other.m_Ss,other.m_S1,other.m_Fpga,other.m_Fa,other.m_Fv);
   return *this;
}

void CResponseSpectra::Init(Float64 PGA,Float64 Ss,Float64 S1,Float64 Fpga,Float64 Fa,Float64 Fv)
{
   m_PGA  = PGA;
   m_Ss   = Ss;
   m_S1   = S1;
   m_Fpga = Fpga;
   m_Fa   = Fa;
   m_Fv   = Fv;
}

Float64 CResponseSpectra::GetPGA() const
{
   return m_PGA;
}

Float64 CResponseSpectra::GetSs() const
{
   return m_Ss;
}

Float64 CResponseSpectra::GetS1() const
{
   return m_S1;
}

Float64 CResponseSpectra::GetFpga() const
{
   return m_Fpga;
}

Float64 CResponseSpectra::GetFa() const
{
   return m_Fa;
}

Float64 CResponseSpectra::GetFv() const
{
   return m_Fv;
}

Float64 CResponseSpectra::GetTo() const
{
   return 0.2*GetTs();
}

Float64 CResponseSpectra::GetTs() const
{
   return GetSd1()/GetSds();
}

Float64 CResponseSpectra::GetAs() const
{
   return m_Fpga*m_PGA;
}

Float64 CResponseSpectra::GetSds() const
{
   return m_Fa*m_Ss;
}

Float64 CResponseSpectra::GetSd1() const
{
   return m_Fv*m_S1;
}

const TCHAR* CResponseSpectra::GetSDC() const
{
   Float64 Sd1 = GetSd1();
   if ( Sd1 < 0.15 )
   {
      return _T("A");
   }
   else if ( 0.15 <= Sd1 && Sd1 < 0.30 )
   {
      return _T("B");
   }
   else if ( 0.30 <= Sd1 && Sd1 < 0.50 )
   {
      return _T("C");
   }
   else
   {
      return _T("D");
   }
}

Float64 CResponseSpectra::GetCsm(Float64 time) const
{
   if ( time < 0 )
      return 0;

   Float64 As = GetAs();
   Float64 Sds = GetSds();
   Float64 Sd1 = GetSd1();
   Float64 Ts = GetTs();
   Float64 To = GetTo();
   if ( 0 <= time && time <= 0.2*Ts )
   {
      return ::LinInterp(time,As,Sds,To);
   }
   else if ( 0.2*Ts < time && time <= Ts )
   {
      return Sds;
   }
   else
   {
      return Sd1/time;
   }
}

std::vector<std::pair<Float64,Float64>> CResponseSpectra::GetSpectraValues(IndexType nPoints,Float64 Tmax)
{
   Float64 To = GetTo();
   Float64 Ts = GetTs();

   std::vector<std::pair<Float64,Float64>> values;
   values.push_back(std::make_pair(0.0,GetCsm(0.0)));
   values.push_back(std::make_pair(To,GetCsm(To)));
   values.push_back(std::make_pair(Ts,GetCsm(Ts)));

   Float64 range = Tmax - Ts;
   if ( range < 0 || nPoints <= 3 )
   {
      return values;
   }

   nPoints -= values.size();
   Float64 step = range/nPoints;
   for ( IndexType i = 1; i <= nPoints; i++ )
   {
      Float64 t = Ts + i*step;
      values.push_back(std::make_pair(t,GetCsm(t)));
   }

   return values;
}