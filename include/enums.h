#pragma once

namespace pml::ptpmonkey
{
    namespace hdr
    {
        enum class enumType{SYNC=0x0, DELAY_REQ=0x1, PEER_DLEAY_REQ = 0x2, PEER_DELAY_RESP = 0x3, FOLLOW_UP=0x8, DELAY_RESP=0x9, PEER_DELAY_RESP_FOLLOW_UP = 0xA,
                    ANNOUNCE=0xB, SIGNALLING=0xC, MANAGEMENT=0xD};

        enum class enumFlags{ ALTERNATE_MASTER = 0x0100,
                            TWO_STEP         = 0x0200,
                            UNICAST          = 0x0400,
                            PROFILE1         = 0x2000,
                            PROFILE2         = 0x4000,
                            SECURITY         = 0x8000,
                            LI_61            = 0x0001,
                            LI_59            = 0x0002,
                            UTC_OFFSET_VALID = 0x0004,
                            TIMESCALE        = 0x0008,
                            TIME_TRACEABLE   = 0x0010,
                            FREQ_TRACEABLE   = 0x0020 };

    }

    namespace mngmnt
    {
        enum class enumType {MANAGEMENT = 0x0001, MANAGEMENT_ERROR_STATUS = 0x0002, ORGANIZATION_EXTENSION = 0x0003, REQUEST_UNICAST_TRANSMISSION = 0x0004, GRANT_UNICAST_TRANSMISSION = 0x0005, CANCEL_UNICAST_TRANSMISSION = 0x0006, ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION = 0x0007, PATH_TRACE = 0x0008, ALTERNATE_TIME_OFFSET_INDICATOR = 0x0009, AUTHENTICATION_2008 = 0x2000, AUTHENTICATION_CHALLENGE = 0x2001, SECURITY_ASSOCIATION_UPDATE = 0x2002, CUM_FREQ_SCALE_FACTOR_OFFSET = 0x2003, PTPMON_REQ = 0x21FE, PTPMON_RESP = 0x21FF, ORGANIZATION_EXTENSION_PROPAGATE = 0x4000, ENHANCED_ACCURACY_METRICS = 0x4001, O, RGANIZATION_EXTENSION_DO_NOT_PROPAGATE = 0x8000, L1_SYNC = 0x8001, PORT_COMMUNICATION_AVAILABILITY = 0x8002, PROTOCOL_ADDRESS = 0x8003, SLAVE_RX_SYNC_TIMING_DATA = 0x8004, SLAVE_RX_SYNC_COMPUTED_DATA = 0x8005,SLAVE_TX_EVENT_TIMESTAMPS = 0x8006, SLAVE_DELAY_TIMING_DATA_NP = 0x7F00, CUMULATIVE_RATE_RATIO = 0x8007, PAD = 0x8008, AUTHENTICATION = 0x8009 };
        
        enum class enumId : std::uint16_t {ERROR=0, USER_DESCRIPTION = 0x0002, SAVE_IN_NON_VOLATILE_STORAGE = 0x0003, RESET_NON_VOLATILE_STORAGE = 0x0004, INITIALIZE = 0x0005, FAULT_LOG = 0x0006, FAULT_LOG_RESET = 0x0007, DEFAULT_DATA_SET = 0x2000, CURRENT_DATA_SET = 0x2001, PARENT_DATA_SET = 0x2002, TIME_PROPERTIES_DATA_SET = 0x2003, PRIORITY1 = 0x2005, PRIORITY2 = 0x2006, DOMAIN = 0x2007, SLAVE_ONLY = 0x2008, TIME = 0x200F, CLOCK_ACCURACY = 0x2010, UTC_PROPERTIES = 0x2011, TRACEABILITY_PROPERTIES = 0x2012, TIMESCALE_PROPERTIES = 0x2013, PATH_TRACE_LIST = 0x2015, PATH_TRACE_ENABLE = 0x2016, GRANDMASTER_CLUSTER_TABLE = 0x2017, ACCEPTABLE_MASTER_TABLE = 0x201A, ACCEPTABLE_MASTER_MAX_TABLE_SIZE = 0x201C, ALTERNATE_TIME_OFFSET_ENABLE = 0x201E, ALTERNATE_TIME_OFFSET_NAME = 0x201F, ALTERNATE_TIME_OFFSET_MAX_KEY = 0x2020, ALTERNATE_TIME_OFFSET_PROPERTIES = 0x2021, EXTERNAL_PORT_CONFIGURATION_ENABLED = 0x3000, HOLDOVER_UPGRADE_ENABLE = 0x3002, TRANSPARENT_CLOCK_DEFAULT_DATA_SET = 0x4000, PRIMARY_DOMAIN = 0x4002, TIME_STATUS_NP = 0xC000, GRANDMASTER_SETTINGS_NP = 0xC001, SUBSCRIBE_EVENTS_NP = 0xC003, SYNCHRONIZATION_UNCERTAIN_NP = 0xC006, NULL_MANAGEMENT = 0x0000, CLOCK_DESCRIPTION = 0x0001, PORT_DATA_SET = 0x2004, LOG_ANNOUNCE_INTERVAL = 0x2009, ANNOUNCE_RECEIPT_TIMEOUT = 0x200A, LOG_SYNC_INTERVAL = 0x200B, VERSION_NUMBER = 0x200C, ENABLE_PORT = 0x200D, DISABLE_PORT = 0x200E, UNICAST_NEGOTIATION_ENABLE = 0x2014, UNICAST_MASTER_TABLE = 0x2018, UNICAST_MASTER_MAX_TABLE_SIZE = 0x2019, ACCEPTABLE_MASTER_TABLE_ENABLED = 0x201B, ALTERNATE_MASTER = 0x201D, MASTER_ONLY = 0x3001, EXT_PORT_CONFIG_PORT_DATA_SET = 0x3003, TRANSPARENT_CLOCK_PORT_DATA_SET = 0x4001, DELAY_MECHANISM = 0x6000, LOG_MIN_PDELAY_REQ_INTERVAL = 0x6001, PORT_DATA_SET_NP = 0xC002, PORT_PROPERTIES_NP = 0xC004, PORT_STATS_NP = 0xC005, PORT_SERVICE_STATS_NP = 0xC007, UNICAST_MASTER_TABLE_NP = 0xC008, PORT_HWCLOCK_NP = 0xC009, POWER_PROFILE_SETTINGS_NP = 0xC00A, CMLDS_INFO_NP = 0xC00B };


        enum class enumGet : std::uint16_t {CLOCK_DESCRIPTION                = static_cast<uint16_t>(enumId::CLOCK_DESCRIPTION),
                                            USER_DESCRIPTION                 = static_cast<uint16_t>(enumId::USER_DESCRIPTION),
                                            DEFAULT_DATA_SET                 = static_cast<uint16_t>(enumId::DEFAULT_DATA_SET),
                                            CURRENT_DATA_SET                 = static_cast<uint16_t>(enumId::CURRENT_DATA_SET),
                                            PORT_DATA_SET                    = static_cast<uint16_t>(enumId::PORT_DATA_SET),
                                            PARENT_DATA_SET                  = static_cast<uint16_t>(enumId::PARENT_DATA_SET),
                                            TIME_PROPERTIES_DATA_SET         = static_cast<uint16_t>(enumId::TIME_PROPERTIES_DATA_SET),
                                            PRIORITY1                        = static_cast<uint16_t>(enumId::PRIORITY1),
                                            PRIORITY2                        = static_cast<uint16_t>(enumId::PRIORITY2),
                                            DOMAIN                           = static_cast<uint16_t>(enumId::DOMAIN),
                                            SLAVE_ONLY                       = static_cast<uint16_t>(enumId::SLAVE_ONLY),
                                            MASTER_ONLY                      = static_cast<uint16_t>(enumId::MASTER_ONLY),
                                            SYNCHRONIZATION_UNCERTAIN_NP     = static_cast<uint16_t>(enumId::SYNCHRONIZATION_UNCERTAIN_NP),
                                            LOG_ANNOUNCE_INTERVAL            = static_cast<uint16_t>(enumId::LOG_ANNOUNCE_INTERVAL),
                                            LOG_MIN_PDELAY_REQ_INTERVAL      = static_cast<uint16_t>(enumId::LOG_MIN_PDELAY_REQ_INTERVAL),
                                            DELAY_MECHANISM                  = static_cast<uint16_t>(enumId::DELAY_MECHANISM),
                                            LOG_SYNC_INTERVAL                = static_cast<uint16_t>(enumId::LOG_SYNC_INTERVAL),
                                            ANNOUNCE_RECEIPT_TIMEOUT         = static_cast<uint16_t>(enumId::ANNOUNCE_RECEIPT_TIMEOUT),
                                            ALTERNATE_TIME_OFFSET_ENABLE     = static_cast<uint16_t>(enumId::ALTERNATE_TIME_OFFSET_ENABLE),
                                            CLOCK_ACCURACY                   = static_cast<uint16_t>(enumId::CLOCK_ACCURACY),
                                            TRACEABILITY_PROPERTIES          = static_cast<uint16_t>(enumId::TRACEABILITY_PROPERTIES),
                                            TIMESCALE_PROPERTIES             = static_cast<uint16_t>(enumId::TIMESCALE_PROPERTIES),
                                            VERSION_NUMBER                   = static_cast<uint16_t>(enumId::VERSION_NUMBER),
                                            ALTERNATE_TIME_OFFSET_NAME       = static_cast<uint16_t>(enumId::ALTERNATE_TIME_OFFSET_NAME),
                                            ALTERNATE_TIME_OFFSET_PROPERTIES = static_cast<uint16_t>(enumId::ALTERNATE_TIME_OFFSET_PROPERTIES),
                                            TIME_STATUS_NP                   = static_cast<uint16_t>(enumId::TIME_STATUS_NP),
                                            GRANDMASTER_SETTINGS_NP          = static_cast<uint16_t>(enumId::GRANDMASTER_SETTINGS_NP),
                                            SUBSCRIBE_EVENTS_NP              = static_cast<uint16_t>(enumId::SUBSCRIBE_EVENTS_NP),
                                            PORT_PROPERTIES_NP               = static_cast<uint16_t>(enumId::PORT_PROPERTIES_NP),
                                            PORT_STATS_NP                    = static_cast<uint16_t>(enumId::PORT_STATS_NP),
                                            PORT_SERVICE_STATS_NP            = static_cast<uint16_t>(enumId::PORT_SERVICE_STATS_NP)};

        enum class enumSet : std::uint16_t { PRIORITY1                        = static_cast<uint16_t>(enumId::PRIORITY1),
                                             PRIORITY2                        = static_cast<uint16_t>(enumId::PRIORITY2),
                                             ALTERNATE_TIME_OFFSET_ENABLE     = static_cast<uint16_t>(enumId::ALTERNATE_TIME_OFFSET_ENABLE),
                                             ALTERNATE_TIME_OFFSET_NAME       = static_cast<uint16_t>(enumId::ALTERNATE_TIME_OFFSET_NAME),
                                             ALTERNATE_TIME_OFFSET_PROPERTIES = static_cast<uint16_t>(enumId::ALTERNATE_TIME_OFFSET_PROPERTIES),
                                             GRANDMASTER_SETTINGS_NP          = static_cast<uint16_t>(enumId::GRANDMASTER_SETTINGS_NP),
                                             SUBSCRIBE_EVENTS_NP              = static_cast<uint16_t>(enumId::SUBSCRIBE_EVENTS_NP),
                                             SYNCHRONIZATION_UNCERTAIN_NP     = static_cast<uint16_t>(enumId::SYNCHRONIZATION_UNCERTAIN_NP),
                                             PORT_DATA_SET_NP                 = static_cast<uint16_t>(enumId::PORT_DATA_SET_NP),
                                             POWER_PROFILE_SETTINGS_NP        = static_cast<uint16_t>(enumId::POWER_PROFILE_SETTINGS_NP)};

        enum class enumAction {GET, SET, RESPONSE, COMMAND, ACKNOWLEDGE};
        enum class enumError{OKAY=0, RESPONSE_TOO_BIG=1, NO_SUCH_ID, WRONG_LENGTH, WRONG_VALUE, NOT_SETABLE, NOT_SUPPORTED, GENERAL_ERROR = 0xFFFE };
    }

    namespace clck
    {
        enum class enumAccuracy : std::uint8_t { ACC_NA = 0, ACC_25NS = 0x20, ACC_100NS = 0x21, ACC_250NS = 0x22,
                                    ACC_1US = 0x23, ACC_2_5US = 0x24, ACC_10US = 0x25, ACC_25US = 0x26, ACC_100US = 0x27, ACC_250US = 0x28,
                                    ACC_1MS = 0x29, ACC_2_5MS = 0x2A, ACC_10MS = 0x2B, ACC_25MS = 0x2C, ACC_100MS = 0x2D, ACC_250MS = 0x2E,
                                    ACC_1S = 0x2F, ACC_10S = 0x30, ACC_OVER=0x31, ACC_UNKNOWN = 0xF3};

        enum class enumTimeSource: std::uint8_t  {NA = 0, ATOMIC = 0x10, GPS = 0x20, RADIO = 0x30, PTP = 0x40, NTP = 0x50, HANDSET = 0x60, OTHER = 0x90, INTERNAL = 0xA0};

        enum class enumType {ORDINARY = 0x8000, BOUNDARY = 0x4000, TRANSPARENT_P2P = 0x2000, TRANSPARENT_E2E = 0x1000, MANAGEMENT=0x800};

        enum class enumSync { UNCERTAIN_DONTCARE = 0xff, UNCERTAIN_FALSE = 0, UNCERTAIN_TRUE = 1};
    }

    namespace port
    {
        enum enumTimestamping {NONE = 0, TIMESTAMP_TX_HARDWARE = 1, TIMESTAMP_TX_SOFTWARE = 2, TIMESTAMP_RX_HARDWARE = 4, TIMESTAMP_RX_SOFTWARE = 8 };
    }

    enum class Rate : std::int8_t { NEVER = -8, PER_SEC_128 = -7, PER_SEC_64 = -6, PER_SEC_32 = -5, PER_SEC_16 = -4, PER_SEC_8 = -3, PER_SEC_4 = -2, PER_SEC_2 = -1,
                                    EVERY_1_SEC = 0, EVERY_2_SEC = 1, EVERY_4_SEC = 2, EVERY_8_SEC = 3, EVERY_16_SEC = 4};

    

    enum class Mode : std::int8_t {MULTICAST, UNICAST, HYBRID};

}