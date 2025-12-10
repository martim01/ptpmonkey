#pragma once

#include <cstdint>

namespace pml::ptpmonkey
{
    namespace hdr
    {
        enum class enumType{kSync=0x0, kDelayReq=0x1, kPeerDelayReq = 0x2, kPeerDelayResp = 0x3, kFollowUp=0x8, kDelayResp=0x9, kPeerDelayRespFollowUp = 0xA, kAnnounce=0xB, kSignalling=0xC, kManagement=0xD};

        enum class enumFlags{ kAlternateMaster = 0x0100,
                            kTwoStep         = 0x0200,
                            kUnicast          = 0x0400,
                            kProfile1         = 0x2000,
                            kProfile2         = 0x4000,
                            kSecurity         = 0x8000,
                            kLI_61            = 0x0001,
                            kLI_59            = 0x0002,
                            kUtcOffsetValid = 0x0004,
                            kTimescale        = 0x0008,
                            kTimeTraceable   = 0x0010,
                            kFreqTraceable   = 0x0020 };

    }

    namespace mngmnt
    {
        enum class enumType {kManagement = 0x0001, kManagementErrorStatus = 0x0002, kOrganizationExtension = 0x0003, kRequestUnicastTransmission = 0x0004, kGrantUnicastTransmission = 0x0005, kCancelUnicastTransmission = 0x0006, kAcknowledgeCancelUnicastTransmission = 0x0007, kPathTrace = 0x0008, kAlternateTimeOffsetIndicator = 0x0009, kAuthentication2008 = 0x2000, kAuthenticationChallenge = 0x2001, KSecurityAssociationUpdate = 0x2002, kCumFreqScaleFactorOffset = 0x2003, kPtpmonReq = 0x21fe, kPtpmonResp = 0x21ff, kOrganizationExtensionPropagate = 0x4000, kEnhancedAccuracyMetrics = 0x4001, kOrganizationExtensionDoNotPropagate = 0x8000, kL1Sync = 0x8001, kPortCommunicationAvailability = 0x8002, kProtocolAddress = 0x8003, kSlaveRxSyncTimingData = 0x8004, kSlaveRxSyncComputedData = 0x8005, kSlaveTxEventTimestamps = 0x8006, kSlaveDelayTimingDataNp = 0x7f00, kCumulativeRateRatio = 0x8007, kPad = 0x8008, kAuthentication = 0x8009 };
        
        enum class enumId : std::uint16_t {KError                                =0,
                                           kNullManagement                      = 0x0000,
                                           kClockDescription                    = 0x0001,
                                           kUserDescription                     = 0x0002,
                                           kSaveInNonVolatileStorage         = 0x0003,
                                           kResetNonVolatileStorage           = 0x0004,
                                           kInitialize                           = 0x0005,
                                           kFaultLog                            = 0x0006,
                                           kFaultLogReset                      = 0x0007,
                                           kDefaultDataSet                     = 0x2000,
                                           kCurrentDataSet                     = 0x2001,
                                           kParentDataSet                      = 0x2002,
                                           kTimePropertiesDataSet             = 0x2003,
                                           kPortDataSet                        = 0x2004,
                                           kPriority1                            = 0x2005,
                                           kPriority2                            = 0x2006,
                                           kDomain                               = 0x2007,
                                           kSlaveOnly                           = 0x2008,
                                           kLogAnnounceInterval                = 0x2009,
                                           kAnnounceReceiptTimeout             = 0x200a,
                                           kLogSyncInterval                    = 0x200b,
                                           kVersionNumber                       = 0x200c,
                                           kEnablePort                          = 0x200d,
                                           kDisablePort                         = 0x200e,
                                           kTime                                 = 0x200f,
                                           kClockAccuracy                       = 0x2010,
                                           kUtcProperties                       = 0x2011,
                                           kTraceabilityProperties              = 0x2012,
                                           kTimescaleProperties                 = 0x2013,
                                           kUnicastNegotiationEnable           = 0x2014,
                                           kPathTraceList                      = 0x2015,
                                           kPathTraceEnable                    = 0x2016,
                                           kGrandmasterClusterTable            = 0x2017,
                                           kUnicastMasterTable                 = 0x2018,
                                           kUnicastMasterMaxTableSize        = 0x2019,
                                           kAcceptableMasterTable              = 0x201a,
                                           kAcceptableMasterTableEnabled      = 0x201b,
                                           kAcceptableMasterMaxTableSize     = 0x201c,
                                           kAlternateMaster                     = 0x201d,
                                           kAlternateTimeOffsetEnable         = 0x201e,
                                           kAlternateTimeOffsetName           = 0x201f,
                                           kAlternateTimeOffsetMaxKey        = 0x2020,
                                           kAlternateTimeOffsetProperties     = 0x2021,
                                           kExternalPortConfigurationEnabled  = 0x3000,
                                           kMasterOnly                          = 0x3001,
                                           kHoldoverUpgradeEnable              = 0x3002,
                                           kExtPortConfigPortDataSet        = 0x3003,
                                           kTransparentClockDefaultDataSet   = 0x4000,
                                           kTransparentClockPortDataSet      = 0x4001,
                                           kPrimaryDomain                       = 0x4002,
                                           kDelayMechanism                      = 0x6000,
                                           kLogMinPdelayReqInterval          = 0x6001,
                                           kTimeStatusNp                       = 0xc000,
                                           kPortDataSetNp                     = 0xc002,
                                           // the following are linuxptp specific
                                           kGrandmasterSettingsNp              = 0xc001,
                                           kSubscribeEventsNp                  = 0xc003,
                                           kPortPropertiesNp                   = 0xc004,
                                           kPortStatsNp                        = 0xc005,
                                           kSynchronizationUncertainNp         = 0xc006,
                                           kPortServiceStatsNp                = 0xc007,
                                           kUnicastMasterTableNp              = 0xc008,
                                           kPortHwclockNp                      = 0xc009,
                                           kPowerProfileSettingsNp            = 0xc00a,
                                           kCmldsInfoNp                        = 0xc00b 
                                        };


        enum class enumGet : std::uint16_t {kClockDescription                = static_cast<uint16_t>(enumId::kClockDescription),
                                            kUserDescription                 = static_cast<uint16_t>(enumId::kUserDescription),
                                            kDefaultDataSet                 = static_cast<uint16_t>(enumId::kDefaultDataSet),
                                            kCurrentDataSet                 = static_cast<uint16_t>(enumId::kCurrentDataSet),
                                            kPortDataSet                    = static_cast<uint16_t>(enumId::kPortDataSet),
                                            kParentDataSet                  = static_cast<uint16_t>(enumId::kParentDataSet),
                                            kTimePropertiesDataSet         = static_cast<uint16_t>(enumId::kTimePropertiesDataSet),
                                            kPriority1                        = static_cast<uint16_t>(enumId::kPriority1),
                                            kPriority2                        = static_cast<uint16_t>(enumId::kPriority2),
                                            kDomain                           = static_cast<uint16_t>(enumId::kDomain),
                                            kSlaveOnly                       = static_cast<uint16_t>(enumId::kSlaveOnly),
                                            kMasterOnly                      = static_cast<uint16_t>(enumId::kMasterOnly),
                                            kLogAnnounceInterval            = static_cast<uint16_t>(enumId::kLogAnnounceInterval),
                                            kLogMinPdelayReqInterval      = static_cast<uint16_t>(enumId::kLogMinPdelayReqInterval),
                                            kDelayMechanism                  = static_cast<uint16_t>(enumId::kDelayMechanism),
                                            kLogSyncInterval                = static_cast<uint16_t>(enumId::kLogSyncInterval),
                                            kAnnounceReceiptTimeout         = static_cast<uint16_t>(enumId::kAnnounceReceiptTimeout),
                                            kAlternateTimeOffsetEnable     = static_cast<uint16_t>(enumId::kAlternateTimeOffsetEnable),
                                            kClockAccuracy                   = static_cast<uint16_t>(enumId::kClockAccuracy),
                                            kTraceabilityProperties          = static_cast<uint16_t>(enumId::kTraceabilityProperties),
                                            kTimescaleProperties             = static_cast<uint16_t>(enumId::kTimescaleProperties),
                                            kVersionNumber                   = static_cast<uint16_t>(enumId::kVersionNumber),
                                            kTime                             = static_cast<uint16_t>(enumId::kTime),
                                            kAlternateTimeOffsetName       = static_cast<uint16_t>(enumId::kAlternateTimeOffsetName),
                                            kAlternateTimeOffsetProperties = static_cast<uint16_t>(enumId::kAlternateTimeOffsetProperties),
                                            kSynchronizationUncertainNp     = static_cast<uint16_t>(enumId::kSynchronizationUncertainNp),
                                            kTimeStatusNp                   = static_cast<uint16_t>(enumId::kTimeStatusNp),
                                            kGrandmasterSettingsNp          = static_cast<uint16_t>(enumId::kGrandmasterSettingsNp),
                                            kSubscribeEventsNp              = static_cast<uint16_t>(enumId::kSubscribeEventsNp),
                                            kPortPropertiesNp               = static_cast<uint16_t>(enumId::kPortPropertiesNp),
                                            kPortStatsNp                    = static_cast<uint16_t>(enumId::kPortStatsNp),
                                            kPortServiceStatsNp            = static_cast<uint16_t>(enumId::kPortServiceStatsNp)};
        enum class enumSet : std::uint16_t { kPriority1                        = static_cast<uint16_t>(enumId::kPriority1),
                                             kPriority2                        = static_cast<uint16_t>(enumId::kPriority2),
                                             kAlternateTimeOffsetEnable     = static_cast<uint16_t>(enumId::kAlternateTimeOffsetEnable),
                                             kAlternateTimeOffsetName       = static_cast<uint16_t>(enumId::kAlternateTimeOffsetName),
                                             kAlternateTimeOffsetProperties = static_cast<uint16_t>(enumId::kAlternateTimeOffsetProperties),
                                             kUserDescription                 = static_cast<uint16_t>(enumId::kUserDescription),
                                             kDomain                           = static_cast<uint16_t>(enumId::kDomain),
                                             kSlaveOnly                       = static_cast<uint16_t>(enumId::kSlaveOnly),
                                             kAnnounceReceiptTimeout         = static_cast<uint16_t>(enumId::kAnnounceReceiptTimeout),
                                             kLogAnnounceInterval            = static_cast<uint16_t>(enumId::kLogAnnounceInterval),
                                             kLogSyncInterval                = static_cast<uint16_t>(enumId::kLogSyncInterval),
                                             kVersionNumber                   = static_cast<uint16_t>(enumId::kVersionNumber),
                                             kTime                             = static_cast<uint16_t>(enumId::kTime),                                     
                                             kGrandmasterSettingsNp          = static_cast<uint16_t>(enumId::kGrandmasterSettingsNp),
                                             kSubscribeEventsNp              = static_cast<uint16_t>(enumId::kSubscribeEventsNp),
                                             kSynchronizationUncertainNp     = static_cast<uint16_t>(enumId::kSynchronizationUncertainNp),
                                             kPortDataSetNp                 = static_cast<uint16_t>(enumId::kPortDataSetNp),
                                             kPowerProfileSettingsNp        = static_cast<uint16_t>(enumId::kPowerProfileSettingsNp)};

        enum class enumAction {kGet, kSet, kResponse, kCommand, kAcknowledge};
        enum class enumError{kOkay=0, kResponseTooBig=1, kNoSuchId, kWrongLength, kWrongValue, kNotSetable, kNotSupported, kGeneralError = 0xFFFE };
    }

    namespace clck
    {
        enum class enumAccuracy : std::uint8_t { kNa = 0, k25Ns = 0x20, k100Ns = 0x21, k250Ns = 0x22,
                                    k1Us = 0x23, k2_5Us = 0x24, k10Us = 0x25, k25Us = 0x26, k100Us = 0x27, k250Us = 0x28,
                                    k1Ms = 0x29, k2_5Ms = 0x2A, k10Ms = 0x2B, k25Ms = 0x2C, k100Ms = 0x2D, k250Ms = 0x2E,
                                    k1S = 0x2F, k10S = 0x30, kOver=0x31, kUnknown = 0xF3};

        enum class enumTimeSource: std::uint8_t  {kNa = 0, kAtomic = 0x10, kGps = 0x20, kRadio = 0x30, kPtp = 0x40, kNtp = 0x50, kHandset = 0x60, kOther = 0x90, kInternal = 0xA0};

        enum class enumType {kOrdinary = 0x8000, kBoundary = 0x4000, kTransparentP2P = 0x2000, kTransparentE2E = 0x1000, kManagement=0x800};

        enum class enumSync { kUncertainDontCare = 0xff, kUncertainFalse = 0, kUncertainTrue = 1};
        enum class enumDelay {kE2E = 0x01, kP2P = 0x02, kDisabled = 0xFE};
    }

    namespace port
    {
        enum enumTimestamping {kNone = 0, kTimestampTxHardware = 1, kTimestampTxSoftware = 2, kTimestampRxHardware = 4, kTimestampRxSoftware = 8 };
    }

    enum class Rate : std::int8_t { kNever = -8, kPerSec128 = -7, kPerSec64 = -6, kPerSec32 = -5, kPerSec16 = -4, kPerSec8 = -3, kPerSec4 = -2, kPerSec2 = -1, kEvery1Sec = 0, kEvery2Sec = 1, kEvery4Sec = 2, kEvery8Sec = 3, kEvery16Sec = 4};

    

    enum class Mode : std::int8_t {kMulticast, kUnicast, kHybrid};

}