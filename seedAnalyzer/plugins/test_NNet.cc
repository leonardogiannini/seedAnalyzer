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
#include "seedAnalyzer/seedAnalyzer/interface/trackVars2.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "TrackingTools/GeomPropagators/interface/AnalyticalImpactPointExtrapolator.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertToFromReco.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"


#include "seedAnalyzer/seedAnalyzer/interface/NNet_compute.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs. 
class test_NNet : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit test_NNet(const edm::ParameterSet&);
      ~test_NNet();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      // ----------member data ---------------------------
      
//       int funzionePippo (int vivaLaVita);
      
      edm::Service<TFileService> file;
     
      TTree *tree;
      
      int evt=0;
      int lumi=0;
      int run=0;
      int min3DIPValue=0;
      int min3DIPSignificance=2;
      int max3DIPValue=100;
      int max3DIPSignificance=10;
//       std::vector<double> seed_3D_ip;
//       std::vector<double> seed_3D_sip;
      std::vector<float> res;

      
      TrackTag tagger;
      
      
      edm::EDGetTokenT<edm::View<reco::Candidate> > CandidateToken;
      edm::EDGetTokenT<std::vector<reco::GenParticle> > genParticleToken;

      edm::EDGetTokenT<reco::VertexCollection> token_primaryVertex;
      edm::EDGetTokenT<reco::BeamSpot> token_beamSpot;
      edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> jetToken;
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
test_NNet::test_NNet(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
    usesResource("TFileService");
    tree=file->make<TTree>("tree","tree");
    
    tree->Branch("lumi",&lumi, "lumi/I");
    tree->Branch("evt",&evt, "evt/I");
    tree->Branch("run",&run, "run/I");  
/*    
    tree->Branch("seed_3D_ip",&seed_3D_ip);
    tree->Branch("seed_3D_sip",&seed_3D_sip);*/
    tree->Branch("seed_Test",&res);
    
    CandidateToken = consumes<edm::View<reco::Candidate>>(edm::InputTag("particleFlow"));
    token_primaryVertex = consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
    token_beamSpot = consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));    
    genParticleToken = consumes<std::vector<reco::GenParticle> >(edm::InputTag("genParticles"));
    jetToken = consumes<reco::JetFlavourInfoMatchingCollection>(iConfig.getParameter<edm::InputTag>("jetMCSrc"));
}


test_NNet::~test_NNet()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}



//
// member functions
//

// ------------ method called for each event  ------------
void
test_NNet::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

    evt=iEvent.id().event();
    lumi=iEvent.id().luminosityBlock();
    run=iEvent.id().run();
    
    
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
    
        
    edm::Handle<reco::JetFlavourInfoMatchingCollection> jetMC;    
    iEvent.getByToken(jetToken, jetMC);
   
//   std::cout << tracks->size() << std::endl;
   
    if(primaryVertices->size()!=0){
        const reco::Vertex &pv = (*primaryVertices)[0];
        GlobalPoint pvp(pv.x(),pv.y(),pv.z());

        
        std::cout << "check the primary  " << std::endl;
        std::cout <<" reco -->" << pv.x() << ", "<< pv.x() << ", "<< pv.x() <<std::endl;
        
        for (reco::JetFlavourInfoMatchingCollection::const_iterator iter = jetMC->begin(); iter != jetMC->end(); ++iter) {
            unsigned int fl = std::abs(iter->second.getPartonFlavour());
            std::cout<< "pt: " <<iter->first->pt()<< "eta: " << iter->first->eta()<< "phi: " << iter->first->phi()<< "m: " << iter->first->mass() << std::endl;               
            std::cout<< fl <<std::endl;
            }
        

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
            if((*tracks)[k].bestTrack() != 0 && std::fabs(pv.z()-(*tracks)[k].vz())<0.1  && (*tracks)[k].pt()>0.5) {
            selectedTracks.push_back(trackBuilder->build(tracks->ptrAt(k)));
            masses.push_back(tracks->ptrAt(k)->mass());}
            }
        
        res.clear();
        res = tagger.computeNN_result(selectedTracks, *jetMC, pv, masses);
        
    }
   tree->Fill();
}
            
            
            
void 
test_NNet::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
test_NNet::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
test_NNet::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(test_NNet);

            
            
            