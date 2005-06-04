
#include "EXVTXKalDetector.h"
#include "EXVTXMeasLayer.h"
#include "EXVTXHit.h"
#include "TRandom.h"

ClassImp(EXVTXKalDetector)

EXVTXKalDetector::EXVTXKalDetector(Int_t m)
                : EXVKalDetector(m)
{
   Double_t A, Z, density, radlen;
   A       = 14.00674 * 0.7 + 15.9994 * 0.3;
   Z       = 7.3;
   density = 1.205e-3;
   radlen  = 3.42e4;
   TMaterial &air = *new TMaterial("VTXAir", "", A, Z, density, radlen, 0.);

   A       = 28.0855;
   Z       = 14.;
   density = 2.33;
   radlen  = 9.36;
   TMaterial &si = *new TMaterial("VTXSi", "", A, Z, density, radlen, 0.);

   static const Int_t    nlayers   = 4;
   static const Double_t lhalf     = 5.;
   static const Double_t lhalfstep = 2.5;
   static const Double_t rmin      = 2.4;
   static const Double_t rstep     = 1.2;
   static const Double_t thick     = 0.033;
                                                                                
   static const Double_t sigmax    = 4.e-4;
   static const Double_t sigmaz    = 4.e-4;

   Bool_t active = EXVTXMeasLayer::kActive;
   Bool_t dummy  = EXVTXMeasLayer::kDummy;

   // create measurement layers of central tracker
   Double_t r   = rmin;
   Double_t len = lhalf;
   for (Int_t layer=0; layer < nlayers; layer++) {
      Add(new EXVTXMeasLayer(air, si, r, len, sigmax, sigmaz, active));
      Add(new EXVTXMeasLayer(si, air, r + thick, len, sigmax, sigmaz, dummy));
      len += lhalfstep;
      r += rstep;
   }
   SetOwner();
}

EXVTXKalDetector::~EXVTXKalDetector()
{
}

void EXVTXKalDetector::ProcessHit(const TVector3    &xx,
                                  const TVMeasLayer &ms,
                                        TObjArray   &hits)
{
   const EXVTXMeasLayer &vms = dynamic_cast<const EXVTXMeasLayer &>(ms);
   TKalMatrix h    = vms.XvToMv(xx);
   Double_t   rphi = h(0, 0);
   Double_t   z    = h(1, 0);

   Double_t dx = vms.GetSigmaX();
   Double_t dz = vms.GetSigmaZ();
   rphi += gRandom->Gaus(0., dx);   // smearing rphi
   z    += gRandom->Gaus(0., dz);   // smearing z

   Double_t meas [2];
   Double_t dmeas[2];
   meas [0] = rphi;
   meas [1] = z;
   dmeas[0] = dx;
   dmeas[1] = dz;

   hits.Add(new EXVTXHit(vms, meas, dmeas, xx, GetBfield(xx)));
}
