#ifndef RESOLVER_H
#define RESOLVER_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QEventLoop>

class Resolver : public QObject
{
    Q_OBJECT
public:
    explicit Resolver(QHostAddress addr, quint16 port, QObject *parent = nullptr);
    QHostAddress resolve(QString hostname);
    ~Resolver();

public slots:
    void readyRead();

private:
    // Used for UDP DNS Packet
    QByteArray hostToLabels(QString hostname);

    // Socket Information
    QUdpSocket m_socket;
    QHostAddress m_addr;
    quint16 m_port;

    // Event Loop for Blocking application to Wait for DNS Response
    QEventLoop m_loop;

    // Resolved Address , that is going to be returned by Resolver::resolve
    QHostAddress m_resolved;

    // Used for Validating DNS Response
    quint64 m_sentLength;
};

#endif // RESOLVER_H
