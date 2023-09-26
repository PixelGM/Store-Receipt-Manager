#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

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

    void setStoreTitle(const std::string& title) { storeTitle = title; }
    void setBranch(const std::string& branchName) { branch = branchName; }
    void setStreetName(const std::string& street) { streetName = street; }
    void setCityDetails(const std::string& details) { cityDetails = details; }
    void setMemberNumber(int number) { memberNumber = number; }

    void addItem(const std::string& idStr, const std::string& name, const std::string& priceStr) {
        // Convert id string to int
        int id = std::stoi(idStr);

        // Convert price string to double
        double price = std::stod(priceStr);

        Item item(id, name, price);
        items.push_back(item);
    }

    void printReceipt() const {
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
    }
};

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
        else {
            std::cout << "Invalid command. Please try again." << std::endl;
        }
    }

    return 0;
}

