#include "../include/iot/app.hpp"

int main(int argc, char* argv[]) {
    const iot::config::AppConfig config{
        iot::config::parse_arguments(argc, argv)
    };

    if (config.show_help) {
        iot::config::print_usage(argv[0]);
        return 0;
    }

    if (!config.valid) {
        iot::config::print_usage(argv[0]);
        return 1;
    }
    return iot::app::run(config);
}