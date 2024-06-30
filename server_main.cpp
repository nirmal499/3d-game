#include <net/server.hpp>

int main()
{
    Server server(60000);
    server.Start();

    while(1)
    {
        server.Update(true);
    }

}