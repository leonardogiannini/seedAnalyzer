#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/CandidatePtrTransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/PatternTools/interface/TwoTrackMinimumDistance.h"
#include <DataFormats/Candidate/interface/Candidate.h>
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "seedAnalyzer/seedAnalyzer/interface/trackVars2.h"


#include "RecoVertex/AdaptiveVertexFinder/interface/TracksClusteringFromDisplacedSeed.h"
#include "RecoBTag/LWTNN/interface/LightweightNeuralNetwork.h"
#include "RecoBTag/LWTNN/interface/parse_json.h" 
#include <fstream>
#include <iostream>
#include "seedAnalyzer/seedAnalyzer/interface/trackVars2.h"


#ifndef NNet_compute_h
#define NNet_compute_h

class TrackTag {

    public:
//         std::vector<reco::TransientTrack> tracks;    
//         std::vector<reco::JetFlavourInfoMatchingCollection> jetMC;
        std::vector<float> compute_result(std::vector<reco::TransientTrack> selectedTracks,reco::Candidate* Jet, reco::Vertex pv);
        std::vector<float> compute_result_dir(std::vector<reco::TransientTrack> selectedTracks,GlobalVector direction, reco::Vertex pv);
        std::vector<float> computeNN_result(std::vector<reco::TransientTrack> selectedTracks,reco::JetFlavourInfoMatchingCollection jetMC,reco::Vertex pv, std::vector<float> masses);
        int seedToJetMatching(reco::TransientTrack seed, reco::JetFlavourInfoMatchingCollection jetMC);
        TrackTag();
        
    private:


        lwt::LightweightNeuralNetwork           *BDchainClassweighted_NN;
        lwt::LightweightNeuralNetwork           *BDchain_NN;
        lwt::LightweightNeuralNetwork           *SameSV_NN;
        lwt::LightweightNeuralNetwork           *SameVtx_NN;
        lwt::LightweightNeuralNetwork           *Seed_BDChain_NN;
        lwt::LightweightNeuralNetwork           *Seed_Bweighted_NN;
        lwt::JSONConfig                         config_TrackPair; 
        lwt::JSONConfig                         config_Seed; 
        lwt::JSONConfig                         config_Seed4; 
        
};


#endif



