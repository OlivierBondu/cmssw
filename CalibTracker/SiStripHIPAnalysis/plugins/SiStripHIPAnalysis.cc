// -*- C++ -*-
//
// Package:    CalibTracker/SiStripHIPAnalysis
// Class:      SiStripHIPAnalysis
// 
/**\class SiStripHIPAnalysis SiStripHIPAnalysis.cc CalibTracker/SiStripHIPAnalysis/plugins/SiStripHIPAnalysis.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Olivier Bondu
//         Created:  Fri, 30 Oct 2015 10:45:37 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1.h"
#include "TChain.h"

#include <string>

#include <ext/hash_map>

using namespace __gnu_cxx;
using __gnu_cxx::hash_map;
using __gnu_cxx::hash;

//
// class declaration
//

struct stAPVGain{
   unsigned int Index; 
   int          Bin;
   unsigned int DetId;
   unsigned int APVId;
   unsigned int SubDet;
   float        x;
   float        y;
   float        z;
   float 	Eta;
   float 	R;
   float 	Phi;
   float  	Thickness;
   double 	FitMPV;
   double 	FitMPVErr;
   double 	FitWidth;
   double 	FitWidthErr;
   double 	FitChi2;
   double 	Gain;
   double       CalibGain;
   double 	PreviousGain;
   double 	NEntries;
   TH1F*	HCharge;
   TH1F*        HChargeN;
   bool         isMasked;
};

class SiStripHIPAnalysis : public edm::EDAnalyzer {
    public:
        explicit SiStripHIPAnalysis(const edm::ParameterSet&);
        ~SiStripHIPAnalysis();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


    private:
        virtual void beginJob() override;
        virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
        virtual void endJob() override;

        // ----------member data ---------------------------
        std::vector<std::string> VInputFiles;
        TH1D * histo; 
        unsigned int NEvents;    
        unsigned int NTracks;
        unsigned int NTotClusters;
        unsigned int NClusters;
        class isEqual{
            public:
		        template <class T> bool operator () (const T& PseudoDetId1, const T& PseudoDetId2) { return PseudoDetId1==PseudoDetId2; }
        };
        __gnu_cxx::hash_map<unsigned int, stAPVGain*,  __gnu_cxx::hash<unsigned int>, isEqual > APVsColl;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
SiStripHIPAnalysis::SiStripHIPAnalysis(const edm::ParameterSet& iConfig)
 :
    VInputFiles(iConfig.getUntrackedParameter<std::vector<std::string>>("InputFiles"))

{
    //now do what ever initialization is needed
    edm::Service<TFileService> fs;
    histo = fs->make<TH1D>("charge" , "Charges" , 200 , -2 , 2 );
    NEvents = 0;
    NTracks = 0;
    NTotClusters = 0;
    NClusters = 0;
}


SiStripHIPAnalysis::~SiStripHIPAnalysis()
{
 
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
SiStripHIPAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    for(unsigned int i=0;i<VInputFiles.size();i++)
    {
        printf("Opening file %3i/%3i --> %s\n",i+1, (int)VInputFiles.size(), (char*)(VInputFiles[i].c_str())); fflush(stdout);
        TChain* tree = new TChain("gainCalibrationTree/tree");
        tree->Add(VInputFiles[i].c_str());

        TString EventPrefix("");
        TString EventSuffix("");

        TString TrackPrefix("track");
        TString TrackSuffix("");

        TString CalibPrefix("GainCalibration");
        TString CalibSuffix("");

        unsigned int                 eventnumber    = 0;    tree->SetBranchAddress(EventPrefix + "event"          + EventSuffix, &eventnumber   , NULL);
        unsigned int                 runnumber      = 0;    tree->SetBranchAddress(EventPrefix + "run"            + EventSuffix, &runnumber     , NULL);
        std::vector<bool>*           TrigTech    = 0;    tree->SetBranchAddress(EventPrefix + "TrigTech"    + EventSuffix, &TrigTech   , NULL);

        std::vector<double>*         trackchi2ndof  = 0;    tree->SetBranchAddress(TrackPrefix + "chi2ndof"       + TrackSuffix, &trackchi2ndof , NULL);
        std::vector<float>*          trackp         = 0;    tree->SetBranchAddress(TrackPrefix + "momentum"       + TrackSuffix, &trackp        , NULL);
        std::vector<float>*          trackpt        = 0;    tree->SetBranchAddress(TrackPrefix + "pt"             + TrackSuffix, &trackpt       , NULL);
        std::vector<double>*         tracketa       = 0;    tree->SetBranchAddress(TrackPrefix + "eta"            + TrackSuffix, &tracketa      , NULL);
        std::vector<double>*         trackphi       = 0;    tree->SetBranchAddress(TrackPrefix + "phi"            + TrackSuffix, &trackphi      , NULL);
        std::vector<unsigned int>*   trackhitsvalid = 0;    tree->SetBranchAddress(TrackPrefix + "hitsvalid"      + TrackSuffix, &trackhitsvalid, NULL);

        std::vector<int>*            trackindex     = 0;    tree->SetBranchAddress(CalibPrefix + "trackindex"     + CalibSuffix, &trackindex    , NULL);
        std::vector<unsigned int>*   rawid          = 0;    tree->SetBranchAddress(CalibPrefix + "rawid"          + CalibSuffix, &rawid         , NULL);
        std::vector<float>*          localdirx      = 0;    tree->SetBranchAddress(CalibPrefix + "localdirx"      + CalibSuffix, &localdirx     , NULL);
        std::vector<float>*          localdiry      = 0;    tree->SetBranchAddress(CalibPrefix + "localdiry"      + CalibSuffix, &localdiry     , NULL);
        std::vector<float>*          localdirz      = 0;    tree->SetBranchAddress(CalibPrefix + "localdirz"      + CalibSuffix, &localdirz     , NULL);
        std::vector<unsigned short>* firststrip     = 0;    tree->SetBranchAddress(CalibPrefix + "firststrip"     + CalibSuffix, &firststrip    , NULL);
        std::vector<unsigned short>* nstrips        = 0;    tree->SetBranchAddress(CalibPrefix + "nstrips"        + CalibSuffix, &nstrips       , NULL);
        std::vector<bool>*           saturation     = 0;    tree->SetBranchAddress(CalibPrefix + "saturation"     + CalibSuffix, &saturation    , NULL);
        std::vector<bool>*           overlapping    = 0;    tree->SetBranchAddress(CalibPrefix + "overlapping"    + CalibSuffix, &overlapping   , NULL);
        std::vector<bool>*           farfromedge    = 0;    tree->SetBranchAddress(CalibPrefix + "farfromedge"    + CalibSuffix, &farfromedge   , NULL);
        std::vector<unsigned int>*   charge         = 0;    tree->SetBranchAddress(CalibPrefix + "charge"         + CalibSuffix, &charge        , NULL);
        std::vector<float>*          path           = 0;    tree->SetBranchAddress(CalibPrefix + "path"           + CalibSuffix, &path          , NULL);
        std::vector<float>*          chargeoverpath = 0;    tree->SetBranchAddress(CalibPrefix + "chargeoverpath" + CalibSuffix, &chargeoverpath, NULL);
        std::vector<unsigned char>*  amplitude      = 0;    tree->SetBranchAddress(CalibPrefix + "amplitude"      + CalibSuffix, &amplitude     , NULL);
        std::vector<double>*         gainused       = 0;    tree->SetBranchAddress(CalibPrefix + "gainused"       + CalibSuffix, &gainused      , NULL);

        printf("Number of Events = %i + %i = %i\n",NEvents,(unsigned int)tree->GetEntries(),(unsigned int)(NEvents+tree->GetEntries()));
        int TreeStep = tree->GetEntries()/50;if(TreeStep<=1)TreeStep=1;
        printf("Progressing Bar              :0%%       20%%       40%%       60%%       80%%       100%%\n");
        printf("Looping on the Tree          :");
        for (unsigned int ientry = 0; ientry < tree->GetEntries(); ientry++)
        {
            if (ientry%TreeStep==0)
                printf(".");fflush(stdout);
            tree->GetEntry(ientry);

            NEvents++;
            NTracks+=(*trackp).size();

        	unsigned int FirstAmplitude=0;
            for (unsigned int i=0;i<(*chargeoverpath).size();i++)
            { // Loop over clusters
                NTotClusters++;
                if (!(*saturation)[i]) continue;
                
                FirstAmplitude+=(*nstrips)[i];
                int NSaturatedStrips = 0;
                for (unsigned int s=0;s<(*nstrips)[i];s++)
                {
                    int StripCharge =  (*amplitude)[FirstAmplitude-(*nstrips)[i]+s];
                    if (StripCharge > 253)
                        NSaturatedStrips++;
                }
                if (NSaturatedStrips >= 3)
                    NClusters++;

            }// End of loop over clusters

        }printf("\n");// End of loop over events

    } // End of loop over files

    std::cout << "NEvents= " << NEvents << "\tNTracks= " << NTracks << "\tNClusters / NTotClusters= " << NClusters << " / " << NTotClusters << std::endl;   

}


// ------------ method called once each job just before starting event loop  ------------
void 
SiStripHIPAnalysis::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SiStripHIPAnalysis::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
SiStripHIPAnalysis::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
SiStripHIPAnalysis::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
SiStripHIPAnalysis::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
SiStripHIPAnalysis::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SiStripHIPAnalysis::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SiStripHIPAnalysis);
