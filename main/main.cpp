#include "main/main.hpp"

std::int32_t main() {

    test::AddressBook myAddressBook;
    auto* myPerson = myAddressBook.add_people();
    myPerson->set_name("Soham");
    myPerson->set_email("hi@mage.com");
    auto* myPhone = myPerson->add_phones();
    myPhone->set_number("957209570");
    myPhone->set_type(test::Person_PhoneType_PHONE_TYPE_MOBILE);
    Logger::testInfo(myAddressBook);
    Logger::info(myPerson->email());
    Logger::info("Hello World!");
    return 0;
}