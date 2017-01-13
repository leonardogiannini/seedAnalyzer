import FWCore.ParameterSet.Config as cms

process = cms.Process("Pippo")

#process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.load('Configuration/StandardSequences/GeometryDB_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/EventContent/EventContent_cff')
process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_v3'


process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
    
    
#       '/store/relval/CMSSW_8_1_0_pre15/RelValTTbar_13/GEN-SIM-RECO/81X_upgrade2017_design_IdealBS_v1_2017design-v1/10000/646AAE5F-E59B-E611-B0AD-0CC47A4C8E64.root'
#        'file:../../../ZeroBias_Jul2016.root'
        #'file:file_QCD_Pt470to600.root'
        #'file:file_QCD_Pt120to170.root'
        #'file:file_QCD_Pt1800to2400.root'
        'file:/scratch/lgiannini/FAE96376-83B7-E611-8DAB-0CC47A4D7674.root'
        
#        'root://xrootd-cms.infn.it//store/relval/CMSSW_8_1_0_pre16/RelValTTbar_13/GEN-SIM-RECO/PU25ns_81X_upgrade2017_realistic_v22_HS-v1/10000/007A655B-7FB7-E611-A316-0025905B858C.root'
#        'root://xrootd-cms.infn.it//store/mc/RunIISpring16MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8-evtgen/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/02EA5736-2C3F-E611-AAD2-0CC47A4D99A4.root'
    
    )#,
    #skipEvents=cms.untracked.uint32(36590)
)

#process.testEff = cms.EDAnalyzer('TestEfficiencyAnalyzer',
#    bits = cms.InputTag("TriggerResults","","HLT"),
#    prescales = cms.InputTag("patTrigger"),
#    objects = cms.InputTag("selectedPatTrigger"),
#    generatorInfo = cms.InputTag("generator"),
#)


from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

## b-tag discriminators
bTagDiscriminators = [
    'pfTrackCountingHighEffBJetTags',
    'pfTrackCountingHighPurBJetTags',
    'pfJetProbabilityBJetTags',
    'pfJetBProbabilityBJetTags',
    'pfSimpleSecondaryVertexHighEffBJetTags',
    'pfSimpleSecondaryVertexHighPurBJetTags',
    'pfCombinedSecondaryVertexV2BJetTags',
    'pfCombinedInclusiveSecondaryVertexV2BJetTags',
    'pfCombinedMVAV2BJetTags'
]

from PhysicsTools.PatAlgos.tools.jetTools import *
## Update the slimmedJets in miniAOD: corrections from the chosen Global Tag are applied and the b-tag discriminators are re-evaluated
updateJetCollection(
    process,
    jetSource = cms.InputTag('slimmedJets'),
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators = bTagDiscriminators,
   # svSource=cms.InputTag('inclusiveCandidateSecondaryVerticesMini')
)
updateJetCollection(
    process,
    labelName="reivfJets",
    jetSource = cms.InputTag('slimmedJets'),
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators = bTagDiscriminators,
    svSource=cms.InputTag('inclusiveCandidateSecondaryVerticesMini')
)

from RecoVertex.AdaptiveVertexFinder.inclusiveVertexing_cff import *
process.inclusiveCandidateVertexFinderMini= inclusiveCandidateVertexFinder.clone(
   primaryVertices='offlineSlimmedPrimaryVertices',
   tracks='packedPFCandidates'
)

process.candidateVertexMergerMini = candidateVertexMerger.clone(
   secondaryVertices = cms.InputTag("inclusiveCandidateVertexFinderMini"),
)

process.candidateVertexArbitratorMini = candidateVertexArbitrator.clone(
   secondaryVertices = cms.InputTag("candidateVertexMergerMini"),
   primaryVertices='offlineSlimmedPrimaryVertices',
   tracks='packedPFCandidates',
   trackMinLayers=0
)

process.inclusiveCandidateSecondaryVerticesMini = candidateVertexMerger.clone(
   secondaryVertices = cms.InputTag("candidateVertexArbitratorMini"),
)

process.redoIVF = cms.Sequence(process.inclusiveCandidateVertexFinderMini*
                               process.candidateVertexMergerMini*
                               process.candidateVertexArbitratorMini*
                               process.inclusiveCandidateSecondaryVerticesMini)


process.analyzer1 = cms.EDAnalyzer('seedAnalyzer',
#    bits = cms.InputTag("TriggerResults","","HLT"),
#    prescales = cms.InputTag("patTrigger"),
#    objects = cms.InputTag("selectedPatTrigger"),
#    generatorInfo = cms.InputTag("generator"),
)

outfile='tree_prova1500.root'

process.TFileService = cms.Service("TFileService",
	fileName = cms.string(outfile) )

#fare IVF o no
#process.p = cms.Path(process.redoIVF+process.analyzer1)

process.p = cms.Path(process.analyzer1)


