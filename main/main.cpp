#include "main/main.hpp"

#include "utl/log/Log.hpp"
#include "utl/log/StdoutLogSink.hpp"

std::int32_t main()
{
    mage::test::AddressBook myAddressBook;
    auto* myPerson = myAddressBook.add_people();
    myPerson->set_name("Soham");
    myPerson->set_email("hi@mage.com");
    auto* myPhone = myPerson->add_phones();
    myPhone->set_number("957209570");
    myPhone->set_type(mage::test::Person_PhoneType_PHONE_TYPE_MOBILE);
    mage::Logger::testInfo(myAddressBook);
    mage::Logger::info(myPerson->email());
    mage::Logger::info("Hello World!");

    // TODO (soham) : extract out absl in custom wrapper
    absl::InitializeLog();
    absl::SetMinLogLevel(absl::LogSeverityAtLeast::kInfo);
    absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfinity);

    const auto mySink = std::make_unique<mage::StdoutLogSink>();
    absl::AddLogSink(mySink.get());

    MAGE_LOG_INFO("Hello World!");
    MAGE_LOG_WARN("Hi World!");

    return 0;
}
