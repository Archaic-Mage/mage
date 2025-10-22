use test_proto::{person::PhoneNumber, person::PhoneType, AddressBook, Person};

fn demonstrate_proto() {
    println!("\n=== Demonstrating Protobuf Usage ===\n");

    // Create a Person message
    let person = Person {
        name: Some("John Doe".to_string()),
        id: Some(1234),
        email: Some("john.doe@example.com".to_string()),
        phones: vec![
            PhoneNumber {
                number: Some("555-1234".to_string()),
                r#type: Some(PhoneType::Mobile.into()),
            },
            PhoneNumber {
                number: Some("555-5678".to_string()),
                r#type: Some(PhoneType::Home.into()),
            },
        ],
    };

    println!("{:?}", person);

    // Create an AddressBook with multiple people
    let address_book = AddressBook {
        people: vec![
            person.clone(),
            Person {
                name: Some("Jane Smith".to_string()),
                id: Some(5678),
                email: Some("jane.smith@example.com".to_string()),
                phones: vec![PhoneNumber {
                    number: Some("555-9999".to_string()),
                    r#type: Some(PhoneType::Work.into()),
                }],
            },
        ],
    };

    println!(
        "\nCreated AddressBook with {} people",
        address_book.people.len()
    );

    // Serialize to bytes (this is what prost does)
    use prost::Message;
    let serialized = address_book.encode_to_vec();
    println!("Serialized size: {} bytes", serialized.len());

    // Deserialize back
    let deserialized = AddressBook::decode(&serialized[..]).expect("Failed to deserialize");
    println!(
        "Deserialized {} people from bytes",
        deserialized.people.len()
    );

    println!("\n=== Proto demonstration complete ===\n");
}

fn main() {
    // Demonstrate protobuf usage
    demonstrate_proto();
}
