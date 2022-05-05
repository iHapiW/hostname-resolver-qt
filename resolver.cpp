#include "resolver.h"

// Initializing Socket
Resolver::Resolver(QHostAddress addr, quint16 port, QObject *parent)
    : QObject{parent}
{
    connect(&m_socket, &QUdpSocket::readyRead,
            this, &Resolver::readyRead);

    m_port = port;
    m_addr = addr;
}

Resolver::~Resolver()
{
    m_socket.close();
}

QHostAddress Resolver::resolve(QString hostname)
{
    // Constructing Data
    QByteArray data;

    // Transaction ID ( self made)
    data.append(0x38);
    data.append(0x38);

    // Flags ( Standard Query )
    data.append(0x01);
    data.append('\x00');

    // Questions ( 1 )
    data.append('\x00');
    data.append(0x01);

    // Answer & Authority & Additional RRS
    data.append('\x00');
    data.append('\x00');
    data.append('\x00');
    data.append('\x00');
    data.append('\x00');
    data.append('\x00');

    // Hostname Lables
    data.append(hostToLabels(hostname));

    // Type ( A )
    data.append('\x00');
    data.append(0x01);

    // Class ( IN )
    data.append('\x00');
    data.append(0x01);

    QNetworkDatagram datagram(data, m_addr, m_port);

    // Opening Socket + Handeling Error
    if(!m_socket.open(QIODevice::ReadWrite))
    {
        qInfo() << "\tCould'nt Open Socket for Reading";
        return QHostAddress((quint32) 0);
    }

    // Setting m_sentLength for future uses
    m_sentLength = data.size();

    // Sending Data
    m_socket.writeDatagram(datagram);

    //! Getting into Event loop , to wait for m_resolved
    //! when data arrives, Resolver::readyRead() will be Asynchronously executed
    m_loop.exec();
    return m_resolved;
}

void Resolver::readyRead()
{
    QByteArray response;

    // Reading Untill it Has Data
    if(m_socket.hasPendingDatagrams())
        response = m_socket.receiveDatagram().data();

    // Analyzing Response Flag if there was no Error ( 0x8180 )
    QByteArray flags = response.left(4);
    flags = flags.right(2);
    if( flags != "\x81\x80" )
    {
        m_resolved.setAddress("0.0.0.0");
        m_loop.quit();
        return;
    }

    // Analyzing Response Type to see if its A Type ( 0x0001 )
    QByteArray respType = response.right(response.size() - m_sentLength);
    respType = respType.left(4);
    respType = respType.right(2);

    QByteArray validType;
    validType.append('\x00');
    validType.append(0x1);

    if( respType != validType )
    {
        m_resolved.setAddress("0.0.0.0");
        m_loop.quit();
        return;
    }

    // Getting last 4 bytes of address ( it should be IP Address )
    QByteArray addr_data = response.right(4);
    QString addr;

    // Converting last 4 bytes to octets and appending to $addr variable.
    for(int i = 0; i < 4; i++)
    {
        addr.append(QString::number((quint8) addr_data.at(i)));
        if(i != 3)  addr.append('.');
    }

    // Setting Resolved Address;
    m_resolved.setAddress(addr);

    // Quitting Event loop to let Resolver::resolve() continue responding with Resolved Address
    m_loop.quit();
}

QByteArray Resolver::hostToLabels(QString hostname)
{
    /*
     *
     * Making Labels from Hostname
     * Example:
     *
     * Input : google.com
     * Output: 0x06 + google + 0x03 com + 0x00
     *
     * Overal Form: 0xLen1 + label1 + 0xLen2 + label2 + ... + 0x0
     *
     */

    QByteArray result;
    foreach(QString label, hostname.split('.'))
    {
        result.append(label.size());
        result.append(label.toLatin1());
    }

    result.append('\x00');
    return result;
}
