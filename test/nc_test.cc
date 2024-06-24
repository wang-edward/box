#include <iostream>
#include <cstdlib>
#include <clocale>
#include <ncpp/NotCurses.hh>

using namespace ncpp;

void translate(ncintype_e evtype) {
    if (evtype == EvType::Unknown) {
        std::cout << "Unknown" << std::endl;
    } else if (evtype == EvType::Press) {
        std::cout << "Press" << std::endl;
    } else if (evtype == EvType::Repeat) {
        std::cout << "Repeat" << std::endl;
    } else if (evtype == EvType::Release) {
        std::cout << "Release" << std::endl;
    }
}

auto main() -> int {
    if (!setlocale(LC_ALL, "")) {
        std::cerr << "Error setting locale\n";
        return EXIT_FAILURE;
    }

    notcurses_options ncopts{};
    ncopts.flags = NCOPTION_INHIBIT_SETLOCALE;
    NotCurses nc(ncopts);

    ncinput ni;
    std::cout << "Press keys (Ctrl+D or Enter to exit):" << std::endl;

    while (nc.get(true, &ni) != (char32_t)-1) {
        translate(ni.evtype);
        if (ni.evtype == EvType::Release) {
            std::cout << "Release! ";
        }
        std::cout << "event type: " << ni.evtype << std::endl;

        if ((ni.ctrl && ni.id == 'D') || ni.id == NCKEY_ENTER) {
            break;
        }

        std::cout << "Key pressed: " << static_cast<char>(ni.id) << " (" << ")" << std::endl;
    }

    nc.stop();
    return EXIT_SUCCESS;
}
