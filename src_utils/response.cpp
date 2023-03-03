#include "response.hpp"


std::map<int, std::string>  Response::_status_codes;
bool                        Response::_initialized = {false};

void Response::initStatusCode(void ) {
    _status_codes[CONTINUE] = "Continue";
    _status_codes[OK] = "OK";
    _status_codes[CREATED] = "Created";
    _status_codes[ACCEPTED] = "Accepted";
    _status_codes[NO_CONTENT] = " No Content";
    _status_codes[MULTIPLE] = "Multiple Choices";
    _status_codes[MOVED] = "Moved Permanently";
    _status_codes[FOUND] = "Found";
    _status_codes[OTHER] = "See Other";
    _status_codes[NOT_MOD] = "Not Modified";
    _status_codes[TMP_REDIR] = "Temporary Redirect";
    _status_codes[PERM_REDIR] = "Permanent Redirect";
    _status_codes[BAD_REQUEST] = "Bad Request";
    _status_codes[UNAUTHORIZED] = "Unauthorized";
    _status_codes[FORBIDDEN] = "Forbidden";
    _status_codes[NOT_FOUND] = "Not Found";
    _status_codes[METHOD_NOT_ALLOWED] = "Method Not Allowed";
    _status_codes[LENGTH_REQUIRED] = "Length Required";
    _status_codes[PAYLOAD] = "Request Entity Too Large";
    _status_codes[SERVER_ERROR] = "Internal Server Error";
    _status_codes[NOT_IMPLEMENTED] = "Not Implemented";
    _status_codes[HTTP_VERSION] = "HTTP Version not supported";
    _initialized = true;
}

Response::Response(int status, size_t b_len, std::string b_type, std::string body) : _status(status), _len(b_len), _type(b_type), _body(body) {
    if (!_initialized)
        initStatusCode();
    std::ostringstream ss;
    ss << getStatusLine() << getGeneralHeader() << getResponseHeader() << getEntityHeader() << CRLF << _body;
    _response = ss.str();
}

Response::Response(int status ) : _status(status) {
    if (!_initialized)
        initStatusCode();
    std::ostringstream ss;
    ss << getStatusLine()<< getGeneralHeader() << getResponseHeader() << getEntityHeader() << CRLF;
    _response = ss.str();
}

Response::Response(int status, size_t b_len, std::string b_type, std::string body, std::map<std::string, std::string> header_values) : _status(status), _len(b_len), _type(b_type), _body(body)
{
	std::map<std::string, std::string>::iterator it;

    if (!_initialized)
        initStatusCode();
    std::ostringstream ss;
    ss << getStatusLine();
    ss << "Date: " << setDate(); //TODO: MODIFY THIS
    for (it = header_values.begin(); it != header_values.end(); it++) {
        ss << it->first << ": " << it->second << CRLF;
    }
	ss << CRLF << _body;
    _response = ss.str();
}

Response::~Response() {}

std::string    Response::setDate()
{
	time_t now;

	now = time(NULL);
	return (ctime(&now));
}

std::string  Response::getGeneralHeader( ) {
    std::ostringstream ss;

    ss << "Date: " << setDate();
    ss << "Connection: close" << CRLF; //Update en fonction du fichier de configuration ?
    return (ss.str());
}

/* There are a few header fields which have general applicability for
   both request and Response messages, but which do not apply to the
   entity being transferred. These header fields apply only to the
   message being transmitted.

       general-header = Cache-Control            ; Section 14.9
                      | Connection               ; Section 14.10
                      | Date                     ; Section 14.18
                      | Pragma                   ; Section 14.32
                      | Trailer                  ; Section 14.40
                      | Transfer-Encoding        ; Section 14.41
                      | Upgrade                  ; Section 14.42
                      | Via                      ; Section 14.45
                      | Warning                  ; Section 14.46

   General-header field names can be extended reliably only in
   combination with a change in the protocol version. However, new or
   experimental header fields may be given the semantics of general
   header fields if all parties in the communication recognize them to
   be general-header fields. Unrecognized header fields are treated as
   entity-header fields.
 *
 */

std::string  Response::getResponseHeader( ) {
    std::ostringstream ss;
    ss << "Accept-Ranges: none" << CRLF; // À vérifier
    if (_status == 201 || (_status / 100) == 3) {
        ss << "Location: " << CRLF; } // Trouver un moyen pour me donner la location
    if (_status == 401) {
        ss << "WWW-Authenticate: Basic realm=Restricted Area" << CRLF; }
    return (ss.str());
}
/* Response-header
 * Accept-Ranges        { Section 14.5  } : Set à "none" -> vérifier
 * Age                  { Section 14.6  } : vraiment nécessaire ? Pas implémenté pour l'instant
 * ETag                 { Section 14.19 } : Idem que pour "Age"
 * Location             { Section 14.30 } : Manque la location dans mes paramètres
 * Proxy-Authenticate   { Section 14.33 } : Error 407 (Proxy Authentication Required) uniquement, on traite ?
 * Retry-After          { Section 14.37 } : Error 503 (Service Unavailable), on traite ?
 * Server               { Section 14.38 } : Revealing the specific software version of the server might
                                            allow the server machine to become more vulnerable to attacks
                                            against software that is known to contain security holes. Server
                                            implementors are encouraged to make this field a configurable
                                            option.
 * Vary                                   : An HTTP/1.1 server SHOULD include a Vary header field with any
                                            cacheable Response that is subject to server-driven negotiation.
 * WWW-Authenticate                       : Error 401 (Unauthorized) The field value consists of at
                                            least one challenge that indicates the authentication scheme(s) and
                                            parameters applicable to the Request-URI. */

const std::string  Response::getEntityHeader(void ) {
    std::ostringstream ss;
    if (_status == METHOD_NOT_ALLOWED)
        ss << "Allow: GET, POST, DELETE" << CRLF; // Corriger en obtenant l'information du fichier de configuration;
    if (_len) {
        ss << "Content-Length: " << _len << CRLF;
        ss << "Content-Type: " << _type << CRLF;
    }
    return (ss.str());
}

/* Entity Header Fields

   Entity-header fields define metainformation about the entity-body or,
   if no body is present, about the resource identified by the request.
   Some of this metainformation is OPTIONAL; some might be REQUIRED by
   portions of this specification.

       entity-header  = Allow                    ; Section 14.7
                      | Content-Encoding         ; Section 14.11 :      If the content-coding of an entity is not "identity", then the
                                                                        Response MUST include a Content-Encoding entity-header (section
                                                                        14.11) that lists the non-identity content-coding(s) used.
                      | Content-Language         ; Section 14.12 :      MAY -> OPTIONAL.
                      | Content-Length           ; Section 14.13 :      Ok -> Vérifier que _body.size() correspond à _len ?
                      | Content-Location         ; Section 14.14 :      MAY -> OPTIONAL.
                      | Content-MD5              ; Section 14.15
                      | Content-Range            ; Section 14.16 :      Important en cas de Status 206 (Partial Content), sauf erreur on ne gère pas.
                      | Content-Type             ; Section 14.17
                      | Expires                  ; Section 14.21 :      Pas sûr que ce soit nécessaire.
                      | Last-Modified            ; Section 14.29 :      HTTP/1.1 servers SHOULD send Last-Modified whenever feasible.
                      | extension-header

       extension-header = message-header

   The extension-header mechanism allows additional entity-header fields
   to be defined without changing the protocol, but these fields cannot
   be assumed to be recognizable by the recipient. Unrecognized header
   fields SHOULD be ignored by the recipient and MUST be forwarded by
   transparent proxies.

 *
 */

const std::string  Response::getStatusLine(void ) {
    std::ostringstream ss;
    ss << "HTTP/1.1 " << _status << " " << _status_codes[_status] << CRLF;
    return (ss.str());
}

const std::string&  Response::getResponse(void ) {
    return _response;
}
