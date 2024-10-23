// abstract packet class which will be a parent
// for more concrete packet types (eth, ip, arp, tcp)
#include <memory>

class Packet
{
public:
    std::shared_ptr<Packet> encapsulatedPacket;
    std::weak_ptr<Packet> parentPacket;

    Packet() = default;
    virtual ~Packet() = default;
    // should parse its own protocol specific data within the parser class
    virtual void parse(const u_char *data, size_t length) = 0;
    virtual void print() const = 0;
};