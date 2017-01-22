// -*- C++ -*-
//
// Package:    seedAnalyzer/seedAnalyzer
// Class:      seedAnalyzer
// 
/**\class seedAnalyzer seedAnalyzer.cc seedAnalyzer/seedAnalyzer/plugins/seedAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Leonardo GIANNINI
//         Created:  Thu, 01 Dec 2016 15:40:12 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"



#include <DataFormats/TrackReco/interface/Track.h>
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoBTag/SecondaryVertex/interface/SecondaryVertex.h"

#include "DataFormats/Candidate/interface/VertexCompositePtrCandidateFwd.h"

#include <RecoVertex/VertexPrimitives/interface/BasicVertexState.h>
#include "RecoVertex/VertexPrimitives/interface/ConvertToFromReco.h"
#include "RecoVertex/VertexPrimitives/interface/VertexState.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "RecoVertex/VertexTools/interface/VertexDistance.h"
#include "RecoVertex/VertexTools/interface/SharedTracks.h"

#include <DataFormats/Candidate/interface/Candidate.h>
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/CandidatePtrTransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/PatternTools/interface/TwoTrackMinimumDistance.h"


#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "seedAnalyzer/seedAnalyzer/interface/trackVars.h"



/*#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"


#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
*/

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "TrackingTools/GeomPropagators/interface/AnalyticalImpactPointExtrapolator.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertToFromReco.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class seedAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit seedAnalyzer(const edm::ParameterSet&);
      ~seedAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      // ----------member data ---------------------------
      
      int funzionePippo (int vivaLaVita);
      int genLevel(std::vector<reco::GenParticle> genP, reco::TransientTrack seed, std::vector<int>& MomFlav, std::vector<int>& BChain, std::vector<double>& allChi);
      
      int seedSisters(std::vector<reco::GenParticle> genP, int genP_index,  std::vector<trackVars> nearTracks);
      int genMap(std::vector<reco::GenParticle> genP, std::vector<reco::TransientTrack> tracks, std::vector<int>& genPnumbers, std::vector<double>& allChi);
      double trackEff(std::vector<reco::GenParticle> genP, std::vector<reco::TransientTrack> tracks, double &BChain_eff, double &DChain_eff, double &B_eff,
      double &low_pt_eff, double &high_pt_eff, std::vector<double> &minGenChiSquare);
      
      edm::Service<TFileService> file;
      edm::RefVector<std::vector<reco::GenParticle> > mothers;
      TTree *tree;
      
      double m;
      float n;
      
      double match_eff=0;
      double tracking_eff=-10;
      
      double BChain_eff=0;
      double DChain_eff=0;
      double B_eff=0;
      double low_pt_eff=0;
      double high_pt_eff=0;
      
      int b_found=0, d_found=0;
      int id_mom;
      
      double gen_pv_x=-10;
      double gen_pv_y=-10;
      double gen_pv_z=-10;
      
      double pv_x=-10;
      double pv_y=-10;
      double pv_z=-10;
      
      int evt=0;
      int lumi=0;
      int run=0;
      
      int n_seed=0;
      
      std::vector<int> genP_index;
      
      std::vector<double> seed_pt;
      std::vector<double> seed_eta;
      std::vector<double> seed_phi;
      std::vector<double> seed_mass;
      
      std::vector<double> seed_dz;
      std::vector<double> seed_dxy;
      std::vector<double> seed_3D_ip;
      std::vector<double> seed_3D_sip;
      std::vector<double> seed_2D_ip;
      std::vector<double> seed_2D_sip;
      
      //vars from matching genParticles if any
      std::vector<double> seed_MC_pt;
      std::vector<double> seed_MC_eta;
      std::vector<double> seed_MC_phi;
      std::vector<double> seed_MC_mass;
      std::vector<double> seed_MC_dz;
      std::vector<double> seed_MC_dxy;
      std::vector<int> seed_MC_MomPdgId;
      std::vector<int> seed_MC_MomFlavour;
      std::vector<int> seed_MC_BChain;
      std::vector<int> seed_MC_DChain; //no B in this case
      std::vector<double> seed_MC_vx;
      std::vector<double> seed_MC_vy;
      std::vector<double> seed_MC_vz;
      std::vector<double> seed_MC_pvd;
      //end vars from matching genParticles if any
      
      std::vector<int> nearTracks_Nvtx;
      std::vector<int> nearTracks_nTracks;//max 20
      std::vector<double> nearTracks_pt;
      std::vector<double> nearTracks_eta;
      std::vector<double> nearTracks_phi;
      std::vector<double> nearTracks_mass;
      std::vector<double> nearTracks_dz;
      std::vector<double> nearTracks_dxy;
      std::vector<double> nearTracks_3D_ip;
      std::vector<double> nearTracks_3D_sip;
      std::vector<double> nearTracks_2D_ip;
      std::vector<double> nearTracks_2D_sip;
      
      std::vector<double> nearTracks_PCAdist;
      std::vector<double> nearTracks_PCAdsig;
      
      std::vector<double> nearTracks_PCAonSeed_x;
      std::vector<double> nearTracks_PCAonSeed_y;
      std::vector<double> nearTracks_PCAonSeed_z;
      
      std::vector<double> nearTracks_PCAonSeed_xerr;
      std::vector<double> nearTracks_PCAonSeed_yerr;
      std::vector<double> nearTracks_PCAonSeed_zerr;
      
      std::vector<double> nearTracks_PCAonTrack_x;
      std::vector<double> nearTracks_PCAonTrack_y;
      std::vector<double> nearTracks_PCAonTrack_z;
      
      std::vector<double> nearTracks_PCAonTrack_xerr;
      std::vector<double> nearTracks_PCAonTrack_yerr;
      std::vector<double> nearTracks_PCAonTrack_zerr; 

      std::vector<double> nearTracks_dotprodTrack;
      std::vector<double> nearTracks_dotprodSeed;
      std::vector<double> nearTracks_dotprodTrackSeed2D;
      std::vector<double> nearTracks_dotprodTrackSeed3D;
      std::vector<double> nearTracks_dotprodTrackSeedVectors2D;
      std::vector<double> nearTracks_dotprodTrackSeedVectors3D;
      
      std::vector<double> nearTracks_PCAonSeed_pvd;
      std::vector<double> nearTracks_PCAonTrack_pvd;
      
      
      //vars from matching genParticles if any
      std::vector<double> nearTracks_MC_pt;
      std::vector<double> nearTracks_MC_eta;
      std::vector<double> nearTracks_MC_phi;
      std::vector<double> nearTracks_MC_dz;
      std::vector<double> nearTracks_MC_dxy;
      
      std::vector<int> nearTracks_MC_MomPdgId;
      std::vector<int> nearTracks_MC_MomFlavour;
      std::vector<int> nearTracks_MC_BChain;
      std::vector<int> nearTracks_MC_DChain; //no B in this case

      std::vector<double> nearTracks_MC_Track_vx;
      std::vector<double> nearTracks_MC_Track_vy;
      std::vector<double> nearTracks_MC_Track_vz;
      
      std::vector<double> nearTracks_MC_fromSeedVtx;
      std::vector<double> nearTracks_MC_pvd;
      //end vars from matching genParticles if any

      std::vector<double> Match_chisquare;
      std::vector<double> minChiSquare_genP;
      std::vector<trackVars> nearTracks;
      trackVars myTrack;

      int min3DIPValue=0;
      int min3DIPSignificance=2;
      int max3DIPValue=100;
      int max3DIPSignificance=10;
      
      
      edm::EDGetTokenT<edm::View<reco::Candidate> > CandidateToken;
      edm::EDGetTokenT<std::vector<reco::GenParticle> > genParticleToken;

      edm::EDGetTokenT<reco::VertexCollection> token_primaryVertex;
      edm::EDGetTokenT<reco::BeamSpot> token_beamSpot;
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
seedAnalyzer::seedAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   usesResource("TFileService");
    tree=file->make<TTree>("tree","tree");
    
    tree->Branch("lumi",&lumi, "lumi/I");
    tree->Branch("evt",&evt, "evt/I");
    tree->Branch("run",&run, "run/I");
    
    tree->Branch("gen_pv_x",&gen_pv_x, "gen_pv_x/D");
    tree->Branch("gen_pv_y",&gen_pv_y, "gen_pv_y/D");
    tree->Branch("gen_pv_z",&gen_pv_z, "gen_pv_z/D");
    tree->Branch("pv_x",&pv_x, "pv_x/D");
    tree->Branch("pv_y",&pv_y, "pv_y/D");
    tree->Branch("pv_z",&pv_z, "pv_z/D");
    
    tree->Branch("match_eff", &match_eff, "match_eff/D");
    tree->Branch("tracking_eff", &tracking_eff, "tracking_eff/D");
    
    tree->Branch("BChain_eff", &BChain_eff, "BChain_eff/D");
    tree->Branch("DChain_eff", &DChain_eff, "DChain_eff/D");
    tree->Branch("B_eff", &B_eff, "B_eff/D");
    tree->Branch("low_pt_eff", &low_pt_eff, "low_pt_eff/D");
    tree->Branch("high_pt_eff", &high_pt_eff, "high_pt_eff/D");
    
    tree->Branch("n_seed",&n_seed, "n_seed/I");

    tree->Branch("seed_pt",&seed_pt);
    tree->Branch("seed_eta",&seed_eta);
    tree->Branch("seed_phi",&seed_phi);
    tree->Branch("seed_mass",&seed_mass);
    
    tree->Branch("seed_dz", &seed_dz);
    tree->Branch("seed_dxy", &seed_dxy);
    tree->Branch("seed_3D_ip", &seed_3D_ip);
    tree->Branch("seed_3D_sip", &seed_3D_sip);
    tree->Branch("seed_2D_ip", &seed_2D_ip);
    tree->Branch("seed_2D_sip", &seed_2D_sip);
    
    tree->Branch("seed_MC_pt",&seed_MC_pt);
    tree->Branch("seed_MC_eta",&seed_MC_eta);
    tree->Branch("seed_MC_phi",&seed_MC_phi);
    tree->Branch("seed_MC_mass",&seed_MC_mass);
    tree->Branch("seed_MC_dz",&seed_MC_dz);
    tree->Branch("seed_MC_dxy",&seed_MC_dxy);
    tree->Branch("seed_MC_MomFlavour",&seed_MC_MomFlavour);
    tree->Branch("seed_MC_MomPdgId",&seed_MC_MomPdgId);
    tree->Branch("seed_MC_BChain",&seed_MC_BChain);
    tree->Branch("seed_MC_DChain",&seed_MC_DChain);
    tree->Branch("seed_MC_vx", &seed_MC_vx);
    tree->Branch("seed_MC_vy", &seed_MC_vy);
    tree->Branch("seed_MC_vz", &seed_MC_vz);
    tree->Branch("seed_MC_pvd", &seed_MC_pvd);
    
    tree->Branch("nearTracks_Nvtx", &nearTracks_Nvtx);
    tree->Branch("nearTracks_nTracks", &nearTracks_nTracks);
    tree->Branch("nearTracks_pt", &nearTracks_pt);
    tree->Branch("nearTracks_eta", &nearTracks_eta);
    tree->Branch("nearTracks_phi", &nearTracks_phi);
    tree->Branch("nearTracks_mass", &nearTracks_mass);
    tree->Branch("nearTracks_dz", &nearTracks_dz);
    tree->Branch("nearTracks_dxy", &nearTracks_dxy);
    tree->Branch("nearTracks_3D_ip", &nearTracks_3D_ip);
    tree->Branch("nearTracks_3D_sip", &nearTracks_3D_sip);
    tree->Branch("nearTracks_2D_ip", &nearTracks_2D_ip);
    tree->Branch("nearTracks_2D_sip", &nearTracks_2D_sip);

    tree->Branch("nearTracks_PCAdist", &nearTracks_PCAdist);
    tree->Branch("nearTracks_PCAdsig", &nearTracks_PCAdsig);
    
    tree->Branch("nearTracks_PCAonSeed_x", &nearTracks_PCAonSeed_x);
    tree->Branch("nearTracks_PCAonSeed_y", &nearTracks_PCAonSeed_y);
    tree->Branch("nearTracks_PCAonSeed_z", &nearTracks_PCAonSeed_z);

    tree->Branch("nearTracks_PCAonSeed_xerr", &nearTracks_PCAonSeed_xerr);
    tree->Branch("nearTracks_PCAonSeed_yerr", &nearTracks_PCAonSeed_yerr);
    tree->Branch("nearTracks_PCAonSeed_zerr", &nearTracks_PCAonSeed_zerr);

    tree->Branch("nearTracks_PCAonTrack_x", &nearTracks_PCAonTrack_x);
    tree->Branch("nearTracks_PCAonTrack_y", &nearTracks_PCAonTrack_y);
    tree->Branch("nearTracks_PCAonTrack_z", &nearTracks_PCAonTrack_z);

    tree->Branch("nearTracks_PCAonTrack_xerr", &nearTracks_PCAonTrack_xerr);
    tree->Branch("nearTracks_PCAonTrack_yerr", &nearTracks_PCAonTrack_yerr);
    tree->Branch("nearTracks_PCAonTrack_zerr", &nearTracks_PCAonTrack_zerr); 

    tree->Branch("nearTracks_dotprodTrack", &nearTracks_dotprodTrack);
    tree->Branch("nearTracks_dotprodSeed", &nearTracks_dotprodSeed);
    tree->Branch("nearTracks_dotprodTrackSeed2D", &nearTracks_dotprodTrackSeed2D);
    tree->Branch("nearTracks_dotprodTrackSeed3D", &nearTracks_dotprodTrackSeed3D);
    tree->Branch("nearTracks_dotprodTrackSeedVectors2D", &nearTracks_dotprodTrackSeedVectors2D);
    tree->Branch("nearTracks_dotprodTrackSeedVectors3D", &nearTracks_dotprodTrackSeedVectors3D);
    
    tree->Branch("nearTracks_PCAonSeed_pvd", &nearTracks_PCAonSeed_pvd);
    tree->Branch("nearTracks_PCAonTrack_pvd", &nearTracks_PCAonTrack_pvd);
    
    tree->Branch("nearTracks_MC_pt", &nearTracks_MC_pt);
    tree->Branch("nearTracks_MC_eta", &nearTracks_MC_eta);
    tree->Branch("nearTracks_MC_phi", &nearTracks_MC_phi);
    tree->Branch("nearTracks_MC_dz", &nearTracks_MC_dz);
    tree->Branch("nearTracks_MC_dxy", &nearTracks_MC_dxy);
    tree->Branch("nearTracks_MC_MomFlavour", &nearTracks_MC_MomFlavour);
    tree->Branch("nearTracks_MC_MomPdgId", &nearTracks_MC_MomPdgId);
    tree->Branch("nearTracks_MC_BChain", &nearTracks_MC_BChain);
    tree->Branch("nearTracks_MC_DChain", &nearTracks_MC_DChain);

    tree->Branch("nearTracks_MC_Track_vx", &nearTracks_MC_Track_vx);
    tree->Branch("nearTracks_MC_Track_vy", &nearTracks_MC_Track_vy);
    tree->Branch("nearTracks_MC_Track_vz", &nearTracks_MC_Track_vz);
    
    tree->Branch("nearTracks_MC_fromSeedVtx", &nearTracks_MC_fromSeedVtx);
    tree->Branch("nearTracks_MC_pvd", &nearTracks_MC_pvd);
    tree->Branch("Match_chisquare", &Match_chisquare);
    
    tree->Branch("minChiSquare_genP", &minChiSquare_genP);

    CandidateToken = consumes<edm::View<reco::Candidate>>(edm::InputTag("particleFlow"));
    token_primaryVertex = consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
    token_beamSpot = consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));
    
    genParticleToken = consumes<std::vector<reco::GenParticle> >(edm::InputTag("genParticles"));

}


seedAnalyzer::~seedAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
seedAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

    evt=iEvent.id().event();
    lumi=iEvent.id().luminosityBlock();
    run=iEvent.id().run();

    seed_pt.clear();
    seed_eta.clear();
    seed_phi.clear();
    seed_mass.clear();
    seed_dz.clear();
    seed_dxy.clear();
    seed_3D_ip.clear();
    seed_3D_sip.clear();
    seed_2D_ip.clear();
    seed_2D_sip.clear();
    
    seed_MC_MomFlavour.clear();
    seed_MC_MomPdgId.clear();
    seed_MC_BChain.clear();
    seed_MC_DChain.clear();
    
    nearTracks_nTracks.clear();
    
    nearTracks_Nvtx.clear();
    nearTracks_pt.clear();
    nearTracks_eta.clear();
    nearTracks_phi.clear();
    nearTracks_dz.clear();
    nearTracks_dxy.clear();
    nearTracks_PCAdist.clear();
    nearTracks_PCAdsig.clear();
    nearTracks_PCAonSeed_x.clear();
    nearTracks_PCAonSeed_y.clear();
    nearTracks_PCAonSeed_z.clear();
    nearTracks_PCAonSeed_xerr.clear();
    nearTracks_PCAonSeed_yerr.clear();
    nearTracks_PCAonSeed_zerr.clear();
    nearTracks_PCAonTrack_x.clear();
    nearTracks_PCAonTrack_y.clear();
    nearTracks_PCAonTrack_z.clear();
    nearTracks_PCAonTrack_xerr.clear();
    nearTracks_PCAonTrack_yerr.clear();
    nearTracks_PCAonTrack_zerr.clear();
    nearTracks_dotprodTrack.clear();
    nearTracks_dotprodSeed.clear();
    
    nearTracks_PCAonSeed_pvd.clear();
    nearTracks_PCAonTrack_pvd.clear();
    
    //vars from matching genParticles if any
    nearTracks_MC_pt.clear();
    nearTracks_MC_eta.clear();
    nearTracks_MC_phi.clear();
    nearTracks_MC_dz.clear();
    nearTracks_MC_dxy.clear();

    nearTracks_MC_MomFlavour.clear();
    nearTracks_MC_MomPdgId.clear();
    nearTracks_MC_BChain.clear();
    nearTracks_MC_DChain.clear();

    seed_MC_vx.clear();
    seed_MC_vy.clear();
    seed_MC_vz.clear();
    seed_MC_pvd.clear();
    nearTracks_MC_Track_vx.clear();
    nearTracks_MC_Track_vy.clear();
    nearTracks_MC_Track_vz.clear();
    
    nearTracks_MC_fromSeedVtx.clear();
    nearTracks_MC_pvd.clear();
    
    seed_MC_pt.clear();
    seed_MC_eta.clear();
    seed_MC_phi.clear();
    seed_MC_mass.clear();
    seed_MC_dz.clear();
    seed_MC_dxy.clear();
    //end vars from matching genParticles if any
    
    Match_chisquare.clear();


    using namespace edm;
    using namespace reco;
    
    
       //1st way to get objects
    Handle<std::vector<reco::GenParticle> > genParticlesCollection;
    iEvent.getByToken(genParticleToken, genParticlesCollection);
    std::vector<reco::GenParticle>  genParticles = *genParticlesCollection.product();
    
       //2nd way to get objects
    Handle<edm::View<reco::Candidate> > tracks;
    iEvent.getByToken(CandidateToken, tracks);

    edm::Handle<reco::VertexCollection > primaryVertices;
    iEvent.getByToken(token_primaryVertex, primaryVertices);
   
//   std::cout << tracks->size() << std::endl;
   
   if(primaryVertices->size()!=0){
        const reco::Vertex &pv = (*primaryVertices)[0];
        GlobalPoint pvp(pv.x(),pv.y(),pv.z());
//        std::cout << pv.x() << std::endl;
        
        gen_pv_x=genParticles[2].vx();
        gen_pv_y=genParticles[2].vy();
        gen_pv_z=genParticles[2].vz();
        
        GlobalPoint gpvp(gen_pv_x,gen_pv_y,gen_pv_z);
        
        pv_x=pv.x();
        pv_y=pv.y();
        pv_z=pv.z();
        
        std::cout << "check the primary  " << std::endl;
        std::cout <<" gen -->" << gen_pv_x << ", "<< gen_pv_y << ", "<< gen_pv_z << "   " ;
        std::cout <<" reco -->" << pv_x << ", "<< pv_y << ", "<< pv_z <<std::endl;


        edm::ESHandle<TransientTrackBuilder> trackBuilder;
        iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);

        edm::Handle<BeamSpot> beamSpot;
        iEvent.getByToken(token_beamSpot,beamSpot);
        
        
        std::cout << "Transient tracks" << std::endl;
        std::vector<TransientTrack> selectedTracks;
        
        std::cout << "Masses" << std::endl;
        std::vector<float> masses;
        
        //if primary: build transient tracks form packedCandidates
        
        for(typename edm::View<reco::Candidate>::const_iterator track = tracks->begin(); track != tracks->end(); ++track) {
        
            unsigned int k=track - tracks->begin();
//            std::cout << k << std::endl;
            if((*tracks)[k].bestTrack() != 0 && std::fabs(pv_z-(*tracks)[k].vz())<0.1  && (*tracks)[k].pt()>0.5) {
            selectedTracks.push_back(trackBuilder->build(tracks->ptrAt(k)));
            masses.push_back(tracks->ptrAt(k)->mass());}
            }
        
        std::cout << "function entering .." << std::endl;
        tracking_eff=trackEff(genParticles, selectedTracks, BChain_eff, DChain_eff, B_eff, low_pt_eff, high_pt_eff, minChiSquare_genP);
        
        //selected tracks are all the good tracks, now match to the gen particles for later reference
        genMap(genParticles, selectedTracks, genP_index, Match_chisquare );
        
        match_eff=0;
        
        for(std::vector<TransientTrack>::const_iterator it = selectedTracks.begin(); it != selectedTracks.end(); it++){
        
        int index= it - selectedTracks.begin();
        
        std::cout << " see how the matching went  : new particle" << "  eff:  " << match_eff << "/" << selectedTracks.size() <<  "  at index " << index << std::endl;
        
//        std::cout << " " << it->track().pt() << " " << it->track().eta() <<  " " << it->track().phi()   << std::endl;
        std::cout << " " << it->track().pt() << " " << it->track().eta() <<  " " << it->track().phi()   << std::endl;
        std::cout <<" vertex for the track  " <<  it->track().vx() <<  " "<< it->track().vy() <<  " " << it->track().vz() << std::endl;
        std::cout <<" reco primary -->" << pv_x << ", "<< pv_y << ", "<< pv_z <<std::endl;
        std::cout << " " << index << " == " << it - selectedTracks.begin() <<  " " << genP_index[index]   << "  " << genP_index.size() <<std::endl;
        
        
        
        int g_index=genP_index[index];
        if (g_index>0){
        match_eff=match_eff+1;
        
        std::cout << " " << genParticles[g_index].pt() << " " << genParticles[g_index].eta() <<  " " << genParticles[g_index].phi()   << std::endl;
        }
        else
        {
        std::cout << " no particle match" << std::endl;
        }
        
        }
        match_eff=match_eff/selectedTracks.size();
        
        
        
        std::vector<TransientTrack> seeds;        
        //if primary: build transient tracks form packedCandidates
        std::cout << "seeds" << std::endl;

        for(std::vector<TransientTrack>::const_iterator it = selectedTracks.begin(); it != selectedTracks.end(); it++){
        
        
        std::pair<bool,Measurement1D> ip = IPTools::absoluteImpactParameter3D(*it,pv);        
        std::pair<bool,Measurement1D> ip2d = IPTools::absoluteTransverseImpactParameter(*it,pv);
        
        
            if(ip.first && ip.second.value() >= min3DIPValue && ip.second.significance() >= min3DIPSignificance &&
            ip.second.value() <= max3DIPValue && ip.second.significance() <= max3DIPSignificance &&
            ip2d.second.value() >= min3DIPValue && ip2d.second.significance() >= min3DIPSignificance &&
            ip2d.second.value() <= max3DIPValue && ip2d.second.significance() <= max3DIPSignificance
            
            )
            { 
            
            std::cout << "new seed : 2d+3d sign." << ip2d.second.significance() << "  " << ip.second.significance() <<"  "<< std::endl;
//              std::cout << "new seed " <<  it-selectedTracks.begin() << " ref " << "qualcosa key"  << " " << ip.second.value() <<std::endl;
//              std::cout << ip.second.significance() << " " << it->track().hitPattern().trackerLayersWithMeasurement() <<std::endl;
//              std::cout << " " << it->track().pt() << " " << it->track().eta() << std::endl;
            int index= it - selectedTracks.begin();
            int g_index=genP_index[index];
            if (g_index>0){
//                match_eff=match_eff+1;
                std::cout << " " << it->track().pt() << " " << it->track().eta() <<  " " << it->track().phi()   << std::endl;
                std::cout << " " << genParticles[g_index].pt() << " " << genParticles[g_index].eta() <<  " " << genParticles[g_index].phi()   << std::endl;
                }
            else
            std::cout << "no match for seed " << it->track().pt() << " " << it->track().eta() <<  " " << it->track().phi()   << std::endl;
//            std::cout << "new seed " <<  it-selectedTracks.begin() << " ref " << it->trackBaseRef().key()  << " " << ip.second.value() << " " << ip.second.significance() << " " << it->track().hitPattern().trackerLayersWithMeasurement() << " " << it->track().pt() << " " << it->track().eta() << std::endl;
            seeds.push_back(*it);
            
            seed_pt.push_back(it->track().pt());
            seed_eta.push_back(it->track().eta());
            seed_phi.push_back(it->track().phi());
            seed_mass.push_back(masses[it-selectedTracks.begin()]);
            
            seed_dz.push_back(it->track().dz());
            seed_dxy.push_back(it->track().dxy());
            seed_3D_ip.push_back(ip.second.value());
            seed_3D_sip.push_back(ip.second.significance());
            seed_2D_ip.push_back(ip2d.second.value());
            seed_2D_sip.push_back(ip2d.second.significance());
            
            if (genP_index[it - selectedTracks.begin()]>0){
            int g_i=genP_index[it - selectedTracks.begin()];
            seed_MC_pt.push_back(genParticles[g_i].pt());
            seed_MC_eta.push_back(genParticles[g_i].eta());
            seed_MC_phi.push_back(genParticles[g_i].phi());
            seed_MC_mass.push_back(genParticles[g_i].mass());
            seed_MC_dxy.push_back((-genParticles[g_i].vx() * genParticles[g_i].py() + genParticles[g_i].vy() * genParticles[g_i].px()) / genParticles[g_i].pt());
            seed_MC_dz.push_back(genParticles[g_i].vz() - (genParticles[g_i].vx() * genParticles[g_i].px() + genParticles[g_i].vy() * genParticles[g_i].py()) / genParticles[g_i].pt() * (genParticles[g_i].pz() / genParticles[g_i].pt()));
            seed_MC_vx.push_back(genParticles[g_i].vx());
            seed_MC_vy.push_back(genParticles[g_i].vy());
            seed_MC_vz.push_back(genParticles[g_i].vz());
            GlobalPoint seed_vtx(genParticles[g_i].vx(), genParticles[g_i].vy(), genParticles[g_i].vz());
            seed_MC_pvd.push_back((seed_vtx-gpvp).mag()); //"write lambda var for gen particle");
            mothers=genParticles[g_i].motherRefVector();

            //mother id

            if (mothers.size()>0 )
            {id_mom = abs(mothers[0]->pdgId());
            id_mom=std::max((id_mom/1000) % 10,(id_mom/100) % 10);}
            else
            id_mom = -1;

            seed_MC_MomFlavour.push_back(id_mom);
            
            if (mothers.size()>0 )
            seed_MC_MomPdgId.push_back(mothers[0]->pdgId());
            else
            seed_MC_MomPdgId.push_back(39);
            //bchain
            
            b_found=0;
            d_found=0;
            while (mothers.size()>0 && b_found==0)
            {
            id_mom = abs(mothers[0]->pdgId());
            id_mom=std::max((id_mom/1000) % 10,(id_mom/100) % 10);
            if (id_mom==5)
            b_found=1;
            if (id_mom==4)
            d_found=1;
            mothers=mothers[0]->motherRefVector();
            }

            seed_MC_BChain.push_back(b_found);
            seed_MC_DChain.push_back(d_found*(!b_found));
            
            
            
            }
            else{
            
            seed_MC_pt.push_back(-100);
            seed_MC_eta.push_back(-100);
            seed_MC_phi.push_back(-100);
            seed_MC_mass.push_back(-100);
            seed_MC_dxy.push_back(-100);
            seed_MC_dz.push_back(-100);
            seed_MC_vx.push_back(-100);
            seed_MC_vy.push_back(-100);
            seed_MC_vz.push_back(-100);
            seed_MC_pvd.push_back(-100);
            seed_MC_MomFlavour.push_back(-100);
            seed_MC_BChain.push_back(-100);
            seed_MC_DChain.push_back(-100);
            seed_MC_MomPdgId.push_back(39);
            }

            
//            int myseed = genLevel(genParticles, *it, seed_MC_MomFlavour, seed_MC_BChain, Match_chisquare);
            
            nearTracks.clear();

            for(std::vector<reco::TransientTrack>::const_iterator tt = selectedTracks.begin();tt!=selectedTracks.end(); ++tt )   {
                VertexDistance3D distanceComputer;
                TwoTrackMinimumDistance dist;
                if(*tt==*it) continue;
                if(dist.calculate(tt->impactPointState(),it->impactPointState())) {
                    GlobalPoint ttPoint          = dist.points().first;
//                    std::cout << ttPoint << std::endl;
                    GlobalError ttPointErr       = tt->impactPointState().cartesianError().position();
                    GlobalPoint seedPosition     = dist.points().second;
                    GlobalError seedPositionErr  = it->impactPointState().cartesianError().position();
                    Measurement1D m = distanceComputer.distance(VertexState(seedPosition,seedPositionErr), VertexState(ttPoint, ttPointErr));
                    GlobalPoint cp(dist.crossingPoint()); 
//                    std::cout << ttPointErr.cxx() << "  errore come fatto  " << std::endl;

                    float PCAseedFromPV =  (dist.points().second-pvp).mag();
                    float PCAtrackFromPV =  (dist.points().first-pvp).mag();
//                    std::cout << distanceFromPV << std::endl;
                    
                    float distance = dist.distance();
//                    float significance = m.significance();
//                    std::cout << distance << std::endl;
//                    std::cout << m.value() << "   " << m.significance() << " m 1d measurement " << std::endl;
                    
                    
                    GlobalVector trackDir2D(tt->impactPointState().globalDirection().x(),tt->impactPointState().globalDirection().y(),0.); 
                    GlobalVector seedDir2D(it->impactPointState().globalDirection().x(),it->impactPointState().globalDirection().y(),0.); 
//                    //SK:UNUSED//    float dotprodTrackSeed2D = trackDir2D.unit().dot(seedDir2D.unit());

                    float dotprodTrack = (dist.points().first-pvp).unit().dot(tt->impactPointState().globalDirection().unit());
                    float dotprodSeed = (dist.points().second-pvp).unit().dot(it->impactPointState().globalDirection().unit());
                    myTrack.set_values(tt->track().pt(), tt->track().eta(), tt->track().phi(),  tt->track().dz(), tt->track().dxy(), distance, 
                    m.significance(), /*put significance etc*/ 
                    
                    seedPosition.x(), seedPosition.y(), seedPosition.z(), seedPositionErr.cxx(), seedPositionErr.cyy(), seedPositionErr.czz(),
                     ttPoint.x(),  ttPoint.y(),  ttPoint.z(),  ttPointErr.cxx(),  ttPointErr.cyy(),  ttPointErr.czz(),
                    dotprodTrack, dotprodSeed
                    
                    );
                    myTrack.set_index(genP_index[tt - selectedTracks.begin()]);
                    myTrack.set_distances(PCAseedFromPV, PCAtrackFromPV);
                    nearTracks.push_back(myTrack);
                     
//                    float w = distanceFromPV*distanceFromPV/(pvDistance*distance);
//                    bool selected = (m.significance() < clusterMaxSignificance && 
//                    dotprodSeed > clusterMinAngleCosine && //Angles between PV-PCAonSeed vectors and seed directions
//                    dotprodTrack > clusterMinAngleCosine && //Angles between PV-PCAonTrack vectors and track directions
//                    //                    dotprodTrackSeed2D > clusterMinAngleCosine && //Angle between track and seed
//                    //      distance*clusterScale*tracks.size() < (distanceFromPV+pvDistance)*(distanceFromPV+pvDistance)/pvDistance && // cut scaling with track density
//                    distance*distanceRatio < distanceFromPV && // cut scaling with track density
//                    distance < clusterMaxDistance);  // absolute distance cut

//                    #ifdef VTXDEBUG
//                    std::cout << tt->trackBaseRef().key() << " :  " << (selected?"+":" ")<< " " << m.significance() << " < " << clusterMaxSignificance <<  " &&  " << 
//                    dotprodSeed  << " > " <<  clusterMinAngleCosine << "  && " << 
//                    dotprodTrack  << " > " <<  clusterMinAngleCosine << "  && " << 
//                    dotprodTrackSeed2D  << " > " <<  clusterMinAngleCosine << "  &&  "  << 
//                    distance*distanceRatio  << " < " <<  distanceFromPV << "  crossingtoPV: " << distanceFromPV << " dis*scal " <<  distance*distanceRatio << "  <  " << distanceFromPV << " dist: " << distance << " < " << clusterMaxDistance <<  std::endl; // cut scaling with track density
//                    #endif           
//                    if(selected)
//                    {
//                    result.push_back(*tt);
//                    seedingPoint = GlobalPoint(cp.x()*w+seedingPoint.x(),cp.y()*w+seedingPoint.y(),cp.z()*w+seedingPoint.z());  
//                    sumWeights+=w; 
//                    }
                }
            }
//            
//            for(unsigned int i=0; i< nearTracks.size(); i++) {
//            std::cout << nearTracks[i].dist << "  "; 
//            }
//            std::cout <<  "  " << std::endl; 
//            
//            for(unsigned int i=0; i< nearTracks.size(); i++) {
//            std::cout << nearTracks[i].dsig << "  "; 
//            }
//            std::cout <<  "  " << std::endl; 
//            
            std::sort (nearTracks.begin(), nearTracks.end(), sortfunction());
//            
//            for(unsigned int i=0; i< nearTracks.size(); i++) {
//            std::cout << nearTracks[i].dist << "  "; 
//            }
            std::cout <<  "  " << std::endl; 
            std::cout <<  "  " << std::endl; 
            nearTracks.resize(20);
            for(unsigned int i=0; i< nearTracks.size(); i++) {
            std::cout << nearTracks[i].dist << "  "; 
            }
            std::cout <<  "  " << std::endl; 
            for(unsigned int i=0; i< nearTracks.size(); i++) {
            std::cout << nearTracks[i].pt << "  ";}
            std::cout <<  "  " << std::endl; 
            
            
            for(unsigned int i=0; i< nearTracks.size(); i++) {
            nearTracks_nTracks.push_back(nearTracks.size());
            nearTracks_Nvtx.push_back(seeds.size()-1);
            nearTracks_pt.push_back(nearTracks[i].pt);
            nearTracks_eta.push_back(nearTracks[i].eta);
            nearTracks_phi.push_back(nearTracks[i].phi);
            nearTracks_dz.push_back(nearTracks[i].dz);
            nearTracks_dxy.push_back(nearTracks[i].dxy);
            nearTracks_PCAdist.push_back(nearTracks[i].dist);
            nearTracks_PCAdsig.push_back(nearTracks[i].dsig);
            nearTracks_PCAonSeed_x.push_back(nearTracks[i].PCA_sx);
            nearTracks_PCAonSeed_y.push_back(nearTracks[i].PCA_sy);
            nearTracks_PCAonSeed_z.push_back(nearTracks[i].PCA_sz);
            nearTracks_PCAonSeed_xerr.push_back(nearTracks[i].PCA_sxerr);
            nearTracks_PCAonSeed_yerr.push_back(nearTracks[i].PCA_syerr);
            nearTracks_PCAonSeed_zerr.push_back(nearTracks[i].PCA_szerr);
            nearTracks_PCAonTrack_x.push_back(nearTracks[i].PCA_tx);
            nearTracks_PCAonTrack_y.push_back(nearTracks[i].PCA_ty);
            nearTracks_PCAonTrack_z.push_back(nearTracks[i].PCA_tz);
            nearTracks_PCAonTrack_xerr.push_back(nearTracks[i].PCA_txerr);
            nearTracks_PCAonTrack_yerr.push_back(nearTracks[i].PCA_tyerr);
            nearTracks_PCAonTrack_zerr.push_back(nearTracks[i].PCA_tzerr);
            nearTracks_dotprodTrack.push_back(nearTracks[i].dotprodTrack);
            nearTracks_dotprodSeed.push_back(nearTracks[i].dotprodSeed);
            nearTracks_PCAonSeed_pvd.push_back(nearTracks[i].seedPCA_pv);
            nearTracks_PCAonTrack_pvd.push_back(nearTracks[i].trackPCA_pv);
            
            if (nearTracks[i].index>0){
            int g_i=nearTracks[i].index;
            nearTracks_MC_pt.push_back(genParticles[g_i].pt());
            nearTracks_MC_eta.push_back(genParticles[g_i].eta());
            nearTracks_MC_phi.push_back(genParticles[g_i].phi());
            nearTracks_MC_dxy.push_back((-genParticles[g_i].vx() * genParticles[g_i].py() + genParticles[g_i].vy() * genParticles[g_i].px()) / genParticles[g_i].pt());
            nearTracks_MC_dz.push_back(genParticles[g_i].vz() - (genParticles[g_i].vx() * genParticles[g_i].px() + genParticles[g_i].vy() * genParticles[g_i].py()) / genParticles[g_i].pt() * (genParticles[g_i].pz() / genParticles[g_i].pt()));
            nearTracks_MC_Track_vx.push_back(genParticles[g_i].vx());
            nearTracks_MC_Track_vy.push_back(genParticles[g_i].vy());
            nearTracks_MC_Track_vz.push_back(genParticles[g_i].vz());
            nearTracks_MC_fromSeedVtx.push_back((genParticles[g_i].vx()==seed_MC_vx[seed_MC_vx.size() - 1])&&(genParticles[g_i].vy()==seed_MC_vy[seed_MC_vy.size() - 1])
            &&(genParticles[g_i].vz()==seed_MC_vz[seed_MC_vz.size() - 1]));
            GlobalPoint track_vtx(genParticles[g_i].vx(), genParticles[g_i].vy(), genParticles[g_i].vz());
            nearTracks_MC_pvd.push_back((track_vtx-gpvp).mag());
            mothers=genParticles[g_i].motherRefVector();

            //mother id

            if (mothers.size()>0 )
            {id_mom = abs(mothers[0]->pdgId());
            id_mom=std::max((id_mom/1000) % 10,(id_mom/100) % 10);}
            else
            id_mom = -1;
            
            nearTracks_MC_MomFlavour.push_back(id_mom);
            
            if (mothers.size()>0 )
            nearTracks_MC_MomPdgId.push_back(mothers[0]->pdgId());
            else
            nearTracks_MC_MomPdgId.push_back(39);
            
            //bchain
            
            b_found=0;
            d_found=0;
            while (mothers.size()>0 && b_found==0)
            {
            id_mom = abs(mothers[0]->pdgId());
            id_mom=std::max((id_mom/1000) % 10,(id_mom/100) % 10);
            if (id_mom==5)
            b_found=1;
            if (id_mom==4)
            d_found=1;
            mothers=mothers[0]->motherRefVector();
            }

            nearTracks_MC_BChain.push_back(b_found);
            nearTracks_MC_DChain.push_back(d_found*(!b_found));
            
            }
            else{
            nearTracks_MC_pt.push_back(-100);
            nearTracks_MC_eta.push_back(-100);
            nearTracks_MC_phi.push_back(-100);
            nearTracks_MC_dxy.push_back(-100);
            nearTracks_MC_dz.push_back(-100);
            nearTracks_MC_Track_vx.push_back(-100);
            nearTracks_MC_Track_vy.push_back(-100);
            nearTracks_MC_Track_vz.push_back(-100);
            nearTracks_MC_MomFlavour.push_back(-100);
            nearTracks_MC_BChain.push_back(-100);
            nearTracks_MC_DChain.push_back(-100);
            nearTracks_MC_fromSeedVtx.push_back(-100);
            nearTracks_MC_MomPdgId.push_back(39);
            nearTracks_MC_pvd.push_back(-100);
            }
            }
            
            
            std::cout << "check the seed again before " << it->track().phi() << " " << it->track().pt() << " " << it->track().eta() << std::endl;
//            seedSisters(genParticles, myseed, nearTracks);
            
            }
            
            
            
            
            
        n_seed=seeds.size();
        }
        }
   tree->Fill();



#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}



int seedAnalyzer::funzionePippo (int vivaLaVita) {
return 1;
}


int  seedAnalyzer::seedSisters(std::vector<reco::GenParticle> genP, int genP_index, std::vector<trackVars> nearTracks)
{

std::cout << "enter the functions" << "  " << genP_index <<std::endl;

edm::RefVector<std::vector<reco::GenParticle> > mothers;
edm::RefVector<std::vector<reco::GenParticle> > sisters;

if (genP_index==-1){
std::cout << "no gen match "  << genP_index <<std::endl;
return 1 ;}

mothers=genP[genP_index].motherRefVector();

//std::cout << "mothers here are" << std::endl;
if (mothers.size()==1){
//std::cout << "get particels from decay" << std::endl;
//std::cout << mothers[0]->numberOfDaughters() << std::endl;
sisters=mothers[0]->daughterRefVector();
//std::cout << "got particels from decay" << std::endl;

std::cout << "the seed found "  << "  "<< genP[genP_index].phi() << " " << genP[genP_index].pt() << " " << genP[genP_index].eta() << " " << genP[genP_index].vx() << " " << genP[genP_index].vy() << " " << genP[genP_index].vz() << " " << std::endl;
for (unsigned int i = 0; i < sisters.size(); i++){

std::cout << "genP sister number  "<< i  << "  "<< sisters[i]->charge() << "  track charged .." << std::endl;
std::cout << "genP sister number  "<< i  << "  "<< sisters[i]->phi() << " " << sisters[i]->pt() << " " << sisters[i]->eta() << " " << sisters[i]->vx() << " " << sisters[i]->vy() << " " << sisters[i]->vz() << " " << std::endl;
}
for (unsigned int i = 0; i < nearTracks.size(); i++){
std::cout << "seed close tracks  "<< i  << "  " << nearTracks[i].phi << " " << nearTracks[i].pt << " " << nearTracks[i].eta << " " << nearTracks[i].PCA_tx << " " << nearTracks[i].PCA_ty << " " << nearTracks[i].PCA_tz << " " << nearTracks[i].dist <<std::endl;
}




}
return 1;

}


double seedAnalyzer::trackEff(std::vector<reco::GenParticle> genP, std::vector<reco::TransientTrack> tracks, double &BChain_eff, double &DChain_eff, double &B_eff,
      double &low_pt_eff, double &high_pt_eff, std::vector<double> &minGenChiSquare){

minGenChiSquare.clear();
int id_mom = -1;
int b_found=0;
int d_found=0;
double eff=0;
int sample_size=0;
int B_sample_size=0;
int BChain_sample_size=0;
int DChain_sample_size=0;
int highpt_sample_size=0;
int lowpt_sample_size=0;
B_eff=0;
BChain_eff=0;
DChain_eff=0;
high_pt_eff=0;
low_pt_eff=0;

if (genP.size()==0)
return 0;

std::vector<trackGenMatch> chi_squares; 
edm::RefVector<std::vector<reco::GenParticle> > mothers; 
trackGenMatch chi_square; 

std::cout << "function entered .." << std::endl;
for (unsigned int g = 0; g < genP.size(); g++){
    chi_squares.clear();
    if (std::fabs(genP[g].eta())<2.4&&genP[g].status()==1&&genP[g].charge()!=0){
        sample_size=sample_size+1;
       std::cout<<"genp checled"<<std::endl;
        double dxy = (-genP[g].vx() * genP[g].py() + genP[g].vy() * genP[g].px()) / genP[g].pt();
        double dsz =  genP[g].vz() * genP[g].pt() / genP[g].p() - (genP[g].vx() * genP[g].px() + genP[g].vy() * genP[g].py()) / genP[g].pt() * genP[g].pz() / genP[g].p();
        
        for(std::vector<reco::TransientTrack>::const_iterator it = tracks.begin(); it != tracks.end(); it++){
        double chi, tot=0;

        chi= (it->track().eta()-genP[g].eta())/(it->track().etaError());
        chi=chi*chi;
        tot=tot+chi;
        chi= (it->track().pt()-genP[g].pt())/(it->track().ptError());
        chi=chi*chi;
        tot=tot+chi;
        chi= (it->track().phi()-genP[g].phi())/(it->track().phiError());
        chi=chi*chi;
        tot=tot+chi;
        chi= (it->track().dsz()-dsz)/(it->track().dszError());
        chi=chi*chi;
        tot=tot+chi;
        chi= (it->track().dxy()-dxy)/(it->track().dxyError());
        chi=chi*chi;
        tot=tot+chi;
        chi_square.set_chi(tot);
//        chi_square.set_GenIndex(g);
        chi_squares.push_back(chi_square);
        
        
        }
    std::cout<<"sorting"<<std::endl;
    if (chi_squares.size()==0)
        return 0;
    std::sort (chi_squares.begin(), chi_squares.end(), sortgen());
    std::cout<<"sorted ok"<<std::endl;

    minGenChiSquare.push_back(chi_squares[0].chi_square);
    mothers=genP[g].motherRefVector();
    
   //bchain

    b_found=0;
    d_found=0;
    while (mothers.size()>0 && b_found==0)
    {
    id_mom = abs(mothers[0]->pdgId());
    id_mom=std::max((id_mom/1000) % 10,(id_mom/100) % 10);
    if (id_mom==5)
    b_found=1;
    if (id_mom==4)
    d_found=1;
    mothers=mothers[0]->motherRefVector();
    }


    //mother id
    mothers=genP[g].motherRefVector();
        
    if (mothers.size()>0 )
    {id_mom = abs(mothers[0]->pdgId());
    id_mom=std::max((id_mom/1000) % 10,(id_mom/100) % 10);}
    else
    id_mom = -1;
            
    
    if (genP[g].pt()<0.5){
    lowpt_sample_size++;
    if (chi_squares[0].chi_square<50)
    low_pt_eff++;
    }
    else{
    highpt_sample_size++;
    if (chi_squares[0].chi_square<50)
    high_pt_eff++; }
    
    if (id_mom==5){
    B_sample_size++;
    if (chi_squares[0].chi_square<50)
    B_eff++;}
    
    if (b_found){
    BChain_sample_size++;
    if (chi_squares[0].chi_square<50)
    BChain_eff++;}
    
    if (d_found){
    DChain_sample_size++;
    if (chi_squares[0].chi_square<50)
    DChain_eff++;}

   
    
    if (chi_squares[0].chi_square<50){
    eff=eff+1;
    }
    
    }
    }


std::cout<<"division"<<std::endl;
B_eff=B_eff/B_sample_size;
BChain_eff=BChain_eff/BChain_sample_size;
DChain_eff=DChain_eff/DChain_sample_size;
high_pt_eff=high_pt_eff/highpt_sample_size;
low_pt_eff=low_pt_eff/lowpt_sample_size;


eff=eff/sample_size;
return eff;
}



int seedAnalyzer::genMap(std::vector<reco::GenParticle> genP, std::vector<reco::TransientTrack> tracks, std::vector<int>& genPnumbers, std::vector<double>& allChi){

std::vector<trackGenMatch> chi_squares;  
trackGenMatch chi_square; 
edm::RefVector<std::vector<reco::GenParticle> > mothers;
edm::RefVector<std::vector<reco::GenParticle> > daughters;
genPnumbers.clear();

for(std::vector<reco::TransientTrack>::const_iterator it = tracks.begin(); it != tracks.end(); it++){
    chi_squares.clear();
    std::cout << "vertex for the track" <<  it->track().vx() <<  " "<< it->track().vy() <<  " " << it->track().vz() << std::endl;
    int j=-1;
    for (unsigned int g = 0; g < genP.size(); g++){

        if (std::fabs(genP[g].eta())<5){
        double dxy = (-genP[g].vx() * genP[g].py() + genP[g].vy() * genP[g].px()) / genP[g].pt();
        double dsz =  genP[g].vz() * genP[g].pt() / genP[g].p() - (genP[g].vx() * genP[g].px() + genP[g].vy() * genP[g].py()) / genP[g].pt() * genP[g].pz() / genP[g].p();

        double chi, tot=0;

        chi= (it->track().eta()-genP[g].eta())/(it->track().etaError());
        chi=chi*chi;
        tot=tot+chi;
        chi= (it->track().pt()-genP[g].pt())/(it->track().ptError());
        chi=chi*chi;
        tot=tot+chi;
        chi= (it->track().phi()-genP[g].phi())/(it->track().phiError());
        chi=chi*chi;
        tot=tot+chi;
        chi= (it->track().dsz()-dsz)/(it->track().dszError());
        chi=chi*chi;
        tot=tot+chi;
        chi= (it->track().dxy()-dxy)/(it->track().dxyError());
        chi=chi*chi;
        tot=tot+chi;
        
        

        chi_square.set_chi(tot);
        chi_square.set_GenIndex(g);
        chi_square.set_numberOfDaughters(genP[g].numberOfDaughters());
        chi_square.set_Status(genP[g].status());
        chi_squares.push_back(chi_square);

        }
    }
    
    std::sort (chi_squares.begin(), chi_squares.end(), sortgen());
    bool to_match=1;
    bool match=0;
    unsigned int i=0;
    while(match==0 and i < chi_squares.size() and to_match)
    {
    if (chi_squares[i].Status==1 && chi_squares[i].chi_square<50) /*detector-stable particle*/{
//    if (chi_squares[i].chi_square<50) /*detector-stable particle*/{
    match=1;
    }
    else if (chi_squares[i].chi_square>50){to_match=0;}
    i++;
    }
    if (match){
    allChi.push_back(chi_squares[i-1].chi_square);
    j=chi_squares[i-1].GenIndex;
    genPnumbers.push_back(j);
    std::cout << " see how the matching went: index  " << chi_squares[i-1].GenIndex << ", chisqaure  " <<  chi_squares[i-1].chi_square<< std::endl;
    std::cout << " a bit of a check if sorting is ok " << chi_squares[i-1].GenIndex << ", chisqaure  " <<  chi_squares[i-1].chi_square
    << ", chisqaure 2 " <<  chi_squares[i].chi_square<< ", chisqaure 3 " <<  chi_squares[i+1].chi_square<< std::endl;}
    
    else{allChi.push_back(chi_squares[0].chi_square);
//    j=chi_squares[i-1].GenIndex;
    genPnumbers.push_back(-1);
    std::cout << " no matching here: index  " << chi_squares[0].GenIndex << ", chisqaure  " <<  chi_squares[0].chi_square<< std::endl;
    std::cout << " a bit of a check if sorting is ok " << chi_squares[0].GenIndex << ", chisqaure  " <<  chi_squares[0].chi_square
    << ", chisqaure 2 " <<  chi_squares[1].chi_square<< ", chisqaure 3 " <<  chi_squares[2].chi_square<< std::endl;
    }
}



return 1;

}






int seedAnalyzer::genLevel(std::vector<reco::GenParticle> genP, reco::TransientTrack seed, std::vector<int>& MomFlav, std::vector<int>& BChain, std::vector<double>& allChi){

std::vector<trackGenMatch> chi_squares;  
trackGenMatch chi_square; 

edm::RefVector<std::vector<reco::GenParticle> > mothers;
edm::RefVector<std::vector<reco::GenParticle> > daughters;

int id, id_mom=-111111, id_dau=-111111;

for (unsigned int i = 0; i < genP.size(); i++)
{

if (fabs(genP[i].eta())<5){
////dxy = (-vx() * py() + vy() * px()) / pt();
////dsz =  vz() * pt() / p() - (vx() * px() + vy() * py()) / pt() * pz() / p();

double dxy = (-genP[i].vx() * genP[i].py() + genP[i].vy() * genP[i].px()) / genP[i].pt();
double dsz =  genP[i].vz() * genP[i].pt() / genP[i].p() - (genP[i].vx() * genP[i].px() + genP[i].vy() * genP[i].py()) / genP[i].pt() * genP[i].pz() / genP[i].p();

//std::cout << dxy << " " << dsz << " " << genP[i].phi() << " " << "PRIMA RIGA" << std::endl;
//std::cout << genP[i].pt() << " " << genP[i].eta() << " " << genP[i].phi() << " " << "2 RIGA" << std::endl;
//std::cout << seed.track().phi() << " " << seed.track().pt() << " " << seed.track().eta() << " " << "3 RIGA" << std::endl;
//std::cout << seed.track().dsz() << " " << seed.track().dxy() << " " << seed.track().eta() << " " << std::endl;
//std::cout << seed.track().phiError() << " " << seed.track().ptError() << " " << seed.track().etaError() << " " << std::endl;
//std::cout << seed.track().dszError() << " " << seed.track().dxyError() << " " << seed.track().etaError() << " " << std::endl;

double chi, tot=0;

chi= (seed.track().eta()-genP[i].eta())/(seed.track().etaError());
chi=chi*chi;
tot=tot+chi;
chi= (seed.track().pt()-genP[i].pt())/(seed.track().ptError());
chi=chi*chi;
tot=tot+chi;
chi= (seed.track().phi()-genP[i].phi())/(seed.track().phiError());
chi=chi*chi;
tot=tot+chi;
chi= (seed.track().dsz()-dsz)/(seed.track().dszError());
chi=chi*chi;
tot=tot+chi;
chi= (seed.track().dxy()-dxy)/(seed.track().dxyError());
chi=chi*chi;
tot=tot+chi;



//std::cout << tot << " chisqaure  "<< std::endl;


chi_square.set_chi(tot);
allChi.push_back(tot);
chi_square.set_GenIndex(i);
chi_square.set_numberOfDaughters(genP[i].numberOfDaughters());
chi_square.set_Status(genP[i].status());


chi_squares.push_back(chi_square);


}
}


std::sort (chi_squares.begin(), chi_squares.end(), sortgen());

bool to_match=1;
bool match=0;
unsigned int i=0; 

while(match==0 and i < chi_squares.size() and to_match)
{
if (chi_squares[i].Status==1) /*detector-stable particle*/{
match=1;

}

std::cout << i << " while here" << match << "  " << chi_squares[i].Status << " "  << chi_squares[i].GenIndex   << std::endl;
i++;
}

std::cout << i-1 << " end of while" << std::endl;
i=chi_squares[i-1].GenIndex;
std::cout << i-1 << " reset while" << std::endl;

daughters=genP[i-1].daughterRefVector();
mothers=genP[i-1].motherRefVector();

id = genP[i-1].pdgId();

//mother id

if (mothers.size()>0 )
{id_mom = mothers[0]->pdgId();
id_mom=std::max((id_mom/1000) % 10,(id_mom/100) % 10);}
else
id_mom = -1;

MomFlav.push_back(id_mom);
//mother id



if (daughters.size()>0 )
id_dau = daughters[0]->pdgId();
else
id_dau = -111111111;

std::cout << id << " " << id_mom << " " << id_dau << " " << std::endl;
std::cout << mothers.size() << " " << daughters.size() << " "  << " " << std::endl;
std::cout << genP[i].status() << " " << std::endl;

//b in chain
int b_found=0;
while (mothers.size()>0 && b_found==0)
{
id_mom = mothers[0]->pdgId();
id_mom=std::max((id_mom/1000) % 10,(id_mom/100) % 10);
if (id_mom==5)
b_found=1;
mothers=mothers[0]->motherRefVector();
}

BChain.push_back(b_found);

if (match)
return i;
else
return -1;

}



// ------------ method called once each job just before starting event loop  ------------
void 
seedAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
seedAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
seedAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(seedAnalyzer);
