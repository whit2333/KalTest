//*************************************************************************
//* ===================
//*  EXITMeasLayer Class
//* ===================
//*
//* (Description)
//*   Sample measurement layer class used by EXITHit.
//* (Requires)
//* (Provides)
//*     class EXITMeasLayer
//* (Update Recored)
//*   2003/09/30  Y.Nakashima       Original version.
//*   2005/07/25  Kim, Youngim
//*************************************************************************
//

#include "EXITMeasLayer.h"
#include "EXITHit.h"
#include "TRandom.h"
#include "TMath.h"
#include "EXITKalDetector.h"

ClassImp(EXITMeasLayer)
                                                                                
EXITMeasLayer::EXITMeasLayer(TMaterial &min,
                             TMaterial &mout,
                             Double_t   r0,
                             Double_t   lhalf,
                             Double_t   sigmax,
                             Double_t   sigmaz,
                             Bool_t     type)
             : EXVMeasLayer(min, mout, type),
               TCylinder(r0, lhalf),
               fSigmaX(sigmax),
               fSigmaZ(sigmaz)
{
}

EXITMeasLayer::~EXITMeasLayer()
{
}

TKalMatrix EXITMeasLayer::XvToMv(const TVector3 &xv) const
{
   // Calculate hit coordinate information:
   //	mv(0,0) = r * phi 
   //     (1,0) = z

   TKalMatrix mv(kMdim,1);
   mv(0,0)  = GetR() * TMath::ATan2(xv.Y(), xv.X());
   mv(1,0)  = xv.Z();
   return mv;
}

TKalMatrix EXITMeasLayer::XvToMv(const TVTrackHit &,
                                 const TVector3   &xv) const
{
   return XvToMv(xv);
}

TVector3 EXITMeasLayer::HitToXv(const TVTrackHit &vht) const
{
   const EXITHit &ht = dynamic_cast<const EXITHit &>(vht);

   Double_t phi = ht(0,0) / GetR();
   Double_t z   = ht(1,0);
   Double_t x   = GetR() * TMath::Cos(phi);
   Double_t y   = GetR() * TMath::Sin(phi);

   return TVector3(x,y,z);
}

void EXITMeasLayer::CalcDhDa(const TVTrackHit &vht,
                             const TVector3   &xxv,
                             const TKalMatrix &dxphiada,
                                   TKalMatrix &H)  const
{
   // Calculate
   //    H = (@h/@a) = (@phi/@a, @z/@a)^t
   // where
   //        h(a) = (phi, z)^t: expected meas vector
   //        a = (drho, phi0, kappa, dz, tanl, t0)
   //

   Int_t sdim = H.GetNcols();
   Int_t hdim = TMath::Max(5,sdim-1);

   Double_t xv = xxv.X();
   Double_t yv = xxv.Y();
   Double_t xxyy = xv * xv + yv * yv;

   // Set H = (@h/@a) = (@d/@a, @z/@a)^t
   
   for (Int_t i=0; i<hdim; i++) {
      H(0,i) = - (yv / xxyy) * dxphiada(0,i) 
               + (xv / xxyy) * dxphiada(1,i);
      H(0,i) *= GetR();
      H(1,i) =  dxphiada(2,i);
   }
   if (sdim == 6) {
      H(0,sdim-1) = 0.;
      H(1,sdim-1) = 0.;
   }
}

void EXITMeasLayer::ProcessHit(const TVector3  &xx,
                                     TObjArray &hits)
{
   TKalMatrix h    = XvToMv(xx);
   Double_t   rphi = h(0, 0);
   Double_t   z    = h(1, 0);

   Double_t dx = GetSigmaX();
   Double_t dz = GetSigmaZ();
   rphi += gRandom->Gaus(0., dx);   // smearing rphi
   z    += gRandom->Gaus(0., dz);   // smearing z

   Double_t meas [2];
   Double_t dmeas[2];
   meas [0] = rphi;
   meas [1] = z;
   dmeas[0] = dx;
   dmeas[1] = dz;

   Double_t b = EXITKalDetector::GetBfield();
   hits.Add(new EXITHit(*this, meas, dmeas, xx, b));
}
