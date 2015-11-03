import FWCore.ParameterSet.Config as cms

process = cms.Process("HIPAnalysis")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("EmptySource",
#process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
#    fileNames = cms.untracked.vstring(
#        '/store/group/dpg_tracker_strip/comm_tracker/Strip/Calibration/calibrationtree/GR15/calibTree_247243.root',
#        'file:myfile.root'
#    )
)

process.HIPAnalysis = cms.EDAnalyzer('SiStripHIPAnalysis',
    InputFiles = cms.untracked.vstring(
        'root://cmsxrootd.fnal.gov//store/group/dpg_tracker_strip/comm_tracker/Strip/Calibration/calibrationtree/GR15/calibTree_247243.root',
        ),
#    , tracks = cms.untracked.InputTag('ctfWithMaterialTracks')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo.root')
 )

process.p = cms.Path(process.HIPAnalysis)
