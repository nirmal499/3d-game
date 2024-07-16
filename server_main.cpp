#include <net/server.hpp>

int main()
{
    Server server(60000);
    server.Start();

    while(true)
    {
        server.Update(true);
    }

}