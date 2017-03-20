#include "seedAnalyzer/seedAnalyzer/interface/NNet_compute.h"

TrackTag::TrackTag() :

        BDchainClassweighted_NN(NULL),
        BDchain_NN(NULL),
        SameSV_NN(NULL),
        SameVtx_NN(NULL),
        Seed_BDChain_NN(NULL),
        Seed_Bweighted_NN(NULL)
{
    

std::ifstream jsonfile1("/scratch/lgiannini/CMSSW_8_1_0_pre16/src/seedAnalyzer/seedAnalyzer/JsonNN/BDchain_classweighted_NN.json");
config_TrackPair = lwt::parse_json(jsonfile1);
BDchainClassweighted_NN = new lwt::LightweightNeuralNetwork(config_TrackPair.inputs, config_TrackPair.layers, config_TrackPair.outputs); 
std::cout <<"init class  BD w"<<std::endl;

std::ifstream jsonfile2("/scratch/lgiannini/CMSSW_8_1_0_pre16/src/seedAnalyzer/seedAnalyzer/JsonNN/BDchain_NN.json");
config_TrackPair = lwt::parse_json(jsonfile2);
BDchain_NN = new lwt::LightweightNeuralNetwork(config_TrackPair.inputs, config_TrackPair.layers, config_TrackPair.outputs); 
std::cout <<"init class BD "<<std::endl;

std::ifstream jsonfile3("/scratch/lgiannini/CMSSW_8_1_0_pre16/src/seedAnalyzer/seedAnalyzer/JsonNN/SameSV_NN.json");
config_TrackPair = lwt::parse_json(jsonfile3);
SameSV_NN = new lwt::LightweightNeuralNetwork(config_TrackPair.inputs, config_TrackPair.layers, config_TrackPair.outputs); 
std::cout <<"init class SameSV"<<std::endl;

std::ifstream jsonfile4("/scratch/lgiannini/CMSSW_8_1_0_pre16/src/seedAnalyzer/seedAnalyzer/JsonNN/SameVtx_NN.json");
config_TrackPair = lwt::parse_json(jsonfile4);
SameVtx_NN = new lwt::LightweightNeuralNetwork(config_TrackPair.inputs, config_TrackPair.layers, config_TrackPair.outputs); 
std::cout <<"init class SameVTx"<<std::endl;

std::ifstream jsonfile5("/scratch/lgiannini/CMSSW_8_1_0_pre16/src/seedAnalyzer/seedAnalyzer/JsonNN/JsonBDC.json");
config_Seed = lwt::parse_json(jsonfile5);
Seed_BDChain_NN = new lwt::LightweightNeuralNetwork(config_Seed.inputs, config_Seed.layers, config_Seed.outputs); 
std::cout <<"init class seed"<<std::endl;

// std::ifstream jsonfile6("/scratch/lgiannini/CMSSW_8_1_0_pre16/src/seedAnalyzer/seedAnalyzer/JsonNN/seed_train_Bweighted_NN.json");
// config_Seed4 = lwt::parse_json(jsonfile6);
// Seed_Bweighted_NN = new lwt::LightweightNeuralNetwork(config_Seed4.inputs, config_Seed4.layers, config_Seed4.outputs); 
// std::cout <<"init class seed 4"<<std::endl;

}

std::vector<float> TrackTag::computeNN_result(std::vector<reco::TransientTrack> selectedTracks, reco::JetFlavourInfoMatchingCollection jetMC, reco::Vertex pv, std::vector<float> masses)
{   
    std::vector<float> TagResult;    
    std::cout<< "computNN" << std::endl;
    
    std::vector<trackVars2> nearTracks;
    trackVars2 myTrack;

    std::vector<double> all_vars;
    std::vector<double> vars;
    std::vector<double> pv_vars;
    std::vector<double> seed_vars;
    std::vector<double> track_vars;
    std::vector<reco::TransientTrack> seeds; 
    std::cout << "seeds" << std::endl;
    lwt::ValueMap inputs;
    
    pv_vars.push_back(pv.x());
    pv_vars.push_back(pv.y());
    pv_vars.push_back(pv.z());
    GlobalPoint pvp(pv.x(),pv.y(),pv.z());

    for(std::vector<reco::TransientTrack>::const_iterator it = selectedTracks.begin(); it != selectedTracks.end(); it++){


        std::pair<bool,Measurement1D> ip = IPTools::absoluteImpactParameter3D(*it,pv);        
        std::pair<bool,Measurement1D> ip2d = IPTools::absoluteTransverseImpactParameter(*it,pv);

        

    if(ip.first && ip.second.value() >= 0.005 && ip.second.significance() >= 1.2 &&
        ip.second.value() <= 1000 && ip.second.significance() <= 1000 /*&&
        ip2d.second.value() >= min3DIPValue && ip2d.second.significance() >= min3DIPSignificance &&
        ip2d.second.value() <= max3DIPValue && ip2d.second.significance() <= max3DIPSignificance*/
        
        )
            { 
                
            seed_vars.clear();
            all_vars.clear();
            
            std::cout << "new seed : 2d+3d sign." << ip2d.second.significance() << "  " << ip.second.significance() <<"  "<< std::endl;
            seeds.push_back(*it);
            
            seed_vars.push_back(it->track().pt());
            seed_vars.push_back(it->track().eta());
            seed_vars.push_back(it->track().phi());
            seed_vars.push_back(masses[it-selectedTracks.begin()]);//mass            
            seed_vars.push_back(it->track().dz());
            seed_vars.push_back(it->track().dxy());
            seed_vars.push_back(ip.second.value());
            seed_vars.push_back(ip.second.significance());
            seed_vars.push_back(ip2d.second.value());
            seed_vars.push_back(ip2d.second.significance());
            
            int seed_jet_match_i=seedToJetMatching(*it, jetMC);            
                      
            seed_vars.push_back(it->track().normalizedChi2());
            seed_vars.push_back(it->track().hitPattern().numberOfValidPixelHits());
            seed_vars.push_back(it->track().hitPattern().numberOfValidHits());  
            
            if(seed_jet_match_i>=0){
                
            GlobalVector direction(jetMC[seed_jet_match_i].first->px(),jetMC[seed_jet_match_i].first->py(),jetMC[seed_jet_match_i].first->pz()); 
            std::pair<double, Measurement1D> jet_distance =IPTools::jetTrackDistance(*it, direction, pv);
            seed_vars.push_back(std::fabs(jet_distance.second.value())); 
            std::cout<<jet_distance.second.value()<<"      distance"<<jet_distance.second.significance()<<std::endl;
            
            TrajectoryStateOnSurface closest = IPTools::closestApproachToJet(it->impactPointState(),pv, direction,it->field());
            if (closest.isValid()) seed_vars.push_back((closest.globalPosition() - pvp).mag()); 
            else seed_vars.push_back(-99); 
            
            }
            
            else{seed_vars.push_back(-100.); seed_vars.push_back(-100.);  }
            
            nearTracks.clear();
           
            for(std::vector<reco::TransientTrack>::const_iterator tt = selectedTracks.begin();tt!=selectedTracks.end(); ++tt )   {
            VertexDistance3D distanceComputer;
            TwoTrackMinimumDistance dist;
            if(*tt==*it) continue;
            if(dist.calculate(tt->impactPointState(),it->impactPointState())) {
                GlobalPoint ttPoint          = dist.points().first;
                GlobalError ttPointErr       = tt->impactPointState().cartesianError().position();
                GlobalPoint seedPosition     = dist.points().second;
                GlobalError seedPositionErr  = it->impactPointState().cartesianError().position();
                Measurement1D m = distanceComputer.distance(VertexState(seedPosition,seedPositionErr), VertexState(ttPoint, ttPointErr));
                GlobalPoint cp(dist.crossingPoint()); 


                float PCAseedFromPV =  (dist.points().second-pvp).mag();
                float PCAtrackFromPV =  (dist.points().first-pvp).mag();                    
                float distance = dist.distance();
                
                
                GlobalVector trackDir2D(tt->impactPointState().globalDirection().x(),tt->impactPointState().globalDirection().y(),0.); 
                GlobalVector seedDir2D(it->impactPointState().globalDirection().x(),it->impactPointState().globalDirection().y(),0.); 
                GlobalVector trackPCADir2D(dist.points().first.x()-pvp.x(),dist.points().first.y()-pvp.y(),0.); 
                GlobalVector seedPCADir2D(dist.points().second.x()-pvp.x(),dist.points().second.y()-pvp.y(),0.); 


                float dotprodTrack = (dist.points().first-pvp).unit().dot(tt->impactPointState().globalDirection().unit());
                float dotprodSeed = (dist.points().second-pvp).unit().dot(it->impactPointState().globalDirection().unit());
                
                float dotprodTrackSeed2D = trackDir2D.unit().dot(seedDir2D.unit());
                float dotprodTrackSeed3D = it->impactPointState().globalDirection().unit().dot(tt->impactPointState().globalDirection().unit());
                float dotprodTrackSeed2DV = trackPCADir2D.unit().dot(seedPCADir2D.unit());
                float dotprodTrackSeed3DV = (dist.points().second-pvp).unit().dot((dist.points().first-pvp).unit());
                
                std::pair<bool,Measurement1D> t_ip = IPTools::absoluteImpactParameter3D(*tt,pv);        
                std::pair<bool,Measurement1D> t_ip2d = IPTools::absoluteTransverseImpactParameter(*tt,pv);
                
                
                myTrack.set_values(tt->track().pt(), tt->track().eta(), tt->track().phi(),  tt->track().dz(), tt->track().dxy(), distance, 
                m.significance(), /*put significance etc*/ 
                
                seedPosition.x(), seedPosition.y(), seedPosition.z(), seedPositionErr.cxx(), seedPositionErr.cyy(), seedPositionErr.czz(),
                    ttPoint.x(),  ttPoint.y(),  ttPoint.z(),  ttPointErr.cxx(),  ttPointErr.cyy(),  ttPointErr.czz(),
                dotprodTrack, dotprodSeed
                
                );
                
                myTrack.set_distances(PCAseedFromPV, PCAtrackFromPV);
                myTrack.set_vars(masses[tt-selectedTracks.begin()], t_ip2d.second.value() , t_ip2d.second.significance(),
                t_ip.second.value() , t_ip.second.significance(), dotprodTrackSeed2D, dotprodTrackSeed3D, dotprodTrackSeed2DV, dotprodTrackSeed3DV );
                nearTracks.push_back(myTrack);

            }
        }
        
        std::sort (nearTracks.begin(), nearTracks.end(), sortfunction2());

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

        track_vars.clear();
        track_vars.push_back(nearTracks[i].pt);
        track_vars.push_back(nearTracks[i].eta);
        track_vars.push_back(nearTracks[i].phi);
        track_vars.push_back(nearTracks[i].dz);
        track_vars.push_back(nearTracks[i].dxy);
        track_vars.push_back(nearTracks[i].mass);
        track_vars.push_back(nearTracks[i].t3Dip);
        track_vars.push_back(nearTracks[i].t3Dsip);
        track_vars.push_back(nearTracks[i].t2Dip);
        track_vars.push_back(nearTracks[i].t2Dsip);
        track_vars.push_back(nearTracks[i].dist);
        track_vars.push_back(nearTracks[i].dsig);
        track_vars.push_back(nearTracks[i].PCA_sx);
        track_vars.push_back(nearTracks[i].PCA_sy);
        track_vars.push_back(nearTracks[i].PCA_sz);
        track_vars.push_back(nearTracks[i].PCA_sxerr);
        track_vars.push_back(nearTracks[i].PCA_syerr);
        track_vars.push_back(nearTracks[i].PCA_szerr);
        track_vars.push_back(nearTracks[i].PCA_tx);
        track_vars.push_back(nearTracks[i].PCA_ty);
        track_vars.push_back(nearTracks[i].PCA_tz);
        track_vars.push_back(nearTracks[i].PCA_txerr);
        track_vars.push_back(nearTracks[i].PCA_tyerr);
        track_vars.push_back(nearTracks[i].PCA_tzerr);
        track_vars.push_back(nearTracks[i].dotprodTrack);
        track_vars.push_back(nearTracks[i].dotprodSeed);
        track_vars.push_back(nearTracks[i].dotprodTrackSeed2D);
        track_vars.push_back(nearTracks[i].dotprodTrackSeed3D);
        track_vars.push_back(nearTracks[i].dotprodTrackSeedVectors2D);
        track_vars.push_back(nearTracks[i].dotprodTrackSeedVectors3D);
        track_vars.push_back(nearTracks[i].seedPCA_pv);
        track_vars.push_back(nearTracks[i].trackPCA_pv);

        vars.clear();
//         std::cout << vars.size() <<"  size  "<<  std::endl; 
        vars.insert(vars.end(), pv_vars.begin(), pv_vars.end());
//         std::cout << vars.size() <<"  size  "<<  std::endl; 
        vars.insert(vars.end(), seed_vars.begin(), seed_vars.begin()+10);
//         std::cout << vars.size() <<"  size  "<<  std::endl; 
        vars.insert(vars.end(), track_vars.begin(), track_vars.end());
//         std::cout << vars.size() <<"  size  "<<  std::endl; 
        vars.insert(vars.end(), seed_vars.begin()+10, seed_vars.end());
//         std::cout << vars.size() <<"  size  "<<  std::endl;

        int count=0;
        inputs.clear();
        for(auto& input : config_TrackPair.inputs) {	
//                 std::cout << input.name <<"  "<<  vars[count] <<std::endl;                
                inputs[input.name] = vars[count]; 
                count++;
            }
            
        lwt::ValueMap nnout;
        
        double out1;
        double out2;
        double out3;
        double out4;

        nnout = BDchainClassweighted_NN->compute(inputs);//          
        for (auto& x: nnout){ out1=x.second; }
        nnout = BDchain_NN->compute(inputs);//          
        for (auto& x: nnout){ out2=x.second; }
        nnout = SameSV_NN->compute(inputs);//          
        for (auto& x: nnout){ out3=x.second; }
        nnout = SameVtx_NN->compute(inputs);//          
        for (auto& x: nnout){ out4=x.second; }

        vars.push_back(out4);
        vars.push_back(out1);
        vars.push_back(out3);
        vars.push_back(out2);
        std::cout << out4 <<"  "<< out1 <<"  "<< out3 <<"  "<< out2 <<"  "<<  std::endl;
        all_vars.insert(all_vars.end(), vars.begin(), vars.end());
        std::cout << all_vars.size() <<"  size  "<<  std::endl;
        vars.clear();


        }
        
    int count=0;
    inputs.clear();
    for(auto& input : config_Seed.inputs) {	
        std::cout << input.name <<"  "<<  all_vars[count] <<std::endl;                
            inputs[input.name] = all_vars[count]; 
        std::cout << inputs[input.name]  <<"  "<<  all_vars[count] <<std::endl;    
            count++;
        }
     
     double out5;
//      out5=9.3;
     lwt::ValueMap nnout;
//     
     nnout = Seed_BDChain_NN->compute(inputs);//          
     for (auto& x: nnout){ out5=x.second; }
     std::cout<<out5 << "   out5   " <<std::endl;

     TagResult.push_back(out5);//theresult
            
            }
    else TagResult.push_back(-10.); //NO seed category
        
    }

return TagResult;    
}    


int TrackTag::seedToJetMatching (reco::TransientTrack seed, reco::JetFlavourInfoMatchingCollection jetMC)
{
  float angular_distance =  0.4;
  float dist_temp;
  int index = -1;
   for (reco::JetFlavourInfoMatchingCollection::const_iterator iter = jetMC.begin(); iter != jetMC.end(); ++iter) {

      
     dist_temp=std::sqrt(std::pow(iter->first->eta()-seed.track().eta(),2) + std::pow(iter->first->phi()-seed.track().phi(),2) );
     if (dist_temp<angular_distance) {
         
         angular_distance=dist_temp;
         index=iter-jetMC.begin();
    }
      
}
std::cout<<angular_distance<<"  index  "<<index<<std::endl;
return index;
}


//here jet matching already done
std::vector<float> TrackTag::compute_result(std::vector<reco::TransientTrack> selectedTracks,reco::Candidate* Jet, reco::Vertex pv)
{   
    std::vector<float> TagResult;    
    std::cout<< "computNN" << std::endl;
    std::vector<trackVars2> nearTracks;
    trackVars2 myTrack;

    std::vector<double> all_vars;
    std::vector<double> vars;
    std::vector<double> pv_vars;
    std::vector<double> seed_vars;
    std::vector<double> track_vars;
    std::cout << "seeds" << std::endl;
    lwt::ValueMap inputs;
    
    pv_vars.push_back(pv.x());
    pv_vars.push_back(pv.y());
    pv_vars.push_back(pv.z());
    GlobalPoint pvp(pv.x(),pv.y(),pv.z());

    for(std::vector<reco::TransientTrack>::const_iterator it = selectedTracks.begin(); it != selectedTracks.end(); it++){


        std::pair<bool,Measurement1D> ip = IPTools::absoluteImpactParameter3D(*it,pv);        
        std::pair<bool,Measurement1D> ip2d = IPTools::absoluteTransverseImpactParameter(*it,pv);       
        std::cout << "test" <<std::endl;

        if(ip.first && ip.second.value() >= 0.005 && ip.second.significance() >= 1.2 &&
        ip.second.value() <= 1000 && ip.second.significance() <= 1000 /*&&
        ip2d.second.value() >= min3DIPValue && ip2d.second.significance() >= min3DIPSignificance &&
        ip2d.second.value() <= max3DIPValue && ip2d.second.significance() <= max3DIPSignificance*/)
            { 
            
            seed_vars.clear();
            all_vars.clear();
            
            std::cout << "new seed : 2d+3d sign." << ip2d.second.significance() << "  " << ip.second.significance() <<"  "<< std::endl;
                      
            seed_vars.push_back(it->track().pt());
            seed_vars.push_back(it->track().eta());
            seed_vars.push_back(it->track().phi());
            seed_vars.push_back(0);//masses[it-selectedTracks.begin()]);//mass            
            seed_vars.push_back(it->track().dz());
            seed_vars.push_back(it->track().dxy());
            seed_vars.push_back(ip.second.value());
            seed_vars.push_back(ip.second.significance());
            seed_vars.push_back(ip2d.second.value());
            seed_vars.push_back(ip2d.second.significance());
            seed_vars.push_back(it->track().normalizedChi2());
            seed_vars.push_back(it->track().hitPattern().numberOfValidPixelHits());
            seed_vars.push_back(it->track().hitPattern().numberOfValidHits());  
            
            GlobalVector direction(Jet->px(),Jet->py(),Jet->pz());            
            std::pair<double, Measurement1D> jet_distance =IPTools::jetTrackDistance(*it, direction, pv);
            std::cout<<jet_distance.second.value()<<"      distance"<<jet_distance.second.significance()<<std::endl;
            TrajectoryStateOnSurface closest = IPTools::closestApproachToJet(it->impactPointState(),pv, direction,it->field());
            
            seed_vars.push_back(std::fabs(jet_distance.second.value()));           
            if (closest.isValid()) seed_vars.push_back((closest.globalPosition() - pvp).mag()); 
            else seed_vars.push_back(-99);          
            
            nearTracks.clear();
                        
            
            for(std::vector<reco::TransientTrack>::const_iterator tt = selectedTracks.begin();tt!=selectedTracks.end(); ++tt )   {
                VertexDistance3D distanceComputer;
                TwoTrackMinimumDistance dist;
                if(*tt==*it) continue;
                if(dist.calculate(tt->impactPointState(),it->impactPointState())) {
                    
                    GlobalPoint ttPoint          = dist.points().first;
                    GlobalError ttPointErr       = tt->impactPointState().cartesianError().position();
                    GlobalPoint seedPosition     = dist.points().second;
                    GlobalError seedPositionErr  = it->impactPointState().cartesianError().position();
                    Measurement1D m = distanceComputer.distance(VertexState(seedPosition,seedPositionErr), VertexState(ttPoint, ttPointErr));
                    GlobalPoint cp(dist.crossingPoint()); 


                    float PCAseedFromPV =  (dist.points().second-pvp).mag();
                    float PCAtrackFromPV =  (dist.points().first-pvp).mag();                    
                    float distance = dist.distance();
                    
                    
                    GlobalVector trackDir2D(tt->impactPointState().globalDirection().x(),tt->impactPointState().globalDirection().y(),0.); 
                    GlobalVector seedDir2D(it->impactPointState().globalDirection().x(),it->impactPointState().globalDirection().y(),0.); 
                    GlobalVector trackPCADir2D(dist.points().first.x()-pvp.x(),dist.points().first.y()-pvp.y(),0.); 
                    GlobalVector seedPCADir2D(dist.points().second.x()-pvp.x(),dist.points().second.y()-pvp.y(),0.); 


                    float dotprodTrack = (dist.points().first-pvp).unit().dot(tt->impactPointState().globalDirection().unit());
                    float dotprodSeed = (dist.points().second-pvp).unit().dot(it->impactPointState().globalDirection().unit());
                    
                    float dotprodTrackSeed2D = trackDir2D.unit().dot(seedDir2D.unit());
                    float dotprodTrackSeed3D = it->impactPointState().globalDirection().unit().dot(tt->impactPointState().globalDirection().unit());
                    float dotprodTrackSeed2DV = trackPCADir2D.unit().dot(seedPCADir2D.unit());
                    float dotprodTrackSeed3DV = (dist.points().second-pvp).unit().dot((dist.points().first-pvp).unit());
                    
                    std::pair<bool,Measurement1D> t_ip = IPTools::absoluteImpactParameter3D(*tt,pv);        
                    std::pair<bool,Measurement1D> t_ip2d = IPTools::absoluteTransverseImpactParameter(*tt,pv);
                    
                    
                    myTrack.set_values(tt->track().pt(), tt->track().eta(), tt->track().phi(),  tt->track().dz(), tt->track().dxy(), distance, 
                    m.significance(), /*put significance etc*/                     
                    seedPosition.x(), seedPosition.y(), seedPosition.z(), seedPositionErr.cxx(), seedPositionErr.cyy(), seedPositionErr.czz(), 
                    ttPoint.x(),  ttPoint.y(),  ttPoint.z(),  ttPointErr.cxx(),  ttPointErr.cyy(),  ttPointErr.czz(), dotprodTrack, dotprodSeed);
                    
                    myTrack.set_distances(PCAseedFromPV, PCAtrackFromPV);
                    myTrack.set_vars(0. /*masses[tt-selectedTracks.begin()],*/, t_ip2d.second.value() , t_ip2d.second.significance(),
                    t_ip.second.value() , t_ip.second.significance(), dotprodTrackSeed2D, dotprodTrackSeed3D, dotprodTrackSeed2DV, dotprodTrackSeed3DV );
                    nearTracks.push_back(myTrack);

                    }
                }
            
            std::sort (nearTracks.begin(), nearTracks.end(), sortfunction2());

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
            
            track_vars.clear();
            track_vars.push_back(nearTracks[i].pt);
            track_vars.push_back(nearTracks[i].eta);
            track_vars.push_back(nearTracks[i].phi);
            track_vars.push_back(nearTracks[i].dz);
            track_vars.push_back(nearTracks[i].dxy);
            track_vars.push_back(nearTracks[i].mass);
            track_vars.push_back(nearTracks[i].t3Dip);
            track_vars.push_back(nearTracks[i].t3Dsip);
            track_vars.push_back(nearTracks[i].t2Dip);
            track_vars.push_back(nearTracks[i].t2Dsip);
            track_vars.push_back(nearTracks[i].dist);
            track_vars.push_back(nearTracks[i].dsig);
            track_vars.push_back(nearTracks[i].PCA_sx);
            track_vars.push_back(nearTracks[i].PCA_sy);
            track_vars.push_back(nearTracks[i].PCA_sz);
            track_vars.push_back(nearTracks[i].PCA_sxerr);
            track_vars.push_back(nearTracks[i].PCA_syerr);
            track_vars.push_back(nearTracks[i].PCA_szerr);
            track_vars.push_back(nearTracks[i].PCA_tx);
            track_vars.push_back(nearTracks[i].PCA_ty);
            track_vars.push_back(nearTracks[i].PCA_tz);
            track_vars.push_back(nearTracks[i].PCA_txerr);
            track_vars.push_back(nearTracks[i].PCA_tyerr);
            track_vars.push_back(nearTracks[i].PCA_tzerr);
            track_vars.push_back(nearTracks[i].dotprodTrack);
            track_vars.push_back(nearTracks[i].dotprodSeed);
            track_vars.push_back(nearTracks[i].dotprodTrackSeed2D);
            track_vars.push_back(nearTracks[i].dotprodTrackSeed3D);
            track_vars.push_back(nearTracks[i].dotprodTrackSeedVectors2D);
            track_vars.push_back(nearTracks[i].dotprodTrackSeedVectors3D);
            track_vars.push_back(nearTracks[i].seedPCA_pv);
            track_vars.push_back(nearTracks[i].trackPCA_pv);

            
            vars.clear();
            std::cout << vars.size() <<"  size  "<<  std::endl; 
            vars.insert(vars.end(), pv_vars.begin(), pv_vars.end());
            std::cout << vars.size() <<"  size  "<<  std::endl; 
            vars.insert(vars.end(), seed_vars.begin(), seed_vars.begin()+10);
            std::cout << vars.size() <<"  size  "<<  std::endl; 
            vars.insert(vars.end(), track_vars.begin(), track_vars.end());
            std::cout << vars.size() <<"  size  "<<  std::endl; 
            vars.insert(vars.end(), seed_vars.begin()+10, seed_vars.end());
            std::cout << vars.size() <<"  size  "<<  std::endl; 

            int count=0;
            inputs.clear();
            for(auto& input : config_TrackPair.inputs) {	
                    std::cout << input.name <<"  "<<  vars[count] <<std::endl;                
                    inputs[input.name] = vars[count]; 
                    count++;
                }
                
            lwt::ValueMap nnout;
            
            double out1;
            double out2;
            double out3;
            double out4;

            nnout = BDchainClassweighted_NN->compute(inputs);//          
            for (auto& x: nnout){ out1=x.second; }
            nnout = BDchain_NN->compute(inputs);//          
            for (auto& x: nnout){ out2=x.second; }
            nnout = SameSV_NN->compute(inputs);//          
            for (auto& x: nnout){ out3=x.second; }
            nnout = SameVtx_NN->compute(inputs);//          
            for (auto& x: nnout){ out4=x.second; }

            vars.push_back(out1);
            vars.push_back(out2);
            vars.push_back(out3);
            vars.push_back(out4);

            all_vars.insert(all_vars.end(), vars.begin(), vars.end());
            vars.clear();


            }
            
            int count=0;
            inputs.clear();
            for(auto& input : config_Seed.inputs) {                
                    std::cout << input.name <<"  "<<  all_vars[count] <<std::endl;                
                    inputs[input.name] = all_vars[count]; 
                    count++;
                    }
            
            double out5;

            lwt::ValueMap nnout;     
            nnout = Seed_BDChain_NN->compute(inputs);          
            for (auto& x: nnout){ out5=x.second; }
            std::cout<<out5<<std::endl;

            
            TagResult.push_back(out5);//theresult
            
            }
        else TagResult.push_back(-10.); //NO seed category
        
    }

    return TagResult;    
}


//another version


std::vector<float> TrackTag::compute_result_dir(std::vector<reco::TransientTrack> selectedTracks,GlobalVector direction, reco::Vertex pv)
{   
    std::vector<float> TagResult;    
    std::cout<< "computNN" << std::endl;
    std::vector<trackVars2> nearTracks;
    trackVars2 myTrack;

    std::vector<double> all_vars;
    std::vector<double> vars;
    std::vector<double> pv_vars;
    std::vector<double> seed_vars;
    std::vector<double> track_vars;
    std::cout << "seeds" << std::endl;
    lwt::ValueMap inputs;
    
    pv_vars.push_back(pv.x());
    pv_vars.push_back(pv.y());
    pv_vars.push_back(pv.z());
    GlobalPoint pvp(pv.x(),pv.y(),pv.z());

    for(std::vector<reco::TransientTrack>::const_iterator it = selectedTracks.begin(); it != selectedTracks.end(); it++){


        std::pair<bool,Measurement1D> ip = IPTools::absoluteImpactParameter3D(*it,pv);        
        std::pair<bool,Measurement1D> ip2d = IPTools::absoluteTransverseImpactParameter(*it,pv);       
        std::cout << "test" <<std::endl;

        if(ip.first && ip.second.value() >= 0.005 && ip.second.significance() >= 1.2 &&
        ip.second.value() <= 1000 && ip.second.significance() <= 1000 /*&&
        ip2d.second.value() >= min3DIPValue && ip2d.second.significance() >= min3DIPSignificance &&
        ip2d.second.value() <= max3DIPValue && ip2d.second.significance() <= max3DIPSignificance*/)
            { 
            
            seed_vars.clear();
            all_vars.clear();
            
            std::cout << "new seed : 2d+3d sign." << ip2d.second.significance() << "  " << ip.second.significance() <<"  "<< std::endl;
                      
            seed_vars.push_back(it->track().pt());
            seed_vars.push_back(it->track().eta());
            seed_vars.push_back(it->track().phi());
            seed_vars.push_back(0);//masses[it-selectedTracks.begin()]);//mass            
            seed_vars.push_back(it->track().dz());
            seed_vars.push_back(it->track().dxy());
            seed_vars.push_back(ip.second.value());
            seed_vars.push_back(ip.second.significance());
            seed_vars.push_back(ip2d.second.value());
            seed_vars.push_back(ip2d.second.significance());
            seed_vars.push_back(it->track().normalizedChi2());
            seed_vars.push_back(it->track().hitPattern().numberOfValidPixelHits());
            seed_vars.push_back(it->track().hitPattern().numberOfValidHits());  
            
//             GlobalVector direction(Jet->px(),Jet->py(),Jet->pz());            
            std::pair<double, Measurement1D> jet_distance =IPTools::jetTrackDistance(*it, direction, pv);
            std::cout<<jet_distance.second.value()<<"      distance"<<jet_distance.second.significance()<<std::endl;
            TrajectoryStateOnSurface closest = IPTools::closestApproachToJet(it->impactPointState(),pv, direction,it->field());
            
            seed_vars.push_back(std::fabs(jet_distance.second.value()));           
            if (closest.isValid()) seed_vars.push_back((closest.globalPosition() - pvp).mag()); 
            else seed_vars.push_back(-99);          
            
            nearTracks.clear();
                        
            
            for(std::vector<reco::TransientTrack>::const_iterator tt = selectedTracks.begin();tt!=selectedTracks.end(); ++tt )   {
                VertexDistance3D distanceComputer;
                TwoTrackMinimumDistance dist;
                if(*tt==*it) continue;
                if(dist.calculate(tt->impactPointState(),it->impactPointState())) {
                    
                    GlobalPoint ttPoint          = dist.points().first;
                    GlobalError ttPointErr       = tt->impactPointState().cartesianError().position();
                    GlobalPoint seedPosition     = dist.points().second;
                    GlobalError seedPositionErr  = it->impactPointState().cartesianError().position();
                    Measurement1D m = distanceComputer.distance(VertexState(seedPosition,seedPositionErr), VertexState(ttPoint, ttPointErr));
                    GlobalPoint cp(dist.crossingPoint()); 


                    float PCAseedFromPV =  (dist.points().second-pvp).mag();
                    float PCAtrackFromPV =  (dist.points().first-pvp).mag();                    
                    float distance = dist.distance();
                    
                    
                    GlobalVector trackDir2D(tt->impactPointState().globalDirection().x(),tt->impactPointState().globalDirection().y(),0.); 
                    GlobalVector seedDir2D(it->impactPointState().globalDirection().x(),it->impactPointState().globalDirection().y(),0.); 
                    GlobalVector trackPCADir2D(dist.points().first.x()-pvp.x(),dist.points().first.y()-pvp.y(),0.); 
                    GlobalVector seedPCADir2D(dist.points().second.x()-pvp.x(),dist.points().second.y()-pvp.y(),0.); 


                    float dotprodTrack = (dist.points().first-pvp).unit().dot(tt->impactPointState().globalDirection().unit());
                    float dotprodSeed = (dist.points().second-pvp).unit().dot(it->impactPointState().globalDirection().unit());
                    
                    float dotprodTrackSeed2D = trackDir2D.unit().dot(seedDir2D.unit());
                    float dotprodTrackSeed3D = it->impactPointState().globalDirection().unit().dot(tt->impactPointState().globalDirection().unit());
                    float dotprodTrackSeed2DV = trackPCADir2D.unit().dot(seedPCADir2D.unit());
                    float dotprodTrackSeed3DV = (dist.points().second-pvp).unit().dot((dist.points().first-pvp).unit());
                    
                    std::pair<bool,Measurement1D> t_ip = IPTools::absoluteImpactParameter3D(*tt,pv);        
                    std::pair<bool,Measurement1D> t_ip2d = IPTools::absoluteTransverseImpactParameter(*tt,pv);
                    
                    
                    myTrack.set_values(tt->track().pt(), tt->track().eta(), tt->track().phi(),  tt->track().dz(), tt->track().dxy(), distance, 
                    m.significance(), /*put significance etc*/                     
                    seedPosition.x(), seedPosition.y(), seedPosition.z(), seedPositionErr.cxx(), seedPositionErr.cyy(), seedPositionErr.czz(), 
                    ttPoint.x(),  ttPoint.y(),  ttPoint.z(),  ttPointErr.cxx(),  ttPointErr.cyy(),  ttPointErr.czz(), dotprodTrack, dotprodSeed);
                    
                    myTrack.set_distances(PCAseedFromPV, PCAtrackFromPV);
                    myTrack.set_vars(0. /*masses[tt-selectedTracks.begin()],*/, t_ip2d.second.value() , t_ip2d.second.significance(),
                    t_ip.second.value() , t_ip.second.significance(), dotprodTrackSeed2D, dotprodTrackSeed3D, dotprodTrackSeed2DV, dotprodTrackSeed3DV );
                    nearTracks.push_back(myTrack);

                    }
                }
            
            std::sort (nearTracks.begin(), nearTracks.end(), sortfunction2());

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
            
            track_vars.clear();
            track_vars.push_back(nearTracks[i].pt);
            track_vars.push_back(nearTracks[i].eta);
            track_vars.push_back(nearTracks[i].phi);
            track_vars.push_back(nearTracks[i].dz);
            track_vars.push_back(nearTracks[i].dxy);
            track_vars.push_back(nearTracks[i].mass);
            track_vars.push_back(nearTracks[i].t3Dip);
            track_vars.push_back(nearTracks[i].t3Dsip);
            track_vars.push_back(nearTracks[i].t2Dip);
            track_vars.push_back(nearTracks[i].t2Dsip);
            track_vars.push_back(nearTracks[i].dist);
            track_vars.push_back(nearTracks[i].dsig);
            track_vars.push_back(nearTracks[i].PCA_sx);
            track_vars.push_back(nearTracks[i].PCA_sy);
            track_vars.push_back(nearTracks[i].PCA_sz);
            track_vars.push_back(nearTracks[i].PCA_sxerr);
            track_vars.push_back(nearTracks[i].PCA_syerr);
            track_vars.push_back(nearTracks[i].PCA_szerr);
            track_vars.push_back(nearTracks[i].PCA_tx);
            track_vars.push_back(nearTracks[i].PCA_ty);
            track_vars.push_back(nearTracks[i].PCA_tz);
            track_vars.push_back(nearTracks[i].PCA_txerr);
            track_vars.push_back(nearTracks[i].PCA_tyerr);
            track_vars.push_back(nearTracks[i].PCA_tzerr);
            track_vars.push_back(nearTracks[i].dotprodTrack);
            track_vars.push_back(nearTracks[i].dotprodSeed);
            track_vars.push_back(nearTracks[i].dotprodTrackSeed2D);
            track_vars.push_back(nearTracks[i].dotprodTrackSeed3D);
            track_vars.push_back(nearTracks[i].dotprodTrackSeedVectors2D);
            track_vars.push_back(nearTracks[i].dotprodTrackSeedVectors3D);
            track_vars.push_back(nearTracks[i].seedPCA_pv);
            track_vars.push_back(nearTracks[i].trackPCA_pv);

            
            vars.clear();
            std::cout << vars.size() <<"  size  "<<  std::endl; 
            vars.insert(vars.end(), pv_vars.begin(), pv_vars.end());
            std::cout << vars.size() <<"  size  "<<  std::endl; 
            vars.insert(vars.end(), seed_vars.begin(), seed_vars.begin()+10);
            std::cout << vars.size() <<"  size  "<<  std::endl; 
            vars.insert(vars.end(), track_vars.begin(), track_vars.end());
            std::cout << vars.size() <<"  size  "<<  std::endl; 
            vars.insert(vars.end(), seed_vars.begin()+10, seed_vars.end());
            std::cout << vars.size() <<"  size  "<<  std::endl; 

            int count=0;
            inputs.clear();
            for(auto& input : config_TrackPair.inputs) {	
                    std::cout << input.name <<"  "<<  vars[count] <<std::endl;                
                    inputs[input.name] = vars[count]; 
                    count++;
                }
                
            lwt::ValueMap nnout;
            
            double out1;
            double out2;
            double out3;
            double out4;

            nnout = BDchainClassweighted_NN->compute(inputs);//          
            for (auto& x: nnout){ out1=x.second; }
            nnout = BDchain_NN->compute(inputs);//          
            for (auto& x: nnout){ out2=x.second; }
            nnout = SameSV_NN->compute(inputs);//          
            for (auto& x: nnout){ out3=x.second; }
            nnout = SameVtx_NN->compute(inputs);//          
            for (auto& x: nnout){ out4=x.second; }

            vars.push_back(out1);
            vars.push_back(out2);
            vars.push_back(out3);
            vars.push_back(out4);

            all_vars.insert(all_vars.end(), vars.begin(), vars.end());
            vars.clear();


            }
            
            int count=0;
            inputs.clear();
            for(auto& input : config_Seed.inputs) {                
                    std::cout << input.name <<"  "<<  all_vars[count] <<std::endl;                
                    inputs[input.name] = all_vars[count]; 
                    count++;
                    }
            
            double out5;

            lwt::ValueMap nnout;     
            nnout = Seed_BDChain_NN->compute(inputs);          
            for (auto& x: nnout){ out5=x.second; }
            std::cout<<out5<<std::endl;

            
            TagResult.push_back(out5);//theresult
            
            }
        else TagResult.push_back(-10.); //NO seed category
        
    }

    return TagResult;    
}