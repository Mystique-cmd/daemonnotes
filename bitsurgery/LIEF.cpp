#include <LIEF/ELF.hpp>
using namespace LIEF::ELF;

int main() {
    auto binary = Parser::parse("elf-Linux-lib-x64.so");

    Section cave{".cave"};
    cave.content(std::vector<uint8_t>(256, 0x90));

    // Use fully qualified enum
    cave.flags(LIEF::ELF::SECTION_FLAGS::EXECINSTR |
               LIEF::ELF::SECTION_FLAGS::ALLOC);

    binary->add(cave, true);
    binary->write("patched.elf");
}
