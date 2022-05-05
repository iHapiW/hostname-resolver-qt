#define Red "\u001b[31m"
#define Reset "\u001b[0m"

#include <QCoreApplication>
#include <QHostAddress>
#include <QTextStream>

#include "resolver.h"

QTextStream cout(stdout);
QTextStream cin(stdin);

using namespace Qt;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Configuring DNS & Resolver
    QHostAddress dns_addr("1.1.1.1");
    quint16 dns_port = 53;
    Resolver resolver(dns_addr, dns_port);

    // Allocating Stack Memories
    QString addr;
    QHostAddress resolved;

    // Printing Banner
    cout << Red << "              _______  _______ _________   _______  _______  _______  _______  _               _______  _______" << Reset << endl;
    cout << Red << "    |\\     /|(  ___  )(  ____ \\\\__   __/  (  ____ )(  ____ \\(  ____ \\(  ___  )( \\    |\\     /|(  ____ \\(  ____ )" << Reset <<  endl;
    cout << Red << "    | )   ( || (   ) || (    \\/   ) (     | (    )|| (    \\/| (    \\/| (   ) || (    | )   ( || (    \\/| (    )|" << Reset << endl;
    cout << Red << "    | (___) || |   | || (_____    | |     | (____)|| (__    | (_____ | |   | || |    | |   | || (__    | (____)|" << Reset << endl;
    cout << Red << "    |  ___  || |   | |(_____  )   | |     |     __)|  __)   (_____  )| |   | || |    ( (   ) )|  __)   |     __)" << Reset << endl;
    cout << Red << "    | (   ) || |   | |      ) |   | |     | (\\ (   | (            ) || |   | || |     \\ \\_/ / | (      | (\\ (" << Reset << endl;
    cout << Red << "    | )   ( || (___) |/\\____) |   | |     | ) \\ \\__| (____/\\/\\____) || (___) || (____/\\\\   /  | (____/\\| ) \\ \\__" << Reset << endl;
    cout << Red << "    |/     \\|(_______)\\_______)   )_(     |/   \\__/(_______/\\_______)(_______)(_______/ \\_/   (_______/|/   \\__/" << Reset << endl;
    cout << endl;

    //! Starting Application Flow
    while(true)
    {
        cout << endl << "\tEnter Hostname: " << flush;
        cin >> addr;

        // Validating Hostname
        if( addr.split('.').size() < 2 )
        {
            cout << "\tInvalid Hostname" << endl;
            continue;
        }

        // Resolving Address
        resolved = resolver.resolve(addr);

        // Validating Resolved Address ( Address will be 0.0.0.0 if any Errors happen while Resolving )
        if( resolved.toIPv4Address() == 0 )
            cout << "\tResult: " << "Host Not Found" << endl;
        else
            cout << "\tResult: " << resolved.toString() << endl;
    }
    return a.exec();
}
