/**
  @file
  @author Stefan Frings
*/

#ifndef DUMPCONTROLLER_H
#define DUMPCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"


using namespace stefanfrings;

/**
  This controller dumps the received HTTP request in the response.
*/

class DumpController : public HttpRequestHandler{
    Q_OBJECT
    Q_DISABLE_COPY(DumpController)
public:

    /** Constructor */
    DumpController(QObject* parent=nullptr);

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
    bool matchUrl(QByteArray& path);
};

#endif // DUMPCONTROLLER_H
