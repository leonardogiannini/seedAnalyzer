from ROOT import *
import numpy

import sys

print sys.argv[1]

f1=TFile.Open("../tree_test_somefiles.root")
tree=f1.Get("analyzer1/tree")
#vertexCandidate_array
seeds=[]
print tree.GetEntries()
for ev in range(int(sys.argv[1]), int(sys.argv[1])+200):
    print ev
    tree.GetEntry(ev)
    if tree.n_seed:
        seen_tracks=0
        print tree.n_seed
        for j in range(tree.n_seed):
            #print tree.n_iv[0]    
#            vertexCandidate_array=[]
            print j, "vertex_index", seen_tracks
            #seen_tracks=0
            for n in range(j*20, (j+1)*20):
                #print j, n, "nseed",  tree.n_seed, tree.nearTracks_Nvtx[n], len(tree.nearTracks_MC_fromSeedVtx), tree.nearTracks_nTracks[n], tree.seed_pt[j], tree.pv_x
                #print tree.seed_MC_vx[j], tree.seed_MC_vy[j], tree.seed_MC_vz[j], "yes no?", tree.nearTracks_MC_fromSeedVtx[n], "PCA distance ", tree.nearTracks_PCAonSeed_pvd[n],  tree.nearTracks_PCAonTrack_pvd[n]
#                print tree.nearTracks_MC_Track_vx[n], tree.nearTracks_MC_Track_vy[n], tree.nearTracks_MC_Track_vz[n], "yes no?", tree.nearTracks_MC_fromSeedVtx[n]
                track_array=numpy.array([
                
                tree.pv_x,      #0
                tree.pv_y,      #1
                tree.pv_z,      #2
                tree.seed_pt[j],    #3
                tree.seed_eta[j],    #4
                tree.seed_phi[j],    #5
                tree.seed_mass[j],    #6
                tree.seed_dz[j],    #7
                tree.seed_dxy[j],    #8
                tree.seed_3D_ip[j],    #9
                tree.seed_3D_sip[j],    #10
                tree.seed_2D_ip[j],    #11
                tree.seed_2D_sip[j],    #12
                
                tree.nearTracks_pt[n],    #13
                tree.nearTracks_eta[n],    #14
                tree.nearTracks_phi[n],    #15
                tree.nearTracks_dz[n],    #16
                tree.nearTracks_dxy[n],    #17
                tree.nearTracks_PCAdist[n],    #18
                tree.nearTracks_PCAdsig[n],    #19
                tree.nearTracks_PCAonSeed_x[n],    #20
                tree.nearTracks_PCAonSeed_y[n],    #21
                tree.nearTracks_PCAonSeed_z[n],    #22
                tree.nearTracks_PCAonSeed_xerr[n],    #23
                tree.nearTracks_PCAonSeed_yerr[n],    #24
                tree.nearTracks_PCAonSeed_zerr[n],    #25
                tree.nearTracks_PCAonTrack_x[n],    #26
                tree.nearTracks_PCAonTrack_y[n],    #27
                tree.nearTracks_PCAonTrack_z[n],    #28
                tree.nearTracks_PCAonTrack_xerr[n],    #29
                tree.nearTracks_PCAonTrack_yerr[n],    #30
                tree.nearTracks_PCAonTrack_zerr[n],    #31
                tree.nearTracks_dotprodTrack[n],    #32
                tree.nearTracks_dotprodSeed[n],     #33
                
                tree.seed_MC_pt[j],    #34
                tree.seed_MC_eta[j],    #35
                tree.seed_MC_phi[j],    #36
                tree.seed_MC_mass[j],    #37
                tree.seed_MC_dz[j],    #38
                tree.seed_MC_dxy[j],    #39
                tree.seed_MC_MomFlavour[j],    #40
                tree.seed_MC_MomPdgId[j],    #41
                tree.seed_MC_BChain[j],    #42
                tree.seed_MC_DChain[j],    #43
                tree.seed_MC_vx[j],    #44
                tree.seed_MC_vy[j],    #45
                tree.seed_MC_vz[j],    #46
                
                tree.nearTracks_PCAonSeed_pvd[n],       #47
                tree.nearTracks_PCAonTrack_pvd[n],    #48
                tree.nearTracks_MC_pt[n],    #49
                tree.nearTracks_MC_eta[n],    #50
                tree.nearTracks_MC_phi[n],    #51
                tree.nearTracks_MC_dz[n],    #52
                tree.nearTracks_MC_dxy[n],    #53
                tree.nearTracks_MC_MomFlavour[n],    #54
                tree.nearTracks_MC_MomPdgId[n],    #55
                tree.nearTracks_MC_BChain[n],    #56
                tree.nearTracks_MC_DChain[n],    #57
                tree.nearTracks_MC_Track_vx[n],    #58
                tree.nearTracks_MC_Track_vy[n],    #59
                tree.nearTracks_MC_Track_vz[n],    #60
                tree.nearTracks_MC_fromSeedVtx[n],    #61
                
                tree.nearTracks_MC_pvd[n],    #62
                tree.seed_MC_pvd[j]    #63
                
                ])
                
                lista=[
                
                "tree.pv_x",
                "tree.pv_y",
                "tree.pv_z",
                "tree.seed_pt[j]",
                "tree.seed_eta[j]",
                "tree.seed_phi[j]",
                "tree.seed_mass[j]",
                "tree.seed_dz[j]",
                "tree.seed_dxy[j]",
                "tree.seed_3D_ip[j]",
                "tree.seed_3D_sip[j]",
                "tree.seed_2D_ip[j]",
                "tree.seed_2D_sip[j]",
                
                "tree.nearTracks_pt[n]",
                "tree.nearTracks_eta[n]",
                "tree.nearTracks_phi[n]",
                "tree.nearTracks_dz[n]",
                "tree.nearTracks_dxy[n]",
                "tree.nearTracks_PCAdist[n]",
                "tree.nearTracks_PCAdsig[n]",
                "tree.nearTracks_PCAonSeed_x[n]",
                "tree.nearTracks_PCAonSeed_y[n]",
                "tree.nearTracks_PCAonSeed_z[n]",
                "tree.nearTracks_PCAonSeed_xerr[n]",
                "tree.nearTracks_PCAonSeed_yerr[n]",
                "tree.nearTracks_PCAonSeed_zerr[n]",
                "tree.nearTracks_PCAonTrack_x[n]",
                "tree.nearTracks_PCAonTrack_y[n]",
                "tree.nearTracks_PCAonTrack_z[n]",
                "tree.nearTracks_PCAonTrack_xerr[n]",
                "tree.nearTracks_PCAonTrack_yerr[n]",
                "tree.nearTracks_PCAonTrack_zerr[n]",
                "tree.nearTracks_dotprodTrack[n]",
                "tree.nearTracks_dotprodSeed[n]",
                
                "tree.seed_MC_pt[j]",
                "tree.seed_MC_eta[j]",
                "tree.seed_MC_phi[j]",
                "tree.seed_MC_mass[j]",
                "tree.seed_MC_dz[j]",
                "tree.seed_MC_dxy[j]",
                "tree.seed_MC_MomFlavour[j]",
                "tree.seed_MC_MomPdgId[j]",
                "tree.seed_MC_BChain[j]",
                "tree.seed_MC_DChain[j]",
                "tree.seed_MC_vx[j]",
                "tree.seed_MC_vy[j]",
                "tree.seed_MC_vz[j]",
                
                "tree.nearTracks_PCAonSeed_pvd[n]",
                "tree.nearTracks_PCAonTrack_pvd[n]",
                "tree.nearTracks_MC_pt[n]",
                "tree.nearTracks_MC_eta[n]",
                "tree.nearTracks_MC_phi[n]",
                "tree.nearTracks_MC_dz[n]",
                "tree.nearTracks_MC_dxy[n]",
                "tree.nearTracks_MC_MomFlavour[n]",
                "tree.nearTracks_MC_MomPdgId[n]",
                "tree.nearTracks_MC_BChain[n]",
                "tree.nearTracks_MC_DChain[n]",
                "tree.nearTracks_MC_Track_vx[n]",
                "tree.nearTracks_MC_Track_vy[n]",
                "tree.nearTracks_MC_Track_vz[n]",
                "tree.nearTracks_MC_fromSeedVtx[n]",
                
                "target",
                "target"
                ]
                
                
                
                if (tree.nearTracks_MC_fromSeedVtx[n]==1 and tree.nearTracks_MC_Track_vz[n]!=tree.gen_pv_z and tree.nearTracks_MC_BChain[n]==0 and tree.nearTracks_MC_DChain[n]==0):
                    print "event===", ev, " seed===", j , " of ", tree.n_seed
                    print "\n"
                    for i in range(len(lista)):
                        print  lista[i], track_array[i]
                    
                try:
                    vertexCandidate_array=numpy.vstack((vertexCandidate_array, track_array))
                except:
                    vertexCandidate_array=track_array
#                .append(track_array)
#                vertexCandidate_array=numpy.array(vertexCandidate_array)
#            vertexCandidate_array=numpy.array(vertexCandidate_array)
#            print vertexCandidate_array.shape
#            seeds.append(vertexCandidate_array)

#seeds=numpy.array(seeds)
numpy.save("track_by_track"+sys.argv[1]+".npy", vertexCandidate_array)

print vertexCandidate_array.shape



