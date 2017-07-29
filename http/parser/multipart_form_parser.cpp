#include "http/request/header_fields/content_type.hpp"
#include "multipart_form_parser.hpp"
#include "final_parser.hpp"
#include "parser_helper.hpp"

MultipartFormParser::MultipartFormParser(Request&& req, BufferPtr buffer)
        :AbstractParser(std::move(req), buffer) { }

// TODO Too complex! To be refactored
ParserPtr MultipartFormParser::process()
{
    auto body = partialRequest.body;
    auto contentType = std::dynamic_pointer_cast<ContentType>(partialRequest.header.get("Content-Type").getValue());
    auto boundaryEntry = contentType->getParameters().get("boundary");
    if (!boundaryEntry.isValid())
        throw std::invalid_argument("invalid Content-Type");

    std::string boundary = "--" + boundaryEntry.getValue();

    size_t lineSep = body->find("\r\n", 2);
    if (lineSep >= body->len())
        return std::make_shared<FinalParser>(std::move(partialRequest), buffer);

    auto lineBuf = body->split(lineSep + 2);
    if (lineBuf->find(boundary.c_str(), boundary.length()) != 0)
        throw std::invalid_argument("expect boundary line");

    for (;;)
    {
        std::string name;
        FormData formData;
        // Parsing fields
        for (;;)
        {
            lineSep = body->find("\r\n", 2);
            if (lineSep >= body->len())
                return std::make_shared<FinalParser>(std::move(partialRequest), buffer);

            lineBuf = body->split(lineSep + 2);
            if (lineSep == 0)
                break;

            auto parseResult = ParserHelper::parseHeaderField(lineBuf->toString());

            if (StringUtils::toLowercase(parseResult.first) == "content-disposition")
            {
                auto content = std::make_shared<HeaderContentWithParameters>();
                ParserHelper::parseHeaderFieldWithParameters(content, parseResult.second);

                if (content->getMainContent() != "form-data")
                    throw std::invalid_argument("unsupported Content-Disposition");

                auto parameters = content->getParameters();
                auto nameEntry = parameters.get("name");
                if (!nameEntry.isValid())
                    throw std::invalid_argument("expect name parameter");
                name = nameEntry.getValue();

                auto filenameEntry = parameters.get("filename");
                if (filenameEntry.isValid())
                    formData.parameters.put("filename", filenameEntry.getValue());
            }
            else
            {
                // Not so important fields
                // TODO expect Content-Type?
                formData.parameters.put(parseResult.first, parseResult.second);
            }
        }

        if (name.length() == 0)
            throw std::invalid_argument("no name given");

        for (bool firstLine = true;; firstLine = false)
        {
            lineSep = body->find("\r\n", 2);
            if (lineSep >= body->len())
                throw std::invalid_argument("expect form-data body");

            lineBuf = body->split(lineSep + 2);
            if (lineBuf->find(boundary.c_str(), boundary.length()) == 0)
                break;

            if (!firstLine)
                formData.data += "\r\n";
            formData.data += lineBuf->toString(0, lineSep);
        }

        partialRequest.queries.insert({name, formData});
    }
}
