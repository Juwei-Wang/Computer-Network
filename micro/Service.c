/* A2 UCID:30053278                                   */
/* Written by Juwei wang  2020.2.28                   */

#include "Service.h"



Manager* get_all_service() {

    static Manager* MICRO_MANAGER = NULL;
    int n = 0;
    int service_count = 6;
    if (MICRO_MANAGER == NULL) {
        MICRO_MANAGER = (Manager*)malloc(sizeof(Manager));
        MICRO_MANAGER->services = (Service*)malloc(service_count* sizeof(Service));
        MICRO_MANAGER->services[0].handle = Identity;
        MICRO_MANAGER->services[0].host = "127.0.0.1";
        MICRO_MANAGER->services[0].port = 21231;

        MICRO_MANAGER->services[1].handle = Reverse;
        MICRO_MANAGER->services[1].host = "127.0.0.1";
        MICRO_MANAGER->services[1].port = 21232;

        MICRO_MANAGER->services[2].handle = Upper;
        MICRO_MANAGER->services[2].host = "127.0.0.1";
        MICRO_MANAGER->services[2].port = 21233;

        MICRO_MANAGER->services[3].handle = Lower;
        MICRO_MANAGER->services[3].host = "127.0.0.1";
        MICRO_MANAGER->services[3].port = 21234;

        MICRO_MANAGER->services[4].handle = Caesar;
        MICRO_MANAGER->services[4].host = "127.0.0.1";
        MICRO_MANAGER->services[4].port = 21235;

        MICRO_MANAGER->services[5].handle = Yours;
        MICRO_MANAGER->services[5].host = "127.0.0.1";
        MICRO_MANAGER->services[5].port = 21236;

        MICRO_MANAGER->count = 6;
    }

    return MICRO_MANAGER;
}







