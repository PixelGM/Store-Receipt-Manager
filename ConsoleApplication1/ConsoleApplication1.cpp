#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include "../nlohmann/json.hpp"
#include <fstream>

class Item {
private:
    int id;
    std::string name;
    double price;

public:
    // Default constructor
    Item() : id(30669), name("Bananas"), price(1.99) {}

    Item(int id, const std::string& name, double price)
        : id(id), name(name), price(price) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }
};

class Receipt {
private:
    std::string title;
    std::string branch;
    std::string street;
    std::string city;
    int member;
    std::vector<Item> items;

public:
    // Default constructor
    Receipt()
        : title("STORE TITLE..."), branch("Branch..."), street("Street..."),
        city("City..."), member(000000000000) {
        items.push_back(Item()); // Default item (Bananas)
        items.push_back(Item(55506, "Chicken", 34.21));
    }

    // Getters and Setters Receipt
    void setTitle(const std::string& title) { this->title = title; }
    void setBranch(const std::string& branchName) { branch = branchName; }
    void setStreet(const std::string& street) { this->street = street; }
    void setCity(const std::string& details) { city = details; }
    void setMember(int number) { member = number; }
    void setItems(const std::vector<Item>& newItems) { items = newItems; }

    std::string getTitle() const { return title; }
    std::string getBranch() const { return branch; }
    std::string getStreet() const { return street; }
    std::string getCity() const { return city; }
    int getMember() const { return member; }
    const std::vector<Item>& getItems() const { return items; }

    void addItem(const std::string& idStr, const std::string& name, const std::string& priceStr) {
        // Convert id string to int
        int id = std::stoi(idStr);

        // Convert price string to double
        double price = std::stod(priceStr);

        Item item(id, name, price);
        items.push_back(item);
    }

    void printReceipt() const {
        std::cout << std::endl;
        std::cout << "---------------------------" << std::endl;
        std::cout << title << std::endl;
        std::cout << branch << std::endl;
        std::cout << street << std::endl;
        std::cout << city << std::endl << std::endl;
        std::cout << "Member " << member << std::endl;
        std::cout << std::endl;

        double subtotal = 0.0;
        for (const auto& item : items) {
            std::cout << item.getId() << " " << item.getName() << " " << std::fixed << std::setprecision(2) << item.getPrice() << std::endl;
            subtotal += item.getPrice();
        }

        double gst = subtotal * 0.05; // Assuming 5% GST
        double pst = subtotal * 0.07; // Assuming 7% PST
        double tax = gst + pst;
        double total = subtotal + tax;

        std::cout << std::endl;
        std::cout << "Subtotal " << subtotal << std::endl;
        std::cout << "Tax " << tax << " (GST " << gst << " PST " << pst << ")" << std::endl;
        std::cout << "TOTAL (" << total << ")" << std::endl;
        std::cout << "---------------------------" << std::endl;
        std::cout << std::endl;
    }
};

nlohmann::json to_json(const Item& item) {
    nlohmann::json j;
    j["id"] = item.getId();
    j["name"] = item.getName();
    j["price"] = item.getPrice();
    return j;
}

nlohmann::json to_json(const Receipt& receipt) {
    nlohmann::json j;
    j["title"] = receipt.getTitle();
    j["branch"] = receipt.getBranch();
    j["street"] = receipt.getStreet();
    j["city"] = receipt.getCity();
    j["member"] = receipt.getMember();

    // For the items vector:
    for (const auto& item : receipt.getItems()) {
        j["items"].push_back(to_json(item));
    }
    return j;
}

// Save the Receipt object to a JSON file
void save_to_file(const Receipt& receipt, const std::string& filename) {
    nlohmann::json j = to_json(receipt);

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file for writing." << std::endl;
        return;
    }

    file << j.dump(4); // 4 spaces for indentation
}

// Load the Receipt object from a JSON file
void load_from_file(Receipt& receipt, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;

    // Check for the existence of keys and handle potential null values
    std::string title = j.contains("title") && !j["title"].is_null() ? j["title"].get<std::string>() : "Default Store Title";
    std::string branch = j.contains("branch") && !j["branch"].is_null() ? j["branch"].get<std::string>() : "Default Branch";
    std::string street = j.contains("street") && !j["street"].is_null() ? j["street"].get<std::string>() : "Default Street Name";
    std::string city = j.contains("city") && !j["city"].is_null() ? j["city"].get<std::string>() : "Default City Details";
    int member = j.contains("member") && !j["member"].is_null() ? j["member"].get<int>() : 0; // Default member number as 0

    std::vector<Item> items;
    if (j.contains("items") && j["items"].is_array()) {
        for (const auto& item_json : j["items"]) {
            int id = item_json.contains("id") && !item_json["id"].is_null() ? item_json["id"].get<int>() : 0;
            std::string name = item_json.contains("name") && !item_json["name"].is_null() ? item_json["name"].get<std::string>() : "Default Item Name";
            double price = item_json.contains("price") && !item_json["price"].is_null() ? item_json["price"].get<double>() : 0.0;
            items.push_back(Item(id, name, price));
        }
    }

    // Set the values to the receipt object
    receipt.setTitle(title);
    receipt.setBranch(branch);
    receipt.setStreet(street);
    receipt.setCity(city);
    receipt.setMember(member);
    receipt.setItems(items);
}

void processCommand(Receipt& receipt) {
    std::string command;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "/exit") {
            break;
        }

        std::istringstream iss(command);
        std::string cmd, arg1, arg2, arg3, arg4;

        iss >> cmd;

        if (cmd == "/set") {
            iss >> arg1;
            std::getline(iss, arg2); // Read the rest of the line after the first argument
            arg2 = arg2.substr(1);   // Remove the leading space

            if (arg1 == "title") {
                receipt.setTitle(arg2);
            }
            else if (arg1 == "branch") {
                receipt.setBranch(arg2);
            }
            else if (arg1 == "street") {
                receipt.setStreet(arg2);
            }
            else if (arg1 == "city") {
                receipt.setCity(arg2);
            }
            else if (arg1 == "member") {
                receipt.setMember(std::stoi(arg2));
            }
        }
        else if (cmd == "/additem") {
            iss >> arg1 >> arg2 >> arg3;
            receipt.addItem(arg1, arg2, arg3);
        }
        else if (cmd == "/print") {
            receipt.printReceipt();
        }
        else if (cmd == "/save") {
            save_to_file(receipt, "receipt.json");
        }
        else if (cmd == "/load") {
            try {
                load_from_file(receipt, "receipt.json");
            }
            catch (const nlohmann::json::exception& e) {
                std::cerr << "JSON error: " << e.what() << std::endl;
            }
        }
        else {
            std::cout << "Invalid command. Please try again." << std::endl;
        }
    }
}


int main() {
    Receipt receipt;
    processCommand(receipt);
    return 0;
}

