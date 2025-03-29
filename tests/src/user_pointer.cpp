#include <iostream>

#include <exlib/core/user_pointer.hpp>

struct TestStruct {
    int value;
    TestStruct(int v) : value(v) {}
};

void test_user_pointer() {
    using namespace ex;

    // Create test values
    int int_value = 42;
    std::string str_value = "Hello World";
    TestStruct test_struct(100);

    // Test set() and get()
    UserPointer::set("int_ptr", &int_value);
    UserPointer::set("str_ptr", &str_value);
    UserPointer::set("struct_ptr", &test_struct);

    // Verify set() and get()
    int* retrieved_int = UserPointer::get<int>("int_ptr");
    std::string* retrieved_str = UserPointer::get<std::string>("str_ptr");
    TestStruct* retrieved_struct = UserPointer::get<TestStruct>("struct_ptr");

    if (retrieved_int && *retrieved_int == 42) {
        std::cout << "int_ptr test passed" << std::endl;
    }
    else {
        std::cout << "int_ptr test failed" << std::endl;
    }

    if (retrieved_str && *retrieved_str == "Hello World") {
        std::cout << "str_ptr test passed" << std::endl;
    }
    else {
        std::cout << "str_ptr test failed" << std::endl;
    }

    if (retrieved_struct && retrieved_struct->value == 100) {
        std::cout << "struct_ptr test passed" << std::endl;
    }
    else {
        std::cout << "struct_ptr test failed" << std::endl;
    }

    // Test find()
    if (UserPointer::find("int_ptr")) {
        std::cout << "find(int_ptr) test passed" << std::endl;
    }
    else {
        std::cout << "find(int_ptr) test failed" << std::endl;
    }

    if (!UserPointer::find("unknown_ptr")) {
        std::cout << "find(unknown_ptr) test passed" << std::endl;
    }
    else {
        std::cout << "find(unknown_ptr) test failed" << std::endl;
    }

    // Test remove()
    UserPointer::remove("int_ptr");
    if (!UserPointer::find("int_ptr")) {
        std::cout << "remove(int_ptr) test passed" << std::endl;
    }
    else {
        std::cout << "remove(int_ptr) test failed" << std::endl;
    }

    // Test clear()
    UserPointer::clear();
    if (!UserPointer::find("str_ptr") && !UserPointer::find("struct_ptr")) {
        std::cout << "clear() test passed" << std::endl;
    }
    else {
        std::cout << "clear() test failed" << std::endl;
    }
}

int main() {
    test_user_pointer();
    
    std::cin.get();
}
