#include "ptploghandler.h"
#include "ptpstructs.h"
#include <iostream>
#include "timeutils.h"

void PtpLogHandler::HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload)
{
    std::shared_ptr<ptpV2Header> pPtpH = std::dynamic_pointer_cast<ptpV2Header>(pHeader);
    std::shared_ptr<ptpV2Payload> pPtpP = std::dynamic_pointer_cast<ptpV2Payload>(pPayload);
    if(pPtpP && pPtpH)
    {
        if((int)pPtpH->nType != 11)
        {
            std::cout << (int)pPtpH->nType << "\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime) << std::endl;
        }
        else
        {
            std::cout << (int)pPtpH->nType << "\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << std::endl;
        }
    }
}
