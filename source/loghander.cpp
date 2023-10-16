#include "loghander.h"
#include "ptpstructs.h"

using namespace pml::ptpmonkey;

void LogHandler::HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload)
{
    pHeader->OutputValues();
    pPayload->OutputValues();
}
