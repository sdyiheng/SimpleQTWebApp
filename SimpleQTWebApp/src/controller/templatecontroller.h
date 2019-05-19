/**
  @file
  @author Stefan Frings
*/

#ifndef TEMPLATECONTROLLER_H
#define TEMPLATECONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

/**
  This controller generates a website using the template engine.
  It generates a Latin1 (ISO-8859-1) encoded website from a UTF-8 encoded template file.
*/

class TemplateController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(TemplateController)
public:

    /** Constructor */
    TemplateController(QObject* parent=nullptr);

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

    bool matchUrl(QByteArray& path);
};

#endif // TEMPLATECONTROLLER_H