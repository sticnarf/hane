#include "multipart_form_parser.h"

MultipartFormParser::MultipartFormParser(Request&& req, BufferPtr buffer)
        :AbstractParser(std::move(req), buffer) { }

ParserPtr MultipartFormParser::process()
{


    // TODO
    return AbstractParser::process();
}
