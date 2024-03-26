#include <gtest/gtest.h>
#include <iostream>
#include "../include/client_logger_builder.h"
#include "client_logger.h"


int main(
    int argc,
    char *argv[])
{
    client_logger_builder new_logger("%d %t %s %m");
    //client_logger_builder new_logger;

//    logger *log = new_logger.add_console_stream(logger::severity::information)
//            ->add_file_stream("./tst_1", logger::severity::information)
//            ->add_file_stream("./tst_2", logger::severity::error)
//            ->build();

    logger *log = new_logger.add_console_stream(logger::severity::information)
            ->transform_with_configuration("C:\\Users\\User\\mp_os\\logger\\client_logger\\tests\\conf_1", "block_2/log_2")
            ->build();

    log->information("information");
    log->error("error");
    log->critical("critical");
    log->trace("trace");
    log->debug("debug");
    log->warning("warning");
}