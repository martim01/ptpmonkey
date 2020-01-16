#include "loghander.h"
#include "ptpstructs.h"

void LogHandler::HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload)
{
    pHeader->OutputValues();
    pPayload->OutputValues();
}
