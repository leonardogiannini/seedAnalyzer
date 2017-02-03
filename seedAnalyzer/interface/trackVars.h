class trackVars {
      public:
    double pt, eta, phi, dz, dxy,  dist, dsig;
    double mass, t3Dip, t3Dsip, t2Dip, t2Dsip;
    double PCA_sx, PCA_sy, PCA_sz, PCA_sxerr, PCA_syerr, PCA_szerr;
    double PCA_tx, PCA_ty, PCA_tz, PCA_txerr, PCA_tyerr, PCA_tzerr;
    double dotprodTrack, dotprodSeed;
    double dotprodTrackSeed2D, dotprodTrackSeed3D;
    double dotprodTrackSeedVectors2D, dotprodTrackSeedVectors3D;
    double seedPCA_pv, trackPCA_pv;
    
    int index;
    
    void set_values (double, double, double, double,  double, double, double,
    double, double, double, double,  double, double,
    double, double, double, double,  double, double,
    double, double
    );
    
    void set_vars (double, double, double, double,
    double, double, double, double, double
    );
    
    void set_index ( int );
    void set_distances ( double, double );

};

void trackVars::set_values (double pt2, double eta2, double phi2, double dz2, double dxy2, double distaaa, double dsig2,
double PCA_sx2, double PCA_sy2, double PCA_sz2, double PCA_sxerr2, double PCA_syerr2, double PCA_szerr2, 
double PCA_tx2, double PCA_ty2, double PCA_tz2, double PCA_txerr2, double PCA_tyerr2, double PCA_tzerr2,
double dotprodTrack2, double dotprodSeed2) {

    pt=pt2;
    eta=eta2;
    phi=phi2; 
    dz=dz2; 
    dxy=dxy2; 
    dist=distaaa;
    dsig=dsig2;
    PCA_sx=PCA_sx2;
    PCA_sy=PCA_sy2;
    PCA_sz=PCA_sz2; 
    PCA_sxerr=PCA_sxerr2;
    PCA_syerr=PCA_syerr2;
    PCA_szerr=PCA_szerr2;
    PCA_tx=PCA_tx2;
    PCA_ty=PCA_ty2;
    PCA_tz=PCA_tz2; 
    PCA_txerr=PCA_txerr2;
    PCA_tyerr=PCA_tyerr2;
    PCA_tzerr=PCA_tzerr2;
    dotprodTrack=dotprodTrack2;
    dotprodSeed=dotprodSeed2;
    
//    std::cout << "filling   "<< pt << " " << eta << " " << phi << " " << dz << " " << dxy << " " << dist << " " << dsig << " " << std::endl;
//    std::cout << "filling   "<< PCA_sx << " " << PCA_sy << " " << PCA_sz << " " << PCA_tx << " " << PCA_ty << " " << PCA_tz << " "  << std::endl;
//    std::cout << "filling   "<< PCA_sxerr << " " << PCA_syerr << " " << PCA_szerr << " " << PCA_txerr << " " << PCA_tyerr << " " << PCA_tzerr << " "  << std::endl;
//        std::cout << "filling " << dotprodTrack << "  " << dotprodSeed << std::endl;
}

void trackVars::set_vars ( double m, double t2dip, double t2dsip, double t3dip, double t3dsip, double t2dTS, double t3dTS, double t2dTSV, double t3dTSV){
mass=m;
t3Dip=t3dip;
t3Dsip=t3dsip;
t2Dip=t2dip;
t2Dsip=t2dsip;
dotprodTrackSeed2D=t2dTS;
dotprodTrackSeed3D=t3dTS;
dotprodTrackSeedVectors2D=t2dTSV;
dotprodTrackSeedVectors3D=t3dTSV;


}



void trackVars::set_index ( int a){
index=a;
}

void trackVars::set_distances ( double a, double b){
seedPCA_pv=a;
trackPCA_pv=b;
}

struct sortfunction
{
    inline bool operator() (const trackVars& struct1, const trackVars& struct2)
    {
        return (struct1.dist < struct2.dist);
    }
};


class trackGenMatch {
      public:
    double chi_square;
    int numberOfDaughters;
    int MomFlav;
    int BChain;
    int GenIndex;
    int Status;
    
    void set_chi ( double );
    void set_numberOfDaughters ( int );
    void set_MomFlav ( int );
    void set_BChain ( int );
    void set_GenIndex ( int );
    void set_Status ( int );
};


void trackGenMatch::set_chi ( double a){
chi_square=a;
}

void trackGenMatch::set_numberOfDaughters ( int a){
numberOfDaughters=a;
}

void trackGenMatch::set_MomFlav ( int a){
MomFlav=a;
}

void trackGenMatch::set_BChain ( int a){
BChain=a;
}

void trackGenMatch::set_GenIndex ( int a){
GenIndex=a;
}

void trackGenMatch::set_Status ( int a){
Status=a;
}


struct sortgen
{
    inline bool operator() (const trackGenMatch& struct1, const trackGenMatch& struct2)
    {
        return (struct1.chi_square < struct2.chi_square);
    }
};
