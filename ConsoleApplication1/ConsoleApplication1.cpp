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
    std::string storeTitle;
    std::string branch;
    std::string streetName;
    std::string cityDetails;
    int memberNumber;
    std::vector<Item> items;

public:
    // Default constructor
    Receipt()
        : storeTitle("STORE TITLE"), branch("Willingdon #548"), streetName("4500 Still Creek Drive"),
        cityDetails("Burnaby, BC V5C 0E5"), memberNumber(111794251737) {
        items.push_back(Item()); // Default item (Bananas)
        items.push_back(Item(55506, "Chicken", 34.21));
    }

    // Getters and Setters Receipt
    void setStoreTitle(const std::string& title) { storeTitle = title; }
    void setBranch(const std::string& branchName) { branch = branchName; }
    void setStreetName(const std::string& street) { streetName = street; }
    void setCityDetails(const std::string& details) { cityDetails = details; }
    void setMemberNumber(int number) { memberNumber = number; }
    void setItems(const std::vector<Item>& newItems) { items = newItems; }

    std::string getStoreTitle() const { return storeTitle; }
    std::string getBranch() const { return branch; }
    std::string getStreetName() const { return streetName; }
    std::string getCityDetails() const { return cityDetails; }
    int getMemberNumber() const { return memberNumber; }
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
        std::cout << storeTitle << std::endl;
        std::cout << branch << std::endl;
        std::cout << streetName << std::endl;
        std::cout << cityDetails << std::endl << std::endl;
        std::cout << "Member " << memberNumber << std::endl;
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

        std::cout << "Subtotal " << subtotal << std::endl;
        std::cout << "Tax " << tax << " (GST " << gst << " PST " << pst << ")" << std::endl;
        std::cout << "TOTAL (" << total << ")" << std::endl;
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
    j["storeTitle"] = receipt.getStoreTitle();
    j["branch"] = receipt.getBranch();
    j["streetName"] = receipt.getStreetName();
    j["cityDetails"] = receipt.getCityDetails();
    j["memberNumber"] = receipt.getMemberNumber();

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
    std::string storeTitle = j.contains("storeTitle") && !j["storeTitle"].is_null() ? j["storeTitle"].get<std::string>() : "Default Store Title";
    std::string branch = j.contains("branch") && !j["branch"].is_null() ? j["branch"].get<std::string>() : "Default Branch";
    std::string streetName = j.contains("streetName") && !j["streetName"].is_null() ? j["streetName"].get<std::string>() : "Default Street Name";
    std::string cityDetails = j.contains("cityDetails") && !j["cityDetails"].is_null() ? j["cityDetails"].get<std::string>() : "Default City Details";
    int memberNumber = j.contains("memberNumber") && !j["memberNumber"].is_null() ? j["memberNumber"].get<int>() : 0; // Default member number as 0

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
    receipt.setStoreTitle(storeTitle);
    receipt.setBranch(branch);
    receipt.setStreetName(streetName);
    receipt.setCityDetails(cityDetails);
    receipt.setMemberNumber(memberNumber);
    receipt.setItems(items);
}

int main() {
    Receipt receipt;
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
            iss >> arg1 >> arg2;
            if (arg1 == "storetitle") {
                receipt.setStoreTitle(arg2);
            }
            else if (arg1 == "branch") {
                receipt.setBranch(arg2);
            }
            else if (arg1 == "streetname") {
                receipt.setStreetName(arg2);
            }
            else if (arg1 == "citydetails") {
                receipt.setCityDetails(arg2);
            }
            else if (arg1 == "membernumber") {
                receipt.setMemberNumber(std::stoi(arg2));
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

    return 0;
}

