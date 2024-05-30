#include <gtest/gtest.h>

#include <big_integer.h>
#include <client_logger.h>
#include <operation_not_supported.h>

logger *create_logger(
    std::vector<std::pair<std::string, logger::severity>> const &output_file_streams_setup,
    bool use_console_stream = true,
    logger::severity console_stream_severity = logger::severity::debug)
{
    logger_builder *builder = new client_logger_builder();
    
    if (use_console_stream)
    {
        builder->add_console_stream(console_stream_severity);
    }
    
    for (auto &output_file_stream_setup: output_file_streams_setup)
    {
        builder->add_file_stream(output_file_stream_setup.first, output_file_stream_setup.second);
    }
    
    logger *built_logger = builder->build();
    
    delete builder;
    
    return built_logger;
}

TEST(positive_tests, test1)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("32850346459076457453464575686784654");
    big_integer bigint_2("423534596495087569087908753095322");
    big_integer result_of_sum = bigint_1 + bigint_2;
    
    std::stringstream ss;
    ss << result_of_sum;
    std::string result_string = ss.str();
    
    EXPECT_TRUE(result_string == "33273881055571545022552484439879976");
    
    delete logger;
}

TEST(positive_tests, test2)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("32850346459076457453464575686784654");
    big_integer bigint_2("0000042353459649508756908790875309532245366457546765745645647567575");
    
    big_integer result_of_sub = bigint_1 - bigint_2;
    
    std::stringstream ss;
    ss << result_of_sub;
    std::string result_string = ss.str();
    
    EXPECT_TRUE(result_string == "-42353459649508756908790875276681898907381089312281069960782921");
    
    delete logger;
}

TEST(positive_tests, test3)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });

    big_integer bigint_1("32850346459076457453464575686784654");
    big_integer bigint_2("12342357553253");

    big_integer result_of_remainder = bigint_1 % bigint_2;

    std::stringstream ss;
    ss << result_of_remainder;
    std::string result_string = ss.str();

    EXPECT_TRUE(result_string == "3232571319826");

    delete logger;
}

TEST(positive_tests, test4)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("-54357893745346457544353");
    big_integer bigint_2("-54357893745346457544354");
    
    EXPECT_TRUE(bigint_1 > bigint_2);
    
    delete logger;
}

TEST(positive_tests, test5)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("423895435432");
    big_integer bigint_2("423895435432");
    
    EXPECT_TRUE(bigint_1 >= bigint_2);
    
    delete logger;
}

TEST(positive_tests, test6)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("-423895435432312432534645756753");
    big_integer bigint_2("0");
    
    EXPECT_TRUE(bigint_1 < bigint_2);
    
    delete logger;
}

TEST(positive_tests, test7)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("4238954354324222200000000");
    big_integer bigint_2("4238954354324222222222234");
    
    EXPECT_TRUE(bigint_1 <= bigint_2);
    
    delete logger;
}

TEST(positive_tests, test8)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("4238954354321");
    big_integer bigint_2("423895435432");
    
    EXPECT_TRUE(bigint_1 != bigint_2);
    
    delete logger;
}

TEST(positive_tests, test9)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_integer bigint_1("-00000044234235347865897389456748953795739648996453238954354321");
    big_integer bigint_2("-00000044234235347865897389456748953795739648996453238954354321");
    
    EXPECT_TRUE(bigint_1 == bigint_2);
    
    delete logger;
}

int main(
    int argc,
    char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

    std::vector<int> a = {0, 0, 1};
    std::vector<int> b = {0, 0, 1};
    big_integer bigint_1(a, nullptr);
    big_integer bigint_2(b, nullptr);

    bigint_1.dump_value(std::cout);
    std::cout << "\n";

    bigint_1 += bigint_2;
    bigint_1.dump_value(std::cout);
    std::cout << "\n";

    bigint_1 |= bigint_2;
    bigint_1.dump_value(std::cout);
    std::cout << "\n";

    bigint_1 &= bigint_2;
    bigint_1.dump_value(std::cout);
    std::cout << "\n";

    bigint_1 ^= bigint_2;
    bigint_1.dump_value(std::cout);
    std::cout << "\n";

    if (bigint_1 != bigint_2)
    {
        std::cout << "== +\n";
    }
    if (bigint_1 < bigint_2)
    {
        std::cout << "< +\n";
    }
    if (!(bigint_1 > bigint_2))
    {
        std::cout << "> +\n";
    }

    bigint_1 = ~bigint_1;
    bigint_1.dump_value(std::cout);
    std::cout << "\n";

    bigint_1 = ~bigint_1;
    bigint_1 += bigint_2;
    bigint_1 += bigint_2;
    bigint_1 -= bigint_2;
    bigint_1.dump_value(std::cout);
    std::cout << "\n";

    bigint_1 = -bigint_1;
    bigint_1.dump_value(std::cout);
    std::cout << "\n";

    std::cout << "\n";
    std::vector<int> c = {INT_MAX};
    std::vector<int> d = {256};
    big_integer bigint_3(c, nullptr);
    big_integer bigint_4(d, nullptr);
    bigint_3.dump_value(std::cout);
    std::cout << "\n";
    bigint_4.dump_value(std::cout);
    std::cout << "\n";
    std::vector<int> f = {1};
    big_integer bigint_5(f, nullptr);
//    bigint_5.dump_value(std::cout);
//    std::cout << "\n";

    bigint_3 *= bigint_4;
    bigint_3.dump_value(std::cout);
    std::cout << "\n";

    bigint_3 *= bigint_4;
    bigint_3.dump_value(std::cout);
    std::cout << "\n";

    std::cout << "res: \n";
    big_integer c1(std::vector<int>{8});
    big_integer d1(std::vector<int>{2,3,1});
    big_integer h = c1 * d1;
    h.dump_value(std::cout);
    std::cout << "\n\n";

    h <<= 3;
    h.dump_value(std::cout);
    std::cout << "\n";
    h <<= 32;
    h.dump_value(std::cout);
    std::cout << "\n";

    h >>= 32;
    h.dump_value(std::cout);
    std::cout << "\n";
    h >>= 3;
    h.dump_value(std::cout);
    std::cout << "\n";
    std::cout << "\n";

    big_integer div_1(std::vector<int>{1000});
    big_integer div_2(std::vector<int>{10});
    big_integer r = div_1 / div_2;
    r.dump_value(std::cout);
    std::cout << "\n";

    r *= h;
    r /= h;
    r.dump_value(std::cout);
    std::cout << "\n";

    std::cout << "\n";
    big_integer caaa(std::vector<int>{1,7,8,9});
    big_integer daaa(std::vector<int>{2,3,1});
    caaa.dump_value(std::cout);
    std::cout << "\n";
    big_integer ha = caaa * daaa;
//    ha.dump_value(std::cout);
//    std::cout << "\n";
    ha /= daaa;
    ha.dump_value(std::cout);
    std::cout << "\n";
    std::cout << (caaa == ha) << std::endl;
    std::cout << "\n";

    big_integer c123(std::vector<int>{1,7,8,9});
    big_integer d123(std::vector<int>{2,3,1});
    big_integer h123 = c123 / d123;
    h123.dump_value(std::cout);
    std::cout << "\n";

    std::cout << "\n";
    big_integer hf(std::vector<int>{1});
    hf <<= 31;
    hf.dump_value(std::cout);
    std::cout << "\n";

    std::cout << "\n";
    big_integer div_o1(std::vector<int>{100});
    big_integer div_o2(std::vector<int>{33});
    big_integer ost = div_o1 % div_o2;
    ost.dump_value(std::cout);
    std::cout << "\n";

    std::cout << "\n";
    std::cout << "mult:\n";
    big_integer bigint_1m("2423545763");
    big_integer bigint_2m("3657687978");
    std::cout << bigint_1m << std::endl;
    std::cout << bigint_2m << std::endl;
    bigint_1m.dump_value(std::cout);
    std::cout << "\n";
    bigint_2m.dump_value(std::cout);
    std::cout << "\n";
    bigint_1m *=bigint_2m;
    std::cout << "8864574201457937214" << std::endl;
    std::cout << bigint_1m << std::endl;
    bigint_1m.dump_value(std::cout);
    std::cout << "\n";
    big_integer bigint_1mm("8864574201457937214");
    bigint_1mm.dump_value(std::cout);
    std::cout << "\n";
}